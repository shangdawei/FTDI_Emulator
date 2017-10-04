#ifndef __ASSERT_H__
#define __ASSERT_H__

#include <stdio.h>
#include <stdint.h>

void assert_failed( uint8_t* file, uint32_t line );

#define __DO_NOTHING__  ( (void) 0 )

/*
 * brief  The assert_param macro is used for function's parameters check.
 * param  expr: If expr is false, it calls assert_failed function
 *   which reports the name of the source file and the source
 *   line number of the call that failed.
 *   If expr is true, it returns no value.
 * retval None
 */
#if !defined DEBUG
#define assert(expr)  __DO_NOTHING__
#else
#define assert(expr)  \
  ( (expr) ? __DO_NOTHING__ : assert_failed((uint8_t *)__FILE__, __LINE__) )
#endif

#endif /* __ASSERT_H__ */
