#include <stdlib.h>
#include "fifo.h"
#include "utility.h"
#include "assert.h"
#include "macro_misc.h"

static uint32_t FIFO_ReadableSize( FIFO *fifo )
{
  return fifo->writeIndex - fifo->readIndex;
}

static uint32_t FIFO_WritableSize( FIFO *fifo )
{
  return ( fifo->capacity ) - ( fifo->writeIndex - fifo->readIndex );
}

static void FIFO_DoWrite( FIFO *fifo, void * data, uint32_t count )
{
  uint32_t writeIndex = fifo->writeIndex & ( fifo->capacity - 1 );
  uint32_t upper = MIN( count, fifo->capacity - writeIndex );
  uint32_t lower = count - upper;
  memcpy( fifo->memory + writeIndex, data, upper );
  if ( lower )
    memcpy( fifo->memory, (uint8_t*) data + upper, lower );
  fifo->writeIndex += count;
#if ( FIFO_DEBUG > 0 )
#endif
}

static void FIFO_DoRead( FIFO *fifo, void * data, uint32_t count )
{
  uint32_t readIndex = fifo->readIndex & ( fifo->capacity - 1 );
  uint32_t upper = MIN( count, fifo->capacity - readIndex );
  uint32_t lower = count - upper;

  memcpy( data, fifo->memory + readIndex, upper );
  if ( lower )
    memcpy( (uint8_t*) data + upper, fifo->memory, lower );

  fifo->readIndex += count;
}

void FIFO_Init( FIFO *fifo, uint32_t capacity, void *memory )
{
  if ( IsPower2( capacity ) == 0 )
    capacity = RoundDownPower2( capacity );

  fifo->capacity = capacity;
  fifo->writeIndex = 0;
  fifo->readIndex = 0;
  fifo->memory = (uint8_t *) memory;
}

FIFO * FIFO_Create( uint32_t capacity )
{
  if ( IsPower2( capacity ) == 0 )
    capacity = RoundUpPower2( capacity );

  uint32_t size = ALIGN_UP( sizeof(FIFO), 4 );
  FIFO * fifo = (FIFO *) malloc( size + capacity );
  if ( fifo )
  {
    uint8_t * memoy = (uint8_t *) ( (uint32_t) fifo + size );

    FIFO_Init( fifo, capacity, memoy );
  }
  return fifo;
}

void FIFO_Destroy( FIFO *fifo )
{
  free( fifo );
}

uint32_t FIFO_IsEmpty( FIFO *fifo )
{
  return fifo->writeIndex == fifo->readIndex;
}

uint32_t FIFO_IsFull( FIFO *fifo )
{
  return fifo->readIndex + fifo->capacity == fifo->writeIndex;
}

// |<--memory  |<--readIndex0   |<--readIndex1
// |<--Space-->|<--continuous-->|<--Space-->|
// |<-----------------capacity------------->|<-----------------capacity------------->|
// |<--memory              |<--readIndex0   |<--readIndex1                           |
// |<--Lower-->|<--Space-->|<-----Upper---->|<--Lower-->|<--Space-->|<-----Upper---->|
// |                       |<--continuous-->|           |
//                         |<----------readable-------->|
uint32_t FIFO_Readable( FIFO *fifo, uint32_t * index )
{
  uint32_t readable = FIFO_ReadableSize( fifo );
  uint32_t readIndex = fifo->readIndex & ( fifo->capacity - 1 );
  uint32_t continuous = fifo->capacity - readIndex;
  *index = readIndex;

  if ( continuous < readable )  // Wrap around
    return continuous;
  else
    return readable;
}

// count of continuous memory
void FIFO_Flush( FIFO *fifo, uint32_t count )
{
  fifo->readIndex += count;
}

uint32_t FIFO_Writable( FIFO *fifo, uint32_t * index )
{
  uint32_t writable = FIFO_WritableSize( fifo );
  uint32_t writeIndex = fifo->writeIndex & ( fifo->capacity - 1 );
  uint32_t continuous = fifo->capacity - writeIndex;
  *index = writeIndex;
  return ( continuous < writable ) ? continuous : writable;
}

void FIFO_Update( FIFO *fifo, uint32_t count )
{
  fifo->writeIndex += count;
}

// return byte count has been written
uint32_t FIFO_Write( FIFO *fifo, void * data, uint32_t count )
{
  uint32_t writtenCount = MIN( count, FIFO_WritableSize( fifo ) );
  FIFO_DoWrite( fifo, data, writtenCount );
  return writtenCount;
}

// return byte count has been read
uint32_t FIFO_Read( FIFO *fifo, void * data, uint32_t count )
{
  uint32_t readCount = MIN( count, FIFO_ReadableSize( fifo ) );
  FIFO_DoRead( fifo, data, readCount );
  return readCount;
}

// return element count has been written
uint32_t FIFO_WriteFixed( FIFO *fifo, void * data, uint32_t elemCount, uint32_t elemSize )
{
  uint32_t writtenCount = MIN( elemCount, FIFO_WritableSize( fifo ) / elemSize );
  uint32_t size = elemSize * writtenCount;
  FIFO_DoWrite( fifo, data, size );
  return elemCount;
}

// return element count has been read
uint32_t FIFO_ReadFixed( FIFO *fifo, void * data, uint32_t elemCount, uint32_t elemSize )
{
  uint32_t readCount = MIN( elemCount, FIFO_ReadableSize( fifo ) / elemSize );
  uint32_t size = elemSize * readCount;
  FIFO_DoRead( fifo, data, size );
  return elemCount;
}

// return byte count has been written
uint32_t FIFO_WriteVariable( FIFO *fifo, void * data, uint32_t size )
{
  if ( FIFO_WritableSize( fifo ) < size + 4 )
    return 0;

  FIFO_DoWrite( fifo, &size, 4 );
  FIFO_DoWrite( fifo, data, size );
  return size;
}

// return byte count has been read
uint32_t FIFO_ReadVariable( FIFO *fifo, void * data )
{
  if ( FIFO_ReadableSize( fifo ) <= 4 )
    return 0;

  uint32_t size;
  FIFO_DoRead( fifo, &size, 4 );
  FIFO_DoRead( fifo, data, size );
  return size;
}
