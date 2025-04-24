/**
 *	celfUtils.h | part of celf.h
 *
 *		CELF utils macros.
 */

#ifndef _CELF_UTILS_H
# define _CELF_UTILS_H

# include <unistd.h>
# include <stdint.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/mman.h>

# define	UNUSED		__attribute__((unused))
# define	INLINE		static inline
# define	DESTRUCTOR	__attribute__((destructor))

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

typedef struct
{
	uint8_t		*raw;
	uint32_t	size;
}	ELF;

typedef struct
{
	ELF		file;
	uint8_t	*f_header;
	uint8_t	*p_headers;
	uint8_t	*s_headers;
	uint8_t	*symbols;
	uint8_t	*reloc;			// not used now
	uint8_t	*reloca;		// not used now
	uint8_t	*dynamic;		// not used now
	uint8_t	*section_names;
	uint8_t	*shstrtab;
	uint8_t	*asst;
	void	*temp;
}	CELF_ctx;

# define	ELF_CONTEXT	__celf_ctx
# define	ELF_RAW		ELF_CONTEXT.file.raw
# define	ELF_SIZE	ELF_CONTEXT.file.size

static CELF_ctx	ELF_CONTEXT = {0};

# if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#  define HOST_IS_LE 1
# else
#  define HOST_IS_BE 1
# endif

# if !defined(CELF_IS_LE)
#  define CELF_IS_LE	ELF_RAW[EI_DATA] == ELF_DATA2LSB
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

INLINE int	ELF_open(const char *fp)
{
	int			fd;
	struct stat	st;

	fd = open(fp, O_RDONLY);
	if (fd == -1)
		return (1);
	if (fstat(fd, &st) == -1)
		return (1);

	ELF_SIZE = st.st_size;
	ELF_RAW = mmap(NULL, ELF_SIZE, PROT_READ, MAP_PRIVATE, fd, 0);
	close(fd);
	if (ELF_RAW == (void *)-1)
		return (1);

	return (0);
}

DESTRUCTOR INLINE int	ELF_close(void)
{
	ELF_CONTEXT = (CELF_ctx){0};

	if (ELF_SIZE == 0)
		return (1);
	if (ELF_RAW == (void *) -1)
		return (1);

	return munmap(ELF_RAW, ELF_SIZE);
}

INLINE int	strtab_cmp(const char *s1, const char *s2)
{
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}

INLINE const char	*strtab_find(const char *strtab, const char *needle, uint32_t size)
{
	const char	*tmp;

	for (tmp = strtab; size; size--, ++tmp)
	{
		if (*tmp != *needle )
			continue;
		if (strtab_cmp(tmp, needle) == 0)
			break ;
	}
	return (tmp);
}

# define	ELF_READ_FIELD(field)	READ_FIELD(field)
# define	ELF_OFFSET(o)			(ELF_CONTEXT.file.raw + (o))

# define	ELF_IN_FILE(ptr)		((ptr - ELF_RAW) < ELF_SIZE)

# if defined(CELF_ASSERT)
#  include <assert.h>
#  define	ELF_ASSERT_PTR(ptr)		assert(ELF_IN_FILE(ptr))
# else
#  define	ELF_ASSERT_PTR(ptr)		if (!ELF_IN_FILE(ptr)) { exit(1); }
# endif

# define	ELF_CALL_PREFIX(c)	CONCAT3(celf, c, _)
# define	ELF_CALL(name)		CONCAT(ELF_CALL_PREFIX(ELF_CLASS), name)
# define	ELF_64_CALL(name)	CONCAT(ELF_CALL_PREFIX(64), name)
# define	ELF_32_CALL(name)	CONCAT(ELF_CALL_PREFIX(32), name)

#endif	// _CELF_UTILS_H
