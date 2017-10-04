#ifndef __MISC_MACRO_H__
#define __MISC_MACRO_H__

#include <stdint.h>

/* Macro for getting minimum value. */
#define MIN(a, b)                             \
  ((a) < (b) ? (a) : (b))

/* Macro for getting maximum value. */
#define MAX(a, b)                             \
  ((a) > (b) ? (a) : (b))

#define ABS(a)                                \
  ( (a) < 0 ? -(a) : (a) )

//
// Number round up/down
//
// This macro truncates number to the align boundary.
#define TRUNC(number, align)                    \
  ( ((uint32_t)(number)) & (~((align)-1)) )

// This macro rounds down number to the align boundary.
#define ROUND_DOWN(number,align) ((number) & ~((align)-1))
// ((uint32_t)(number) - ( (uint32_t)(number) % (uint32_t)(align) ))

// This macro rounds up number to the align boundary.
#define ROUND_UP(number,align) (((number)+(align)-1) & ~((align)-1))
// ROUND_DOWN((uint32_t)(number) + (uint32_t)(align) - 1, (align))
//
// Address alignment up/down
//
// This macro rounds down addr to the align boundary.
#define ALIGN_DOWN(addr, align)               \
  ( ((uint32_t)(addr)) & (~( (align)-1) ) )

// This macro rounds up addr to the align boundary.
#define ALIGN_UP(addr, align)                 \
  ( ((uint32_t)(addr)) & ( (align) - 1 ) ) ?  \
  ( ( ((uint32_t)(addr)) + (align) - 1) &     \
  (~( (align) - 1) ) ) : ( (uint32_t)(addr) )

/* Macros for handling packed structs. */
#define STRINGIZE(X)  #X


#endif /* __MISC_MACRO_H__ */

