#include "assert.h"
#include "trace.h"

void assert_failed( uint8_t* file, uint32_t line )
{
  TRACE_FATAL_WP( "Error in file %s on line %d\r\n", file, (int )line );
}
