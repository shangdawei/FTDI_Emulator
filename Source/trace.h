#ifndef __TRACE_H__
#define __TRACE_H__

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define TRACE_DUMP_LINE_SIZE    ( 16 )
#define TRACE_DUMP_LINE_COUNT   (  8 )

#define TRACE_LEVEL_DEBUG       5
#define TRACE_LEVEL_INFO        4
#define TRACE_LEVEL_WARNING     3
#define TRACE_LEVEL_ERROR       2
#define TRACE_LEVEL_FATAL       1
#define TRACE_LEVEL_NO_TRACE    0

#define TRACE_LEVEL_DUMP        TRACE_LEVEL_INFO

/*By default, no trace */
#if !defined( TRACE_LEVEL )
#define TRACE_LEVEL             TRACE_LEVEL_NO_TRACE
#endif

/*By default, trace level is static (not dynamic)*/
#if !defined(DYN_TRACES)
#define DYN_TRACES              0
#endif

#if defined( DEBUG )
#undef TRACE_LEVEL
#define TRACE_LEVEL             TRACE_LEVEL_DEBUG
#endif

#undef NOTRACE
#if (DYN_TRACES==0)
#if (TRACE_LEVEL == TRACE_LEVEL_NO_TRACE)
#define                         NOTRACE
#endif
#endif

/*------------------------------------------------------------------------------
 *         Global Macros
 * ------------------------------------------------------------------------------
 */

/**
 *  Outputs a formatted string using 'printf' if the log level is high
 *  enough. Can be disabled by defining TRACE_LEVEL=0 during compilation.
 *   ...  Additional parameters depending on formatted string.
 */
#if defined(NOTRACE)

/*Empty macro*/
#define TRACE_STRING(str)     do {            } while(0)
#define TRACE_CHAR(ch)        do {            } while(0)
#define TRACE_DUMP(...)       do {            } while(0)

#define TRACE_DEBUG(...)      do {            } while(0)
#define TRACE_INFO(...)       do {            } while(0)
#define TRACE_WARNING(...)    do {            } while(0)
#define TRACE_ERROR(...)      do {            } while(0)
#define TRACE_DEBUG_WP(...)   do {            } while(0)
#define TRACE_INFO_WP(...)    do {            } while(0)
#define TRACE_WARNING_WP(...) do {            } while(0)
#define TRACE_ERROR_WP(...)   do {            } while(0)

#define TRACE_FATAL(...)      do { while(1);  } while(0)
#define TRACE_FATAL_WP(...)   do { while(1);  } while(0)

#elif (DYN_TRACES > 0)

/*Trace output depends on dwTraceLevel value*/
#define TRACE_STRING(str)     do { if (dwTraceLevel >= TRACE_LEVEL_DUMP)    { PutString(str);                                       } } while(0)
#define TRACE_CHAR(ch)        do { if (dwTraceLevel >= TRACE_LEVEL_DUMP)    { PutChar(ch);                                          } } while(0)
#define TRACE_DUMP(...)       do { if (dwTraceLevel >= TRACE_LEVEL_DUMP)    { TRACE_DumpBuffer( __VA_ARGS__ );                      } } while(0)

#define TRACE_DEBUG(...)      do { if (dwTraceLevel >= TRACE_LEVEL_DEBUG)   { printf("-D- " __VA_ARGS__); printf("\n\r");           } } while(0)
#define TRACE_INFO(...)       do { if (dwTraceLevel >= TRACE_LEVEL_INFO)    { printf("-I- " __VA_ARGS__); printf("\n\r");           } } while(0)
#define TRACE_WARNING(...)    do { if (dwTraceLevel >= TRACE_LEVEL_WARNING) { printf("-W- " __VA_ARGS__); printf("\n\r");           } } while(0)
#define TRACE_ERROR(...)      do { if (dwTraceLevel >= TRACE_LEVEL_ERROR)   { printf("-E- " __VA_ARGS__); printf("\n\r");           } } while(0)
#define TRACE_FATAL(...)      do { if (dwTraceLevel >= TRACE_LEVEL_FATAL)   { printf("-F- " __VA_ARGS__); printf("\n\r"); while(1); } } while(0)

#define TRACE_DEBUG_WP(...)   do { if (dwTraceLevel >= TRACE_LEVEL_DEBUG)   { printf(       __VA_ARGS__); printf("\n\r");            } } while(0)
#define TRACE_INFO_WP(...)    do { if (dwTraceLevel >= TRACE_LEVEL_INFO)    { printf(       __VA_ARGS__); printf("\n\r");            } } while(0)
#define TRACE_WARNING_WP(...) do { if (dwTraceLevel >= TRACE_LEVEL_WARNING) { printf(       __VA_ARGS__); printf("\n\r");            } } while(0)
#define TRACE_ERROR_WP(...)   do { if (dwTraceLevel >= TRACE_LEVEL_ERROR)   { printf(       __VA_ARGS__); printf("\n\r");            } } while(0)
#define TRACE_FATAL_WP(...)   do { if (dwTraceLevel >= TRACE_LEVEL_FATAL)   { printf(       __VA_ARGS__); printf("\n\r");  while(1); } } while(0)

#else

/*Trace compilation depends on TRACE_LEVEL value*/
#if (TRACE_LEVEL >= TRACE_LEVEL_DUMP)

#define TRACE_STRING(str)     do { PutString(str); } while(0)
#define TRACE_CHAR(ch)        do { PutChar(ch); } while(0)
#define TRACE_DUMP(...)       do { TRACE_DumpBuffer( __VA_ARGS__ ); } while(0)

#else

#define TRACE_STRING(str)     do { } while(0)
#define TRACE_CHAR(ch)        do { } while(0)
#define TRACE_DUMP(...)       do { } while(0)

#endif

#if (TRACE_LEVEL >= TRACE_LEVEL_DEBUG)
#define TRACE_DEBUG(...)      do { printf("-D- " __VA_ARGS__); printf("\n\r");} while(0)
#define TRACE_DEBUG_WP(...)   do { printf(__VA_ARGS__); printf("\n\r");} while(0)
#else
#define TRACE_DEBUG(...)      do { } while(0)
#define TRACE_DEBUG_WP(...)   do { } while(0)
#endif

#if (TRACE_LEVEL >= TRACE_LEVEL_INFO)
#define TRACE_INFO(...)       do { printf("-I- " __VA_ARGS__); printf("\n\r");} while(0)
#define TRACE_INFO_WP(...)    do { printf(__VA_ARGS__); printf("\n\r"); } while(0)
#else
#define TRACE_INFO(...)       do { } while(0)
#define TRACE_INFO_WP(...)    do { } while(0)
#endif

#if (TRACE_LEVEL >= TRACE_LEVEL_WARNING)
#define TRACE_WARNING(...)    do { printf("-W- " __VA_ARGS__); printf("\n\r");} while(0)
#define TRACE_WARNING_WP(...) do { printf(__VA_ARGS__); printf("\n\r"); } while(0)
#else
#define TRACE_WARNING(...)    do { } while(0)
#define TRACE_WARNING_WP(...) do { } while(0)
#endif

#if (TRACE_LEVEL >= TRACE_LEVEL_ERROR)
#define TRACE_ERROR(...)      do { printf("-E- " __VA_ARGS__); printf("\n\r");} while(0)
#define TRACE_ERROR_WP(...)   do { printf(__VA_ARGS__); printf("\n\r"); } while(0)
#else
#define TRACE_ERROR(...)      do { } while(0)
#define TRACE_ERROR_WP(...)   do { } while(0)
#endif

#if (TRACE_LEVEL >= TRACE_LEVEL_FATAL)
#define TRACE_FATAL(...)      do { printf("-F- " __VA_ARGS__); printf("\n\r"); while(1); } while(0)
#define TRACE_FATAL_WP(...)   do { printf(__VA_ARGS__); printf("\n\r"); while(1); } while(0)
#else
#define TRACE_FATAL(...)      do { while(1); } while(0)
#define TRACE_FATAL_WP(...)   do { while(1); } while(0)
#endif

#endif

#if ( DYN_TRACES > 0 )
extern uint32_t dwTraceLevel;
void TRACE_SetLevel( uint32_t Level );
#endif

void TRACE_DumpBuffer( void * buf, int32_t size, int32_t max_size );

#endif /* __TRACE_H__ */
