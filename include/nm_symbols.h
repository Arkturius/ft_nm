/**
 *	nm_symbols.h
 * -------------------------------------------------------------------------- */

#ifndef _NM_SYMBOLS_H
# define _NM_SYMBOLS_H

#define	ELF32_SHDR(X)	((ELF32_Shdr *)(X))
#define	ELF64_SHDR(X)	((ELF64_Shdr *)(X))

#define	ELF32_SYM(X)	((ELF32_Sym *)(X))
#define	ELF64_SYM(X)	((ELF64_Sym *)(X))

#define	SYM_NAME(X)		((X)->name)
#define SYM_VALUE(X)	((X)->value)
#define	SYM_SHIDX(X)	((X)->shidx)
#define	SYM_BIND(X)		((X)->bind)
#define	SYM_TYPE(X)		((X)->type)
#define	SYM_SECTION(X)	((X)->section)

typedef struct
{
	const char	*name;
	const void	*section;
	
	uint64_t	value;
	uint16_t	shidx;
	int32_t		index;
	uint8_t		bind;
	uint8_t		type;
}	nm_SYM;

typedef struct
{
	uint32_t	count;
	nm_SYM		table[];
}	nm_SYMTAB;

nm_SYMTAB	*nm_symtab_init(void);
void		nm_symbols_extract(nm_SYMTAB *symbols);
void		nm_symbols_display(const char *file, nm_SYMTAB *symbols);

#endif // _NM_SYMBOLS_H
