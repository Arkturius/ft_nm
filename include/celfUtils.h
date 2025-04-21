/**
 *	celfUtils.h
 *
 *		CELF utils macros.
 */

#ifndef _CELF_UTILS_H
# define _CELF_UTILS_H

# include <stdint.h>

# define	UNUSED	__attribute__((unused))
# define	INLINE	static inline

# if !defined(CONCAT) || !defined(CONCAT_INNER) || !defined(CONCAT3) || !defined(CONCAT4)
#  undef	CONCAT_INNER
#  undef	CONCAT
#  undef	CONCAT3
#  undef	CONCAT4

#  define	CONCAT_INNER(a, b)	a##b
#  define	CONCAT(a, b)		CONCAT_INNER(a, b)
#  define	CONCAT3(a, b, c)	CONCAT(CONCAT(a, b), c)
#  define	CONCAT4(a, b, c, d)	CONCAT(CONCAT(a, b), CONCAT(c, d))
# endif	// CONCAT

# if defined(_CELF_LOGGING)
#  define	LOG(X, ...)		printf("LOG : " X "\n", ##__VA_ARGS__)
#  define	ERR(X, ...)		printf("ERR : " X "\n", ##__VA_ARGS__)
# else
#  define	LOG(X, ...)
#  define	ERR(X, ...)
# endif	// _CELF_LOGGING

INLINE	uint16_t	byteswap16(uint16_t x)
{
	return (x >> 8) | (x << 8);
}

INLINE	uint32_t	byteswap32(uint32_t x)
{
	return	((x & 0xFF000000U) >> 24)	|
			((x & 0x00FF0000U) >> 8)	|
			((x & 0x0000FF00U) << 8)	|
			((x & 0x000000FFU) << 24);
}

INLINE	uint64_t	byteswap64(uint64_t x)
{
    return ((x & 0xFF00000000000000ULL) >> 56) |
           ((x & 0x00FF000000000000ULL) >> 40) |
           ((x & 0x0000FF0000000000ULL) >> 24) |
           ((x & 0x000000FF00000000ULL) >> 8)  |
           ((x & 0x00000000FF000000ULL) << 8)  |
           ((x & 0x0000000000FF0000ULL) << 24) |
           ((x & 0x000000000000FF00ULL) << 40) |
           ((x & 0x00000000000000FFULL) << 56);
}

# if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#  define HOST_IS_LE 1
# else
#  define HOST_IS_BE 1
# endif

# if !defined(CELF_IS_LE)
#  define CELF_IS_LE	1
# endif

# if defined(CELF_IS_LE) && defined(HOST_IS_LE)
#  define		READ_FIELD_INNER(field, size)									\
    ((HOST_IS_LE != (CELF_IS_LE)) ? (											\
        (size) == 2 ? byteswap16(field) :										\
        (size) == 4 ? byteswap32(field) :										\
        (size) == 8 ? byteswap64(field) : (field)								\
    ) : (field))
#  define	READ_FIELD(field)	READ_FIELD_INNER(field, sizeof(field))
# endif

#endif	// _CELF_UTILS_H
