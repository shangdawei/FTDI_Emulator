/*
 * trace.c
 *
 *  Created on: 2012-10-25
 *      Author: Administrator
 */

#include "trace.h"
#include <stdio.h>

extern void PutChar( int32_t ch );
extern int32_t GetChar( void );

void PutString( int8_t * str )
{
  int8_t ch;
  while ( 1 )
  {
    ch = *str++;
    if ( 0 == ch )
      break;
    PutChar( (int32_t) ch );
  }
}

#define CHAR_TO_HEX_DIGIT(c)  \
  (((c) >= '0' && (c) <= '9') ? (c) - '0' : (c) - 'A' + 10)

#if ( DYN_TRACES > 0 )
uint32_t dwTraceLevel;
void TRACE_SetLevel( uint32_t Level)
{
  dwTraceLevel = Level;
}
#endif

// Retarget the C library function to the USART
// Required for printf()
int32_t fputc( int32_t ch, FILE * f )
{
  (void) ( f );
  PutChar( ch );
  return ch;
}

// Retarget the C library function to the USART
// Required for printf()
int32_t fputs( const char *pStr, FILE *f )
{
  int32_t num = 0;
  while ( *pStr != 0 )
  {
    fputc( *pStr, f );
    num++;
    pStr++;
  }
  return num;
}

int32_t fgetc( FILE * f )
{
  (void) ( f );
  return GetChar( );
}

const uint8_t HexCharacterArray[ ] =
{
  '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

static void TRACE_Byte2Hex( uint8_t toPrintf, uint8_t * hex )
{
  int32_t ToPrintf;

  ToPrintf = toPrintf;
  toPrintf = ( toPrintf >> 4 ) & 0x0F;
  hex[ 0 ] = HexCharacterArray[ toPrintf ];

  toPrintf = ToPrintf & 0x0F;
  hex[ 1 ] = HexCharacterArray[ toPrintf ];

  hex[ 2 ] = 0x20;
}

// 01 23 45 67 89 AB CD EF 01 23 45 67 89 AB CD EF ................
// 01 23 45 67                                     ....
//
void TRACE_DumpBuffer( void * buf, int32_t size, int32_t max_size )
{
  uint32_t count_of_line;
  if ( ( max_size > 0 ) && ( size > max_size ) )
    size = max_size;

  count_of_line = ( size + TRACE_DUMP_LINE_SIZE - 1 ) / TRACE_DUMP_LINE_SIZE;

  if ( count_of_line > TRACE_DUMP_LINE_COUNT )
    count_of_line = TRACE_DUMP_LINE_COUNT;

  uint8_t hex_buf[ ( TRACE_DUMP_LINE_SIZE ) * 4 ];
  uint8_t * buf_ptr = (uint8_t*) buf;

  for ( uint32_t count = 0; count < count_of_line; count++ )
  {
    uint32_t byte_count_of_line = size;
    if ( byte_count_of_line > TRACE_DUMP_LINE_SIZE )
      byte_count_of_line = TRACE_DUMP_LINE_SIZE;

    memset( hex_buf, 0x20, sizeof( hex_buf ) );

    uint8_t * byte_ptr = (uint8_t*) buf_ptr;

    for ( uint32_t byte_count = 0; byte_count < byte_count_of_line; byte_count++ )
      TRACE_Byte2Hex( byte_ptr[ byte_count ], &hex_buf[ byte_count * 3 ] );

    for ( uint32_t byte_count = 0; byte_count < byte_count_of_line; byte_count++ )
    {
      uint8_t ch = byte_ptr[ byte_count ];
      if ( ( ch < 0x20 ) || ( ch > 0x7F ) )
        ch = '.';
      hex_buf[ TRACE_DUMP_LINE_SIZE * 3 + byte_count ] = ch;
    }

    hex_buf[ TRACE_DUMP_LINE_SIZE * 3 + byte_count_of_line ] = 0x00;

    size -= byte_count_of_line;
    buf_ptr += byte_count_of_line;

    printf( (char const *) hex_buf );
    printf( "\n\r" );
  }
}
