#include "EEPROM_Emulator.h"

/* Logical Page Organization : Header + Addr_Data
 *
 * Header    : 0xFFFFFFFF, 0xFFFFFFFE, 0xFFFFFFFC ------------------> 4 Bytes
 * Addr_Data : HighWord = Addr, LowWord = Data < 2 byte var > : ----> ( (EepromPageSize/4) - 1 ) * Word
 * AddrRange : From 0x0000 to 0xFFFE can be used, 0xFFFF is used for Header
 *
 * Wear-leveling implementation
 *
 * In order to enhance the emulated EEPROM capacity, four pages will be used (Page0, Page1, Page2 and Page3).
 * The wear-leveling algorithm is implemented as follows:
 * when page n is full, the device switches to page n+1. Page n is garbage-collected and then erased.
 * When it is the turn of Page3 to be full, goes back to Page0, Page3 is garbage-collected then erased and so on.
 *
 * Page Status Change
 *                                                                                      Page0     Page1       Page2
 * A) After Erased, Before Formatted, All pages are PAGE_STATUS_ERASED                  ERASED    ERASED      ERASED
 * B) After Formatted, Mark Page0 as PAGE_STATUS_VALID                                  VALID     ERASED      ERASED
 * C) After Page0 is full, Mark Page1 as PAGE_STATUS_RECEIVING                          VALID     RECEIVING   ERASED
 * D) After Transferred Page0 to Page1, Mark Page1 as PAGE_STATUS_VALID                 VALID     VALID       ERASED
 * E) After Page1 as PAGE_STATUS_VALID, Erased Page0 and Mark it as PAGE_STATUS_ERASED  ERASED    VALID       ERASED
 *
 * Page Status Check and Action
 *
 * Data or page header corruption is possible in case of a power loss during a variable update, page erase or transfer.
 *
 * 1) if All pages are ERASED ---------------------------> Eeprom need be Formated with init data
 * 2) Only 1 page is VALID, other pages are ERASED ------> Do nothing
 * 3) Page N is VALID, Page N+1 is RECEIVING ------------> Transfer Page N to Page N+1
 * 4) Page N is VALID, Page N+1 is VALID ----------------> Erase Page N
 *
 * 5) the corruption between A) and B) ------------------> Format with init data
 * 6) the corruption between B) and C) ------------------> Mark New Page as RECEIVING
 * 7) the corruption between C) and D) ------------------> Transfer again
 * 8) the corruption between D) and E) ------------------> Mark Old Page as PAGE_STATUS_ERASED
 *
 */

/* Page status definitions
 *
 * STM32F10X : 0xFFFF --> 0x???? -----> 0x0000
 * PGOK      : 0xFFFF --> 0xFFFE -----> 0x0000
 * PGERR     : 0xFFFF --> 0xFFFE --X--> 0xFFFC
 *
 */
#define PAGE_STATUS_ERASED      ( (uint32_t) 0xFFFFFFFF )     /* PAGE is empty */
#define PAGE_STATUS_RECEIVING   ( (uint32_t) 0xFFFFFFFE )     /* PAGE is marked to receive data */
#define PAGE_STATUS_VALID       ( (uint32_t) 0xFFFF0000 )     /* PAGE containing valid data */
#define PAGE_STATUS_BREAKDOWN   ( (uint32_t) 0x00000000 )     /* PAGE is bad */

#define MAX_EEPROM_PAGE_COUNT   ( 32 )                        /* Bitmap is 32 bits */
#define INVALID_PAGE_INDEX      ( -1 )

typedef struct
{
  uint8_t ErasedPageCount;
  uint8_t ValidPageCount;
  uint8_t ReceivingdPageCount;

  int8_t FirstErasedPageIndex;
  int8_t SecondErasedPageIndex;

  int8_t FirstValidPageIndex;
  int8_t SecondValidPageIndex;

  int8_t ReceivingPageIndex;
} EEPROM_PagesStatus;

typedef struct
{
  const EEPROM_Configuration * Configuration;
  const Driver_EEPROM * Driver;

  uint8_t * Cache;                  // Cache to Read, Update while Write

  uint16_t EepromWordCount;         //
  uint8_t EepromPageCount;          //
  uint8_t EepromPageRatio;          // 1 Eeprom Page = N Flash Pages -------- Logical Page Size

  uint32_t BadPageBtimap;           // Bit=0 : good, Bit=1 : bad
  uint32_t AddrToWrite;             // Header <----Read--- AddrToWrite -----Write---> End
  int8_t PageToRead;                // -1 : Find at first
  int8_t PageToWrite;               // -1 : Find at first
  int8_t PageToTransfer;            // -1 : Find at first

} EEPROM_Context;

static EEPROM_Context Context;

static uint32_t EepromPage2FlashPage( int8_t PageIndex )
{
  return PageIndex * Context.EepromPageRatio;
}

static uint32_t EEPROM_PageBaseAddress( int8_t PageIndex )
{
  // Address of First DWord -- Header
  return Context.Configuration->FlashPageBase + Context.Configuration->FlashPageSize * EepromPage2FlashPage( PageIndex );
}

static uint32_t EEPROM_PageLastAddress( int8_t PageIndex )
{
  // Address of Last DWord in Eeprom Page
  return EEPROM_PageBaseAddress( PageIndex + 1 ) - 4;
}

static uint8_t EEPROM_NextPageIndex( int8_t PageIndex )
{
  uint32_t NextPageIndex = PageIndex + 1;
  if ( NextPageIndex == Context.EepromPageCount )
    NextPageIndex = 0;

  return NextPageIndex;
}

static uint32_t EEPROM_QueryPageStatus( int8_t PageIndex )
{
  uint32_t PageAddress = EEPROM_PageBaseAddress( PageIndex );
  return *( (volatile uint32_t*) ( PageAddress ) );
}

static int32_t EEPROM_ChangePageStatus( int8_t PageIndex, uint32_t PageStatus )
{
  int32_t RetValue = EEPROM_PAGE_FULL;

  if ( EEPROM_QueryPageStatus( PageIndex ) != PageStatus )
  {
    uint32_t PageAddress = EEPROM_PageBaseAddress( PageIndex );
    if ( PAGE_STATUS_ERASED == PageStatus )
    {
      // Erase Header Page and All Data Pages
      // FlashErasePage() will Checks if all the bits in the page are 1's.
      RetValue = Context.Driver->FlashErasePage( PageAddress, Context.EepromPageRatio );
    }
    else
    {
      // Change Header only
      // FlashProgram() will Verity Whether Value is programmed OK or not
      RetValue = Context.Driver->FlashProgram( PageAddress, PageStatus );
    }
  }

  // EEPROM_SUCCESS or EEPROM_FLASH_FAULT < Erase or Program Failed >
  return RetValue;
}

static void EEPROM_CalculateParameter( void )
{
  const EEPROM_Configuration * Configuration = Context.Configuration;

  uint32_t Capacity = Configuration->EepromCapacity;
  uint32_t DataSize = ( ( Capacity + Capacity >> 2 ) >> 1 ) << 2;     // Capacity *= 1.25, ( Capacity / 2 ) * 4

  Context.EepromWordCount = ( Capacity + 1 ) >> 1;                    // Count In Word

  Context.EepromPageRatio = ( ( 4 + DataSize ) + Configuration->FlashPageSize - 1 ) / Configuration->FlashPageSize;
  Context.EepromPageCount = Configuration->FlashPageCount / Context.EepromPageRatio;

  if ( Context.EepromPageCount > MAX_EEPROM_PAGE_COUNT )
    Context.EepromPageCount = MAX_EEPROM_PAGE_COUNT;

  // 0 - Not Available
  // 1 - Read Only
  // 2 - Read and Write
  if ( Context.EepromPageCount < 2 )
  {
    for ( ;; )
      ;
  }
}

static int32_t EEPROM_FindTransferPage( int8_t ValidPage )
{
  int8_t TransferPage = INVALID_PAGE_INDEX;
  int8_t PageIndex = ValidPage;
  do
  {
    PageIndex = EEPROM_NextPageIndex( PageIndex );
    if ( 0 == ( Context.BadPageBtimap & ( 1 << PageIndex ) ) )
    {
      if ( PAGE_STATUS_ERASED == EEPROM_QueryPageStatus( PageIndex ) )
      {
        TransferPage = PageIndex;
        break;
      }
    }
  }while ( PageIndex != ValidPage );

  return TransferPage;
}

static void EEPROM_QueryPagesStatus( EEPROM_PagesStatus * PagesStatus )
{
  int8_t PageIndex;
  uint32_t PageStatus, PageMask;

  Context.BadPageBtimap = 0;

  PagesStatus->ErasedPageCount = 0;
  PagesStatus->ValidPageCount = 0;
  PagesStatus->ReceivingdPageCount = 0;

  PagesStatus->FirstErasedPageIndex = INVALID_PAGE_INDEX;
  PagesStatus->SecondErasedPageIndex = INVALID_PAGE_INDEX;
  PagesStatus->FirstValidPageIndex = INVALID_PAGE_INDEX;
  PagesStatus->SecondValidPageIndex = INVALID_PAGE_INDEX;
  PagesStatus->ReceivingPageIndex = INVALID_PAGE_INDEX;

  // Query All Pages Status
  for ( PageIndex = 0; PageIndex < Context.EepromPageCount; PageIndex++ )
  {
    PageStatus = EEPROM_QueryPageStatus( PageIndex );

    if ( PageStatus == PAGE_STATUS_ERASED )
    {
      PagesStatus->ErasedPageCount++;
      if ( INVALID_PAGE_INDEX == PagesStatus->FirstErasedPageIndex )
        PagesStatus->FirstErasedPageIndex = PageIndex;
      else if ( INVALID_PAGE_INDEX == PagesStatus->SecondErasedPageIndex )
        PagesStatus->SecondErasedPageIndex = PageIndex;
    }
    else if ( PageStatus == PAGE_STATUS_RECEIVING )
    {
      PagesStatus->ReceivingdPageCount++;
      if ( INVALID_PAGE_INDEX == PagesStatus->ReceivingPageIndex )
        PagesStatus->ReceivingPageIndex = PageIndex;
    }
    else if ( PageStatus == PAGE_STATUS_VALID )
    {
      PagesStatus->ValidPageCount++;
      if ( INVALID_PAGE_INDEX == PagesStatus->FirstValidPageIndex )
        PagesStatus->FirstValidPageIndex = PageIndex;
      else if ( INVALID_PAGE_INDEX == PagesStatus->SecondValidPageIndex )
        PagesStatus->SecondValidPageIndex = PageIndex;
    }
    else
    {
      PageMask = ( 1 << PageIndex );
      Context.BadPageBtimap |= PageMask;
    }
  }
}

// Build Cache -- Gather the most recently written value of each variable
int32_t EEPROM_BuildCache( void )
{
  int32_t RetValue = EEPROM_SUCCESS;

  uint32_t Pair;
  uint32_t WordAddr;
  uint16_t * WordCache = (uint16_t *) Context.Cache;

  // Mark Unused Word as 0xFFFF
  for ( WordAddr = 0; WordAddr < Context.EepromWordCount; WordAddr++ )
    WordCache[ WordAddr ] = 0xFFFF;

  /* Get the valid Page start and end Address */
  uint32_t BaseAddress = EEPROM_PageBaseAddress( Context.PageToRead );
  uint32_t LastAddress = EEPROM_PageLastAddress( Context.PageToRead );

  for ( BaseAddress += 4; BaseAddress <= LastAddress; BaseAddress += 4 )
  {
    RetValue = Context.Driver->FlashRead( BaseAddress, &Pair );
    if ( EEPROM_SUCCESS != RetValue )
      return RetValue;

    WordAddr = Pair >> 16;
    if ( WordAddr < Context.EepromWordCount )
      WordCache[ WordAddr ] = Pair;
  }

  return RetValue;
}

static int32_t EEPROM_WritePair( uint32_t Addr, uint16_t Data )
{
  int32_t RetValue;

  uint16_t OldValue;
  uint32_t ValueToWrite;

  RetValue = EEPROM_ReadWord( Addr, &OldValue );
  if ( EEPROM_SUCCESS != RetValue )
    return RetValue;

  if ( ( Context.PageToWrite == Context.PageToRead ) && ( OldValue == Data ) )
    return EEPROM_SUCCESS;

  /* Get the Page start and Address */
  uint32_t LastAddress = EEPROM_PageLastAddress( Context.PageToWrite );
  uint32_t BaseAddress = EEPROM_PageBaseAddress( Context.PageToWrite );
  if ( Context.AddrToWrite )
    BaseAddress = Context.AddrToWrite - 4;

  while ( BaseAddress < LastAddress )
  {
    BaseAddress += 4;     // Next address location

    /* Verify if Address contents are 0xFFFFFFFF */
    RetValue = Context.Driver->FlashRead( BaseAddress, &ValueToWrite );
    if ( EEPROM_SUCCESS != RetValue )
      return RetValue;

    if ( 0xFFFFFFFF != ValueToWrite )
      continue;

    ValueToWrite = ( ( Addr >> 1 ) << 16 ) | Data;      // Gave Byte Address

    RetValue = Context.Driver->FlashProgram( BaseAddress, ValueToWrite );
    if ( EEPROM_SUCCESS != RetValue )
      return RetValue;

    // Update Offset and Cache
    Context.AddrToWrite = BaseAddress + 4;
    *(uint16_t *) &Context.Cache[ Addr ] = Data;        // Gave Byte Address
    return EEPROM_SUCCESS;
  }

  return EEPROM_PAGE_FULL;
}

static int32_t EEPROM_TransferPage( uint32_t Addr, uint16_t Data )
{
  // (1) Mark New Page as PAGE_STATUS_RECEIVING
  int32_t RetValue = EEPROM_FLASH_FAULT;
  if ( ( INVALID_PAGE_INDEX == Context.PageToTransfer ) || ( Context.PageToTransfer == Context.PageToRead ) )
    return RetValue;    // No available page to transfer

  RetValue = EEPROM_ChangePageStatus( Context.PageToTransfer, PAGE_STATUS_RECEIVING );
  if ( EEPROM_SUCCESS != RetValue )
    return RetValue;

  // (2) Transfer Old Page to New Page
  Context.AddrToWrite = 0;
  Context.PageToWrite = Context.PageToTransfer;

  uint32_t WordIndex = 0;
  uint16_t * WordCache = (uint16_t *) Context.Cache;

  for ( ; WordIndex < Context.EepromWordCount; WordIndex++ )
  {
    if ( WordIndex == ( Addr >> 1 ) )
      WordCache[ WordIndex ] = Data;          // Update Cache

    RetValue = EEPROM_WritePair( WordIndex << 1, WordCache[ WordIndex ] );
    if ( EEPROM_SUCCESS != RetValue )         // this page is breakdown
      return RetValue;
  }

  // (3) Mark New Page as PAGE_STATUS_VALID
  RetValue = EEPROM_ChangePageStatus( Context.PageToWrite, PAGE_STATUS_VALID );
  if ( EEPROM_SUCCESS != RetValue )         // this page is breakdown
    return RetValue;

  // (4) Mark Old Page as PAGE_STATUS_ERASED
  RetValue = EEPROM_ChangePageStatus( Context.PageToRead, PAGE_STATUS_ERASED );
  if ( EEPROM_SUCCESS != RetValue )         // this page is breakdown
    return RetValue;

  // (5) Update Status Information

  // Context.AddrToWrite is updated by EEPROM_WritePair()
  Context.PageToRead = Context.PageToWrite;
  Context.PageToTransfer = EEPROM_FindTransferPage( Context.PageToRead );

  return EEPROM_SUCCESS;
}

// Check for invalid header states and repair if necessary
// Restore the pages to a known good state in case of page's status corruption after a power loss.
//
// Normal status, Only 1 Page is VALID, EEPROM is ready to read and write
// Virgin Status, All pages are ERASED, EEPROM is ready to Format with init data
//
// ERASED     ERASED      ERASED    ERASED ---- Virgin Status to be formatted
//
// ERASED     ERASED      ERASED    VALID ----- Normal Satus, Page3 for Reading and Writing
// ERASED     ERASED      VALID     ERASED ---- Normal Satus, Page2 for Reading and Writing
// ERASED     VALID       ERASED    ERASED ---- Normal Satus, Page1 for Reading and Writing
// VALID      ERASED      ERASED    ERASED ---- Normal Satus, Page0 for Reading and Writing
//
// VALID      ERASED      ERASED    ERASED ---- Normal Satus, Page0 for Reading and Writing --- (1) Do Nothing
// VALID      RECEIVING   ERASED    ERASED ---- Failed Status, Transferring Page0 to Page1 ---- (2) Erase Page1
// VALID      VALID       ERASED    ERASED ---- Failed Status, Transferring Page0 to Page1 ---- (3) Erase Page0
// ERASED     VALID       ERASED    ERASED ---- Normal Satus, Page1 for Reading and Writing --- (4) Do Nothing
//
// ERASED     ERASED      ERASED    VALID ----- Normal Satus, Page3 for Reading and Writing --- (1) Do Nothing
// RECEIVING  ERASED      ERASED    VALID ----- Failed Status, Transferring Page3 to Page0 ---- (2) Erase Page0
// VALID      ERASED      ERASED    VALID ----- Failed Status, Transferring Page3 to Page0 ---- (3) Erase Page3
// VALID      ERASED      ERASED    ERASED ---- Normal Satus, Page0 for Reading and Writing --- (4) Do Nothing
//
// VALID      VALID                        ---- Failed Status, Transferring PageX to PageY ---- (3) Erase Which Page?
//                                         ---- Data in Last Addr is 0xFFFFFFFF is NewPage
static int32_t EEPROM_DataRecovery( EEPROM_PagesStatus * PagesStatus )
{
  int32_t RetValue = EEPROM_FLASH_FAULT;
  uint32_t LastAddress;

  Context.AddrToWrite = 0;

  // ERASED     ERASED      ERASED    ERASED ---------- Virgin Status to be formatted
  if ( ( 0 == PagesStatus->ReceivingdPageCount ) && ( 0 == PagesStatus->ValidPageCount ) )
  {
    if ( 0 == PagesStatus->ErasedPageCount )
      return RetValue;

    RetValue = EEPROM_VIRGIN;
    Context.PageToRead = PagesStatus->FirstErasedPageIndex;
    Context.PageToWrite = PagesStatus->FirstErasedPageIndex;
    Context.PageToTransfer = PagesStatus->SecondErasedPageIndex;

    return RetValue;
  }

  // PageN      PageN+1     |   PageN         PageN+1
  // VALID      ERASED      |   ERASED        VALID --- Do Nothing
  // VALID      RECEIVING   |   RECEIVING     VALID --- Erase RECEIVING Page if Transfer is Failed, else Erase Valid Page
  // VALID      VALID       |   VALID         VALID --- Erase Old Page, Data in Last Addr is not 0xFFFFFFFF
  //
  if ( 2 == PagesStatus->ValidPageCount )
  {
    LastAddress = EEPROM_PageLastAddress( PagesStatus->FirstValidPageIndex );
    if ( 0xFFFFFFFF == *( (volatile uint32_t*) ( LastAddress ) ) )
      RetValue = EEPROM_ChangePageStatus( PagesStatus->SecondValidPageIndex, PAGE_STATUS_ERASED );
    else
    {
      RetValue = EEPROM_ChangePageStatus( PagesStatus->FirstValidPageIndex, PAGE_STATUS_ERASED );
      PagesStatus->FirstValidPageIndex = PagesStatus->SecondValidPageIndex;
    }
  }
  else if ( 1 == PagesStatus->ValidPageCount )
  {
    if ( 1 == PagesStatus->ReceivingdPageCount )
      RetValue = EEPROM_ChangePageStatus( PagesStatus->ReceivingPageIndex, PAGE_STATUS_ERASED );
    else
      RetValue = EEPROM_SUCCESS;
  }

  if ( EEPROM_SUCCESS == RetValue )
  {
    Context.PageToRead = PagesStatus->FirstValidPageIndex;
    Context.PageToWrite = PagesStatus->FirstValidPageIndex;
    Context.PageToTransfer = EEPROM_FindTransferPage( Context.PageToRead );
  }

  return RetValue;
}

int32_t EEPROM_Init( const EEPROM_Configuration * Configuration, const Driver_EEPROM * Driver, uint8_t * Cache )
{
  int32_t RetValue;
  EEPROM_PagesStatus PagesStatus;

  Context.Configuration = Configuration;
  Context.Driver = Driver;
  Context.Cache = Cache;

  Context.Driver->FlashInit( Context.Configuration->FlashPageBase, Context.Configuration->FlashPageCount );

  EEPROM_CalculateParameter( );
  EEPROM_QueryPagesStatus( &PagesStatus );
  RetValue = EEPROM_DataRecovery( &PagesStatus );
  if ( EEPROM_SUCCESS == RetValue )
    EEPROM_BuildCache( );

  return RetValue;
}

int32_t EEPROM_Format( uint8_t * EepromData, uint32_t Size )
{
  int32_t RetValue;
  uint32_t PageIndex;
  uint32_t WordAddr;

  uint32_t WordCount = ( ( Size + 1 ) >> 1 );
  uint16_t * WordCache = (uint16_t *) Context.Cache;
  uint16_t * WordData = (uint16_t *) EepromData;

  int8_t FirstErasedPageIndex = INVALID_PAGE_INDEX;
  int8_t SecondErasedPageIndex = INVALID_PAGE_INDEX;

  Context.BadPageBtimap = 0;

  for ( PageIndex = 0; PageIndex < Context.EepromPageCount; PageIndex++ )
  {
    if ( PAGE_STATUS_ERASED != EEPROM_QueryPageStatus( PageIndex ) )
    {
      RetValue = EEPROM_ChangePageStatus( PageIndex, PAGE_STATUS_ERASED );
      if ( EEPROM_SUCCESS != RetValue )     // Mark Done
      {
        Context.BadPageBtimap |= ( 1 << PageIndex );
        continue;
      }
    }

    if ( INVALID_PAGE_INDEX == FirstErasedPageIndex )
      FirstErasedPageIndex = PageIndex;
    else if ( INVALID_PAGE_INDEX == SecondErasedPageIndex )
      SecondErasedPageIndex = PageIndex;
  }

  // No page to write
  if ( INVALID_PAGE_INDEX == FirstErasedPageIndex )
    RetValue = EEPROM_FLASH_FAULT;

  Context.AddrToWrite = 0;
  Context.PageToRead = INVALID_PAGE_INDEX;    // Force Write
  Context.PageToWrite = FirstErasedPageIndex;

  do
  {
    for ( WordAddr = 0; WordAddr < WordCount; WordAddr++ )
    {
      WordCache[ WordAddr ] = WordData[ WordAddr ];

      RetValue = EEPROM_WriteWord( WordAddr << 1, WordData[ WordAddr ] );
      if ( EEPROM_SUCCESS != RetValue )       // Write Failed, Try Next Page
      {
        RetValue = EEPROM_ChangePageStatus( PageIndex, PAGE_STATUS_BREAKDOWN );
        if ( EEPROM_SUCCESS != RetValue )
          return RetValue;
        Context.BadPageBtimap |= ( 1 << Context.PageToWrite );
        Context.PageToWrite = EEPROM_FindTransferPage( Context.PageToWrite );
        RetValue = EEPROM_FLASH_FAULT;        // Write Failed, Try Next Page
        break;
      }
    }

    if ( INVALID_PAGE_INDEX == Context.PageToWrite )
      break;

    if ( EEPROM_SUCCESS == RetValue )         // Write Done
    {
      RetValue = EEPROM_ChangePageStatus( 0, PAGE_STATUS_VALID );
      if ( EEPROM_SUCCESS == RetValue )       // Mark Done
        break;
    }
  }while ( Context.PageToWrite != FirstErasedPageIndex );

  if ( EEPROM_SUCCESS == RetValue )
  {
    for ( ; WordAddr < ( Context.Configuration->EepromCapacity >> 1 ); WordAddr++ )
      WordCache[ WordAddr ] = 0xFFFF;

    Context.PageToRead = Context.PageToWrite;
    Context.PageToTransfer = EEPROM_FindTransferPage( Context.PageToRead );
  }

  return RetValue;
}

int32_t EEPROM_ReadByte( uint32_t Addr, uint8_t * Data )
{
  int32_t RetValue = EEPROM_SUCCESS;

  if ( ( Data ) && ( Addr < Context.Configuration->EepromCapacity ) )
    *Data = Context.Cache[ Addr ];    // Gave Byte Addressed

  return RetValue;
}

int32_t EEPROM_ReadWord( uint32_t Addr, uint16_t * Data )
{
  int32_t RetValue = EEPROM_SUCCESS;

  uint16_t * WordCache = (uint16_t *) Context.Cache;
  if ( ( Data ) && ( Addr < Context.Configuration->EepromCapacity ) )
    *Data = WordCache[ Addr >> 1 ];

  return RetValue;
}

int32_t EEPROM_ReadDWord( uint32_t Addr, uint32_t * Data )
{
  int32_t RetValue = EEPROM_SUCCESS;

  uint32_t * DWordCache = (uint32_t *) Context.Cache;
  if ( ( Data ) && ( Addr < Context.Configuration->EepromCapacity ) )
    *Data = DWordCache[ Addr >> 2 ];

  return RetValue;
}

int32_t EEPROM_WriteByte( uint32_t Addr, uint8_t Data )
{
  uint16_t Value;
  int32_t RetValue;

  RetValue = EEPROM_ReadWord( Addr, &Value );
  if ( EEPROM_SUCCESS != RetValue )
    return RetValue;

  if ( Addr & 1 )
    Value = ( Value & 0x00FF ) | ( Data << 8 );     // Odd Addr
  else
    Value = ( Value & 0xFF00 ) | ( Data << 0 );     // Even Addr

  return EEPROM_WriteWord( ( Addr >> 1 ) << 1, Value );
}

int32_t EEPROM_WriteWord( uint32_t Addr, uint16_t Data )
{
  int32_t RetValue = EEPROM_WritePair( Addr, Data );

  // Can not Write because Page is full, Transfer to Next Page
  if ( EEPROM_PAGE_FULL == RetValue )
    RetValue = EEPROM_TransferPage( Addr, Data );    //

  return RetValue;
}

int32_t EEPROM_WriteDWord( uint32_t Addr, uint32_t Data )
{
  int32_t RetValue;

  Addr = ( Addr >> 2 ) << 2;                            // Write to DWord Addr
  RetValue = EEPROM_WriteWord( Addr, Data >> 0 );       // First Word
  if ( EEPROM_SUCCESS == RetValue )
  {
    Addr += 2;
    RetValue = EEPROM_WriteWord( Addr, Data >> 16 );    // Other Word
  }
  return RetValue;
}

#include "stm32f10x_eeprom.h"

static const uint8_t DefaultEeprom[ 128 ] = {
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,  //
  0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,  //
  0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,  //
  0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,  //
  0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,  //
  0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,  //
  0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,  //
  0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F, };

static const EEPROM_Configuration Configuration = {
  0x08008000, 4, STM32F10X_EE_FLASH_PAGE_SIZE, 128 };

static uint8_t EEPROM_Cache[ 128 ];

void EEPROM_Demo( void )
{
  if ( EEPROM_VIRGIN == EEPROM_Init( &Configuration, &EEPROM_Driver, EEPROM_Cache ) )
    EEPROM_Format( (uint8_t *) DefaultEeprom, 128 );

  uint8_t Value;
  for ( uint32_t i = 0; i < 256; i++ )
  {
    EEPROM_WriteByte( 0, i );
    EEPROM_ReadByte( 0, &Value );

    EEPROM_WriteByte( 1, i );
    EEPROM_ReadByte( 1, &Value );
  }
}
