#include "utility.h"

#if ( __ARM_PROFILE_M__ > 0 )
#if ( __CORE__ >= __ARM6M__ )
#endif
#endif

const uint8_t RevBitOrderByteTable[ 256 ] =
{
  0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0, 0x08, 0x88, 0x48,
  0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8, 0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4,
  0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4, 0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C,
  0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC, 0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2,
  0x32, 0xB2, 0x72, 0xF2, 0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A,
  0xFA, 0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6, 0x0E, 0x8E,
  0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE, 0x01, 0x81, 0x41, 0xC1, 0x21,
  0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1, 0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9,
  0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9, 0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55,
  0xD5, 0x35, 0xB5, 0x75, 0xF5, 0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD,
  0x7D, 0xFD, 0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3, 0x0B,
  0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB, 0x07, 0x87, 0x47, 0xC7,
  0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7, 0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F,
  0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF, };

// Rev byte order in each Word independently
// converts 16-bit big-endian data into little-endian data
// or 16-bit little-endian data into big-endian data
//
// REV16
// This ARM instruction is available in ARMv6 and above.
// This 16-bit Thumb instruction is available in ARMv6 and above.
// This 32-bit Thumb instruction is available in ARMv6T2 and above.
//
uint16_t RevByteOrderWord( uint16_t InWord )
{
#if ( __ARM_PROFILE_M__ > 0 )
#pragma diag_suppress=Pe940
  asm("rev16 r0, r0");
#else
  // This should generate the REV16 instruction.
  return ( ( InWord & 0x00ff ) << 8 ) | ( ( InWord & 0xff00 ) >> 8 );
#endif
}
#pragma diag_warning=Pe940

// Rev byte order in a DWord
// converts 32-bit big-endian data into little-endian data
// or 32-bit little-endian data into big-endian data.
//
// REV
//
// This ARM instruction is available in ARMv6 and above.
// This 16-bit Thumb instruction is available in ARMv6 and above.
// This 32-bit Thumb instruction is available in ARMv6T2 and above.
//
#pragma diag_suppress=Pe940
#if ( __ARM_PROFILE_M__ == 0 )
__arm
#endif
uint32_t RevByteOrderDWord( uint32_t InDWord )
{
#if ( __ARM_PROFILE_M__ > 0 )
  asm("rev r0, r0");
#elif (0)
  // This should generate the REV instruction.
  return ( InDWord & 0x000000FFU ) << 24 | ( InDWord & 0x0000FF00U ) << 8
  | ( InDWord & 0x00FF0000U ) >> 8 | ( InDWord & 0xFF000000U ) >> 24;
#else
  //------------------------------------------------------------------------------
  // R0 = A     B     C     D
  //------------------------------------------------------------------------------
  // XOR  C     D     A     B               R0, ROR #16
  // R1 = A^C   B^D   C^A   D^B     EOR     R1, R0
  // R1 = A^C   00    C^A   D^B     BIC     R1, R1, #0x00FF0000
  // R0 = D     A     B     C       MOV     R0, R0, ROR #8
  // R1 = 00    A^C   00    C^A             R1, LSR #8
  //------------------------------------------------------------------------------
  // R0 = D     C     B     A               EOR     R0, R0, R1, LSR #8
  //------------------------------------------------------------------------------
  //
  // R1 = A^C, B^D, C^A, D^B
  asm("EOR     R1, R0, R0, ROR #16");
  // R1 = A^C, 00,  C^A, D^B
  asm("BIC     R1, R1, #0x00FF0000");
  // R0 = D A B C
  asm("MOV     R0, R0, ROR #8");
  // R0 = D C B A
  asm("EOR     R0, R0, R1, LSR #8");
#endif
}
#pragma diag_warning=Pe940

#if ( __ARM_PROFILE_M__ == 0 )
__arm
#endif
void RevByteOrderDWordBuffer( uint32_t * Dest, uint32_t * Source, uint32_t DWordCount )
{
#if ( __ARM_PROFILE_M__ > 0 )
  asm volatile(
      "REV_BUF_TEST:            \n"
      "CMP     R2, #0           \n"
      "BNE.N   REV_BUF_LOOP     \n"
      "BX      LR               \n"
      "REV_BUF_LOOP:            \n"
      "LDR     R3, [R1], #4     \n"
      "REV     R3, R3           \n"
      "STR     R3, [R0], #4     \n"
      "SUB     R2, R2, #1       \n"
      "BNE.N   REV_BUF_TEST     \n");
#else
  asm volatile(
      "PUSH    {R4, LR}             \n"
      "REV_BUF_TEST:                \n"
      "CMP     R2, #0               \n"
      "BNE     REV_BUF_LOOP         \n"
      "POP     {R4, PC}             \n"
      "REV_BUF_LOOP:                \n"
      "LDR     R3, [R1], #4         \n"
      "EOR     R4, R3, R3, ROR #16  \n"
      "BIC     R4, R4, #0x00FF0000  \n"
      "MOV     R3, R3, ROR #8       \n"
      "EOR     R3, R3, R4, LSR #8   \n"
      "STR     R3, [R0], #4         \n"
      "SUB     R2, R2, #1           \n"
      "BNE     REV_BUF_TEST         \n");
#endif
}

#if ( __ARM_PROFILE_M__ == 0 )
__arm
#endif
void XorDWordBuffer( uint32_t * Dest, uint32_t * Source, uint32_t DWordCount )
{
#if ( __ARM_PROFILE_M__ > 0 )
  asm volatile(
      "PUSH    {R4, LR}             \n"
      "XOR_BUF_TEST:                \n"
      "CMP     R2, #0               \n"
      "BNE.N   XOR_BUF_LOOP         \n"
      "POP     {R4, PC}             \n"
      "XOR_BUF_LOOP:                \n"
      "LDR     R3, [R1], #4         \n"
      "LDR     R4, [R0]             \n"
      "EOR     R3, R3, R4           \n"
      "STR     R3, [R0], #4         \n"
      "SUB     R2, R2, #1           \n"
      "BNE.N   XOR_BUF_TEST         \n");
#else
  asm volatile(
      "PUSH    {R4, LR}             \n"
      "XOR_BUF_TEST:                \n"
      "CMP     R2, #0               \n"
      "BNE     XOR_BUF_LOOP         \n"
      "POP     {R4, PC}             \n"
      "XOR_BUF_LOOP:                \n"
      "LDR     R3, [R1], #4         \n"
      "LDR     R4, [R0]             \n"
      "EOR     R3, R3, R4           \n"
      "STR     R3, [R0], #4         \n"
      "SUB     R2, R2, #1           \n"
      "BNE     XOR_BUF_TEST         \n");
#endif
}

#if ( __ARM_PROFILE_M__ == 0 )
__arm
#endif
void CopyDWordBuffer( uint32_t * Dest, uint32_t * Source, uint32_t DWordCount )
{
#if ( __ARM_PROFILE_M__ > 0 )
  asm volatile(
      "CPY_BUF_TEST:                \n"
      "CMP     R2, #0               \n"
      "BNE.N   CPY_BUF_LOOP         \n"
      "BX      LR                   \n"
      "CPY_BUF_LOOP:                \n"
      "LDR     R3, [R1], #4         \n"
      "STR     R3, [R0], #4         \n"
      "SUB     R2, R2, #1           \n"
      "BNE.N   CPY_BUF_TEST         \n");
#else
  asm volatile(
      "CPY_BUF_TEST:                \n"
      "CMP     R2, #0               \n"
      "BNE     CPY_BUF_LOOP         \n"
      "BX      LR                   \n"
      "CPY_BUF_LOOP:                \n"
      "LDR     R3, [R1], #4         \n"
      "STR     R3, [R0], #4         \n"
      "SUB     R2, R2, #1           \n"
      "BNE     CPY_BUF_TEST         \n");
#endif
}

// Rev the bit order in a 32-bit word.
//
// RBIT
//
// This ARM instruction is available in ARMv6T2 and above.
// This 16-bit Thumb instruction is not available.
// This 32-bit Thumb instruction is available in ARMv6T2 and above.
//
uint32_t RevBitOrderDWord( uint32_t InDWord )
{
#if ( __ARM_PROFILE_M__ > 0 )
#pragma diag_suppress=Pe940
  asm("rbit r0, r0");
#elif (0)
  InDWord = ( ( InDWord & 0x55555555 ) << 1 )
  | ( ( InDWord >> 1 ) & 0x55555555 );

  InDWord = ( ( InDWord & 0x33333333 ) << 2 )
  | ( ( InDWord >> 2 ) & 0x33333333 );

  InDWord = ( ( InDWord & 0x0F0F0F0F ) << 4 )
  | ( ( InDWord >> 4 ) & 0x0F0F0F0F );

  InDWord = ( InDWord << 24 ) | ( ( InDWord & 0xFF00 ) << 8 )
  | ( ( InDWord >> 8 ) & 0xFF00 ) | ( InDWord >> 24 );

  return InDWord;
#else
  InDWord = ( InDWord >> 16 ) | ( InDWord << 16 );
  InDWord = ( InDWord >> 8 & 0x00ff00ff ) | ( InDWord << 8 & 0xff00ff00 );
  InDWord = ( InDWord >> 4 & 0x0f0f0f0f ) | ( InDWord << 4 & 0xf0f0f0f0 );
  InDWord = ( InDWord >> 2 & 0x33333333 ) | ( InDWord << 2 & 0xcccccccc );
  InDWord = ( InDWord >> 1 & 0x55555555 ) | ( InDWord << 1 & 0xaaaaaaaa );
  return InDWord;
#endif
}
#pragma diag_warning=Pe940

// Rev the bit order in a 32-bit word.
uint16_t RevBitOrderWord( uint16_t InWord )
{
#if ( __ARM_PROFILE_M__ > 0 )
#pragma diag_suppress=Pe940
  asm("rbit r0, r0");
  asm("mov  r0, r0, LSR #16");
#else
  return ( RevBitOrderByteTable[ InWord & 0xFF ] << 8 )
  | ( RevBitOrderByteTable[ InWord >> 8 ] );
#endif
}
#pragma diag_warning=Pe940

// Rev the bit order in a byte.
uint8_t RevBitOrderByte( uint8_t InByte )
{
#if ( __ARM_PROFILE_M__ > 0 )
#pragma diag_suppress=Pe940
  asm("rbit r0, r0");
  asm("mov  r0, r0, LSR #24");
#else
  return RevBitOrderByteTable[ InByte ];
#endif
}
#pragma diag_warning=Pe940

// Rev bit order of value
// Returns the value v with the bottom b [0,32] bits reflected.
// Example: RevBitOrder(0x3e23L, 3) == 0x3e26
// Example: RevBitOrder(0x0001L,16) == 0x8000
uint32_t RevBitOrder( uint32_t InDWord, uint32_t BitCount )
{
#if ( __ARM_PROFILE_M__ > 0 )
#pragma diag_suppress=Pe940
  // UVWXYZ00000000000000
  asm("rsb r2, r1, #32");
  asm("mov r3, r0, LSL r2");
  // 00000000000000ZYXWUV
  asm("rbit r3, r3");
  // ABCDE......FGH000000
  asm("mov r0, r0, LSR r1");
  asm("mov r0, r0, LSL r1");
  // ABCDE......FGHZYXWUV
  asm("orr r0, r0, r3");
#else
  int32_t i;
  uint32_t InDWordValue = InDWord;
  for ( i = 0; i < BitCount; i++ )
  {
    if ( InDWordValue & 1L )
    InDWord |= BITMASK( ( BitCount - 1 ) - i );
    else
    InDWord &= ~BITMASK( ( BitCount - 1 ) - i );
    InDWordValue >>= 1;
  }
  return InDWord;
#endif
}
#pragma diag_warning=Pe940

// CLZ
//
// This ARM instruction is available in ARMv5T and above.
// This 16-bit Thumb instruction is not available.
// This 32-bit Thumb instruction is available in ARMv6T2 and above.
//
const char debruijn32[ 32 ] =
{
  0, 31, 9, 30, 3, 8, 13, 29, 2, 5, 7, 21, 12, 24, 28, 19, 1, 10, 4, 14, 6, 22, 25, 20, 11, 15, 23, 26, 16, 27, 17, 18 };

uint32_t CountLeadingZero( uint32_t InDWord )
{
#if ( __ARM_PROFILE_M__ > 0 )
#pragma diag_suppress=Pe940
  asm("clz r0, r0");
#elif ( 0 )
  if ( InDWord == 0 )
  return 32;
  InDWord |= InDWord >> 1;
  InDWord |= InDWord >> 2;
  InDWord |= InDWord >> 4;
  InDWord |= InDWord >> 8;
  InDWord |= InDWord >> 16;
  InDWord++;
  return debruijn32[ InDWord * 0x076be629 >> 27 ];
#else
  uint32_t Count = 0;
  int32_t AInteger = (int32_t) InDWord;
  while ( AInteger > 0 )
  {
    AInteger <<= 1;
    Count++;
  }
  return Count;
#endif
}
#pragma diag_warning=Pe940

// ROR
//
// This ARM instruction is available in all architectures.
// This 16-bit Thumb instruction is available in ARMv4T and above.
// This 32-bit Thumb instruction is available in ARMv6T2 and above.
uint32_t RotateRight( uint32_t InDWord, uint32_t Times )
{
#if ( __ARM_PROFILE_M__ > 0 )
#pragma diag_suppress=Pe940
#else
#pragma diag_suppress=Pe940
#endif
}
#pragma diag_warning=Pe940

// RRX
//
// This ARM instruction is available in all architectures
// This 16-bit Thumb instruction is not available.
// This 32-bit Thumb instruction is available in ARMv6T2 and above.
uint32_t RotateRightExtend( uint32_t InDWord, uint32_t Times )
{
#if ( __ARM_PROFILE_M__ > 0 )
#pragma diag_suppress=Pe940
#else
#pragma diag_suppress=Pe940
#endif
}
#pragma diag_warning=Pe940

int32_t Hex2Bin( uint8_t * pHexString, uint8_t * pBinArray )
{
  int32_t o = 0;
  int32_t i = 0;

  while ( pHexString[ i ] != 0x00 )
  {
    switch ( pHexString[ i ] )
    {
      case '0':
        pBinArray[ o ] = 0x00;
        break;
      case '1':
        pBinArray[ o ] = 0x10;
        break;
      case '2':
        pBinArray[ o ] = 0x20;
        break;
      case '3':
        pBinArray[ o ] = 0x30;
        break;
      case '4':
        pBinArray[ o ] = 0x40;
        break;
      case '5':
        pBinArray[ o ] = 0x50;
        break;
      case '6':
        pBinArray[ o ] = 0x60;
        break;
      case '7':
        pBinArray[ o ] = 0x70;
        break;
      case '8':
        pBinArray[ o ] = 0x80;
        break;
      case '9':
        pBinArray[ o ] = 0x90;
        break;
      case 'A':
      case 'a':
        pBinArray[ o ] = 0xa0;
        break;
      case 'B':
      case 'b':
        pBinArray[ o ] = 0xb0;
        break;
      case 'C':
      case 'c':
        pBinArray[ o ] = 0xc0;
        break;
      case 'D':
      case 'd':
        pBinArray[ o ] = 0xd0;
        break;
      case 'E':
      case 'e':
        pBinArray[ o ] = 0xe0;
        break;
      case 'F':
      case 'f':
        pBinArray[ o ] = 0xf0;
        break;
      default:
        return -1;
    }

    if ( pHexString[ i + 1 ] == 0x00 )
      return -1;

    switch ( pHexString[ i + 1 ] )
    {
      case '0':
        pBinArray[ o ] |= 0x00;
        break;
      case '1':
        pBinArray[ o ] |= 0x01;
        break;
      case '2':
        pBinArray[ o ] |= 0x02;
        break;
      case '3':
        pBinArray[ o ] |= 0x03;
        break;
      case '4':
        pBinArray[ o ] |= 0x04;
        break;
      case '5':
        pBinArray[ o ] |= 0x05;
        break;
      case '6':
        pBinArray[ o ] |= 0x06;
        break;
      case '7':
        pBinArray[ o ] |= 0x07;
        break;
      case '8':
        pBinArray[ o ] |= 0x08;
        break;
      case '9':
        pBinArray[ o ] |= 0x09;
        break;
      case 'A':
      case 'a':
        pBinArray[ o ] |= 0x0a;
        break;
      case 'B':
      case 'b':
        pBinArray[ o ] |= 0x0b;
        break;
      case 'C':
      case 'c':
        pBinArray[ o ] |= 0x0c;
        break;
      case 'D':
      case 'd':
        pBinArray[ o ] |= 0x0d;
        break;
      case 'E':
      case 'e':
        pBinArray[ o ] |= 0x0e;
        break;
      case 'F':
      case 'f':
        pBinArray[ o ] |= 0x0f;
        break;
      default:
        return -1;
    }

    i += 2;
    o++;
  }

  return o;
}

//将长度为8的字符串转为二进制位串
uint8_t Byte8ToBit64( uint8_t * pByte8, uint8_t * pBit64 )
{
  uint8_t cnt, cnt1;
  for ( cnt = 0; cnt < 8; cnt++ )
  {

    for ( cnt1 = 0; cnt1 < 8; cnt1++ )
    {
      if ( ( pByte8[ cnt ] & 0x80 ) == 0x80 )
        *pBit64 = 0x01;
      else
        *pBit64 = 0x00;
      pBit64++;
      pByte8[ cnt ] = pByte8[ cnt ] << 1;
    }
  }
  return 1;
}

//将二进制位串转为长度为8的字符串
void Bit64ToByte8( uint8_t * pBit64, uint8_t * pByte8 )
{
  uint8_t cnt, cnt1;
  memset( pByte8, 0, 8 );
  for ( cnt = 0; cnt < 8; cnt++ )
  {
    for ( cnt1 = 0; cnt1 < 8; cnt1++ )
    {
      pByte8[ cnt ] = pByte8[ cnt ] << 1;
      pByte8[ cnt ] |= *pBit64;
      pBit64++;
    }
  }

}

/**
 * Log2 - log of base 2 of 32-bit unsigned value
 */
uint32_t Log2( uint32_t v )
{
  uint32_t l = 0;
  while ( ( 1UL << l ) < v )
    l++;
  return l;
}

/*
 * Determine whether some value is a power of two,
 * where zero is *not* considered a power of two.
 */
uint32_t IsPower2( uint32_t n )
{
  return ( n != 0 && ( ( n & ( n - 1 ) ) == 0 ) );
}

/*
 * round the given value up to the nearest power of two
 * the result is undefined when n == 0
 */
uint32_t RoundUpPower2( uint32_t n )
{
  if ( n <= 1 )
    return n;

  if ( n & ( n - 1 ) == 0 )
    return n;

  return 1UL << ( Log2( n ) + 1 );
}

/*
 * round the given value down to nearest power of two
 * the result is undefined when n == 0
 */
uint32_t RoundDownPower2( uint32_t n )
{
  if ( n == 0 )
    return n;

  return 1UL << Log2( n );
}

// Returns a longword whose value is (2^Width)-1.
// The trick is to do this portably (e.g. without doing <<32).
uint32_t WidthMask( uint32_t Width )
{
  return ( ( ( 1L << ( Width - 1 ) ) - 1L ) << 1 ) | 1L;
}
