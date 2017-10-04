#ifndef __UTILITY_H__
#define __UTILITY_H__

#include <stdint.h>
#include <string.h>

/* The following definitions make the code more readable. */
#define BITMASK(X)        (1L << (X))
#define MASK32            0xFFFFFFFFL

/*
 * LOG2 - log of base 2 of 32-bit unsigned value
 * constant-capable log of base 2 calculation
 * the result is 0 when n == 0
 */
#define LOG2(n)           ( \
  (n) & (1ULL << 31) ? 31 : \
  (n) & (1ULL << 30) ? 30 : \
  (n) & (1ULL << 29) ? 29 : \
  (n) & (1ULL << 28) ? 28 : \
  (n) & (1ULL << 27) ? 27 : \
  (n) & (1ULL << 26) ? 26 : \
  (n) & (1ULL << 25) ? 25 : \
  (n) & (1ULL << 24) ? 24 : \
  (n) & (1ULL << 23) ? 23 : \
  (n) & (1ULL << 22) ? 22 : \
  (n) & (1ULL << 21) ? 21 : \
  (n) & (1ULL << 20) ? 20 : \
  (n) & (1ULL << 19) ? 19 : \
  (n) & (1ULL << 18) ? 18 : \
  (n) & (1ULL << 17) ? 17 : \
  (n) & (1ULL << 16) ? 16 : \
  (n) & (1ULL << 15) ? 15 : \
  (n) & (1ULL << 14) ? 14 : \
  (n) & (1ULL << 13) ? 13 : \
  (n) & (1ULL << 12) ? 12 : \
  (n) & (1ULL << 11) ? 11 : \
  (n) & (1ULL << 10) ? 10 : \
  (n) & (1ULL <<  9) ?  9 : \
  (n) & (1ULL <<  8) ?  8 : \
  (n) & (1ULL <<  7) ?  7 : \
  (n) & (1ULL <<  6) ?  6 : \
  (n) & (1ULL <<  5) ?  5 : \
  (n) & (1ULL <<  4) ?  4 : \
  (n) & (1ULL <<  3) ?  3 : \
  (n) & (1ULL <<  2) ?  2 : \
  (n) & (1ULL <<  1) ?  1 : \
  (n) & (1ULL <<  0) ?  0 : 0 )

// round the given value up to the nearest power of two
#define ROUNDUP_POWER2(n)   \
  ( (n) == 0 ) ? 0 : ( 1UL << ( LOG2( (n) - 1 ) + 1 ) )

#define IS_POWER2( x )      \
  ( (x) != 0 && ( ((x) & ( (x) - 1) ) == 0))

/*
 * ORDER_BASE2 - calculate the (rounded up) base 2 order of the argument
 *
 * The first few values calculated by this routine:
 *  ob2(0) = 0
 *  ob2(1) = 0
 *  ob2(2) = 1
 *  ob2(3) = 2
 *  ob2(4) = 2
 *  ob2(5) = 3
 *  ... and so on.
 */
#define ORDER_BASE2(n) LOG2(ROUNDUP_POWER2(n))

extern const uint8_t RevBitOrderByteTable[ 256 ];

uint8_t RevBitOrderByte( uint8_t InByte );
uint16_t RevBitOrderWord( uint16_t InWord );
uint32_t RevBitOrderDWord( uint32_t InDWord );

uint32_t RevBitOrder( uint32_t InDWord, uint32_t BitCount );

uint16_t RevByteOrderWord( uint16_t InWord );
uint32_t RevByteOrderDWord( uint32_t InDWord );

void RevByteOrderDWordBuffer( uint32_t * Dest, uint32_t * Source, uint32_t DWordCount );
void XorDWordBuffer( uint32_t * Dest, uint32_t * Source, uint32_t DWordCount );
void CopyDWordBuffer( uint32_t * Dest, uint32_t * Source, uint32_t DWordCount );

uint32_t CountLeadingZero( uint32_t InDWord );

int32_t Hex2Bin( uint8_t * pHexString, uint8_t * pBinArray );

void Bit64ToByte8( uint8_t * pBit64, uint8_t * pByte8 );
uint8_t Byte8ToBit64( uint8_t * pByte8, uint8_t * pBit64 );

uint32_t WidthMask( uint32_t Width );
uint32_t RoundUpPower2( uint32_t n );
uint32_t RoundDownPower2( uint32_t n );
uint32_t IsPower2( uint32_t n );
uint32_t Log2( uint32_t v );

#endif /* __UTILITY_H__ */
