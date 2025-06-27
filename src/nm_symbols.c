/**
 *	nm_symbols.c
 * -------------------------------------------------------------------------- */

#include <nm.h>

INLINE int	nm_symbol_is_undef
(nm_SYM *symbol)
{
	return (SYM_SHIDX(symbol) == SHN_UNDEF);
}

INLINE int	nm_symbol_is_extrn
(nm_SYM *symbol)
{
	uint8_t	bind = SYM_BIND(symbol);

	return ((bind == STB_GLOBAL) || (bind == STB_WEAK));
}

INLINE int	nm_symbol_is_debug
(nm_SYM *symbol)
{
	uint8_t	type = SYM_TYPE(symbol);

	return (((type == STT_FILE) && !SYM_VALUE(symbol)) || (type == STT_SECTION));
}

static uint8_t	*nm_symbol_names
(void *symtab_hdr)
{
	if (!symtab_hdr)
		return (NULL);

	uint32_t	symtab_link = -1u;
	uint32_t	sheaders_size = ELF_sheaders_size();
	void		*sheaders = NULL;
	void		*symstrtab = NULL;
	
	sheaders = ELF_sheaders_get();
	if (ELF_is64bit())
	{
		symtab_link = READ_FIELD(((ELF64_Shdr *)symtab_hdr)->sh_link);
		symstrtab = ((ELF64_Shdr *)sheaders) + symtab_link;
	}
	else
	{
		symtab_link = READ_FIELD(((ELF32_Shdr *)symtab_hdr)->sh_link);
		symstrtab = ((ELF32_Shdr *)sheaders) + symtab_link;
	}
	if (symtab_link == -1u || symtab_link > sheaders_size)
		return (NULL);
	return (ELF_section_content(symstrtab));
}

static int	nm_symbol_extract64
(ELF64_Sym *elf_sym, nm_SYM *sym)
{
	void		*symtab_hdr = ELF_sheader_get_by_name(".symtab");
	uint8_t		*names = nm_symbol_names(symtab_hdr);
	
	if (!names)
		return (1);

	uint32_t	name_idx = READ_FIELD(elf_sym->st_name);
	uint8_t		info = READ_FIELD(elf_sym->st_info);
	uint16_t	shidx = READ_FIELD(elf_sym->st_shidx);

	SYM_VALUE(sym) = READ_FIELD(elf_sym->st_value);
	SYM_SECTION(sym) = ELF_sheader_get_by_idx(shidx);
	SYM_SHIDX(sym) = shidx;
	SYM_TYPE(sym) = ST_TYPE(info);
	SYM_BIND(sym) = ST_BIND(info);
	SYM_NAME(sym) = (const char *)names + name_idx;

	return (0);
}

static int	nm_symbol_extract32
(ELF32_Sym *elf_sym, nm_SYM *sym)
{
	void		*symtab_hdr = ELF_sheader_get_by_name(".symtab");
	uint8_t		*names = nm_symbol_names(symtab_hdr);
	
	if (!names)
		return (1);

	uint32_t	name_idx = READ_FIELD(elf_sym->st_name);
	uint8_t		info = READ_FIELD(elf_sym->st_info);
	uint16_t	shidx = READ_FIELD(elf_sym->st_shidx);

	SYM_VALUE(sym) = READ_FIELD(elf_sym->st_value);
	SYM_SECTION(sym) = ELF_sheader_get_by_idx(shidx);
	SYM_SHIDX(sym) = shidx;
	SYM_TYPE(sym) = ST_TYPE(info);
	SYM_BIND(sym) = ST_BIND(info);
	SYM_NAME(sym) = (const char *)names + name_idx;

	return (0);
}

static char	*nm_section_name
(const void *section)
{
	char		*section_names = ELF_shstrtab_get();

	if (!section_names)
		return (NULL);

	uint32_t	name_idx;

	if (ELF_is64bit())
		name_idx = READ_FIELD(ELF64_SHDR(section)->sh_name);
	else
		name_idx = READ_FIELD(ELF32_SHDR(section)->sh_name);

	return (section_names + name_idx);
}

static int	nm_symbol_filter
(nm_SYM *sym)
{
	if ((SYM_TYPE(sym) == STT_NOTYPE) && SYM_NAME(sym) && SYM_NAME(sym)[0] == 0)
		return (1);
	if (nm_flags_isset(NM_DISP_UNDEF) && !nm_symbol_is_undef(sym))
		return (1);
	if (nm_flags_isset(NM_DISP_EXTRN) && !nm_symbol_is_extrn(sym))
		return (1);
	if (!nm_flags_isset(NM_DISP_DEBUG) && nm_symbol_is_debug(sym))
		return (1);
	return (0);
}

void	nm_symbols_extract
(nm_SYMTAB *symbols)
{
	uint32_t	symtab_size = ELF_symtab_size();
	void		*symtab = ELF_symtab_get();

	if (symtab_size == 0 || !symtab)
		return ;

	for (uint32_t i = 0; i < symtab_size; ++i)
	{
		nm_SYM	*symbol = symbols->table + symbols->count;

		if (ELF_is64bit())
			nm_symbol_extract64(ELF64_SYM(symtab) + i, symbol);
		else
			nm_symbol_extract32(ELF32_SYM(symtab) + i, symbol);

		if (nm_symbol_filter(symbol))
			continue ;

		if (SYM_SECTION(symbol) && SYM_TYPE(symbol) == STT_SECTION)
			SYM_NAME(symbol) = nm_section_name(SYM_SECTION(symbol));
		
		symbol->index = i;
		symbols->count++;
	}
}

static int	nm_symbol_cmp
(const void *a_ptr, const void *b_ptr)
{
	const nm_SYM	*a = a_ptr;
	const nm_SYM	*b = b_ptr;
	int				ret = 0;

	ret = u_strcmp(SYM_NAME(a), SYM_NAME(b));	
	if (!ret)
		return (a->index - b->index);
	if (nm_flags_isset(NM_SORT_REVERSE))
		return (-ret);
	return (ret);
}

static void	nm_symbols_sort
(nm_SYMTAB *symbols)
{
	if (nm_flags_isset(NM_SORT_NONE))
		return ;
	u_qsort(symbols->table, symbols->count, sizeof(nm_SYM), &nm_symbol_cmp);
}

static char	nm_symbol_type
(nm_SYM *symbol)
{
	uint8_t		type = SYM_TYPE(symbol);
	uint8_t		bind = SYM_BIND(symbol);
	uint16_t	shidx = SYM_SHIDX(symbol);

	char		letter = '?';

	if (bind == STB_WEAK)
	{
		if (type == STT_GNU_IFUNC)
			letter = 'i';
		else
		{
			letter = (type == STT_OBJECT) ? 'V' : 'W';
			letter = (shidx == SHN_UNDEF) ? letter | 32 : letter;
		}
	}
	else if (shidx == SHN_UNDEF)
		letter = 'U';
	else if (shidx == SHN_ABS)
		letter = 'A';
	else if (shidx == SHN_COMMON)
		letter = 'C';
	else
	{
		if (type == STT_GNU_IFUNC)
			letter = 'i';
		else
		{
			uint64_t	sflags;
			uint32_t	stype;

			if (ELF_is64bit())
			{
				const ELF64_Shdr	*section = SYM_SECTION(symbol);
				
				sflags = READ_FIELD(section->sh_flags);
				stype = READ_FIELD(section->sh_type);
			}
			else
			{
				const ELF32_Shdr	*section = SYM_SECTION(symbol);
				
				sflags = READ_FIELD(section->sh_flags);
				stype = READ_FIELD(section->sh_type);
			}

			if (sflags & SHF_EXECINSTR)
				letter = 'T';
			else if (sflags & SHF_WRITE)
				letter = (stype == SHT_NOBITS) ? 'B' : 'D';
			else if (sflags & SHF_ALLOC)
				letter = 'R';
			else if (stype == SHT_PROGBITS)
			{
				if (!u_strncmp(symbol->name, ".debug", 6))
					return ('N');
				return (sflags & SHF_WRITE) ? 'N' : 'n';
			}
			else if (sflags == SHF_SOLARIS)
				return ('n');
		}
	}
	if (bind == STB_LOCAL && letter >= 'A' && letter <= 'Z')
		letter |= 32;
	return (letter);
}

static void	nm_symbol_display_value
(nm_SYM *symbol)
{
	const char	*fmt = "%016lx\x00\x00%08lx\x00\x00";
	const char	*empty = "                ";

	if (nm_symbol_is_undef(symbol))
		fmt = empty;
	if (ELF_is32bit())
		fmt += 8;

	u_printf(fmt, SYM_VALUE(symbol));
}

static void	nm_symbol_display
(nm_SYM *symbol)
{
	char	letter = nm_symbol_type(symbol);

	nm_symbol_display_value(symbol);
	u_printf(" %c %s\n", letter, symbol->name);
}

void	nm_symbols_display
(const char *file, nm_SYMTAB *symbols)
{
	nm_symbols_sort(symbols);

	if (nm_flags_isset(NM_FILENAME_PER_FILE))
		u_printf("\n%s:\n", file);
	
	for (uint32_t i = 0; i < symbols->count; ++i)
	{
		nm_SYM	*symbol = symbols->table + i;

		nm_symbol_display(symbol);
	}
}

nm_SYMTAB	*nm_symtab_init
(void)
{
	uint32_t	symtab_size = ELF_symtab_size();
	uint32_t	size = sizeof(nm_SYMTAB) + symtab_size * sizeof(nm_SYM);
	nm_SYMTAB	*symbols = malloc(size);

	if (symbols)
		u_memset(symbols, 0, size);
	return (symbols);
}
