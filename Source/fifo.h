/*
 * http://en.wikipedia.org/wiki/Circular_buffer -- Mirroring
 *
 * A generic kernel FIFO implementation -- Linux kfifo.h
 *
 * For one reader and one writer there is no lock
 * For multiple writer and one reader there is only a need to lock the writer.
 * For one writer and multiple reader there is only a need to lock the reader.
 *
 */

#ifndef __FIFO_H__
#define __FIFO_H__

#include <stdint.h>

typedef struct TAG_FIFO
{
  uint32_t capacity;      // 2^n
  uint32_t writeIndex;    // Data will be written to
  uint32_t readIndex;     // Data will be read from
  uint8_t * memory;       // fifo memory
} FIFO;

void FIFO_Init( FIFO *fifo, uint32_t capacity, void *memory );

FIFO * FIFO_Create( uint32_t capacity );
void FIFO_Destroy( FIFO *fifo );

uint32_t FIFO_Write( FIFO *fifo, void * data, uint32_t count );
uint32_t FIFO_Read( FIFO *fifo, void * data, uint32_t count );

uint32_t FIFO_WriteFixed( FIFO *fifo, void * data, uint32_t elemCount, uint32_t elemSize );
uint32_t FIFO_ReadFixed( FIFO *fifo, void * data, uint32_t elemCount, uint32_t elemSize );

uint32_t FIFO_WriteVariable( FIFO *fifo, void * data, uint32_t size );
uint32_t FIFO_ReadVariable( FIFO *fifo, void * data );

uint32_t FIFO_IsEmpty( FIFO *fifo );
uint32_t FIFO_IsFull( FIFO *fifo );

uint32_t FIFO_Writable( FIFO *fifo, uint32_t * index );
uint32_t FIFO_Readable( FIFO *fifo, uint32_t * index );

// -1 : Flush All
void FIFO_Flush( FIFO *fifo, uint32_t count );
void FIFO_Update( FIFO *fifo, uint32_t count );

#endif
