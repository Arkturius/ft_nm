/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_functions.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 11:59:46 by rgramati          #+#    #+#             */
/*   Updated: 2025/04/21 16:18:12 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 *	nm_functions.h
 *
 *		generic ft_nm functions declarations to be defined for 32 and 64.
 */

// #define NM_LSIMP

#include <stdint.h>
#include <stdio.h>

#if defined(NM_LSIMP)
# include <ft_nm.h>
# define NM_ELF_CLASS 64
#endif

#if !defined(FT_NM_H)
# error "This file should not be included outside of ft_nm.h"
#endif

#if !defined(NM_ELF_CLASS)
# error "NM_ELF_CLASS must be defined."
#endif

#define	NM_ELF_TYPEDEF		NM_CONCAT(ELF, NM_ELF_CLASS)
#define	NM_ELF_TYPENAME(t)	NM_CONCAT(NM_ELF_TYPEDEF, NM_CONCAT(_, t))

#define NM_ELF_FUNC(n)		NM_CONCAT_4(nm, NM_ELF_CLASS, _, n)

#define	NM_ELF_HEADER		NM_ELF_TYPENAME(Hdr)
#define	NM_ELF_SHEADER		NM_ELF_TYPENAME(Shdr)
#define NM_ELF_SYMBOL		NM_ELF_TYPENAME(Sym)
#define NM_ELF_SYMTAB		NM_CONCAT(symtab, NM_ELF_CLASS)
#define	NM_ELF_ULONG			NM_CONCAT(NM_CONCAT(uint, NM_ELF_CLASS), _t)

static void	NM_ELF_FUNC(findSectionHeaders)(void)
{
	NM_ELF_HEADER	*header;
	NM_ELF_ULONG		sections_off;

	header			= NM_CONCAT(NM_CONTEXT.file.hdr, NM_ELF_CLASS);
	sections_off	= CELF_FIELD(header->e_shoff);

	NM_CONTEXT.file.NM_CONCAT(hdr, NM_ELF_CLASS) = header;

	NM_CONTEXT.sections_count	= CELF_FIELD(header->footer.e_shnum);
	NM_CONTEXT.sections_hdr		= NM_FILE_OFF(sections_off);
}

void	NM_ELF_FUNC(findSymbolTable)(void)
{
	NM_ELF_FUNC(findSectionHeaders)();
	
	NM_ELF_SHEADER	*sections_start;
	NM_ELF_SHEADER	*sections_end;
	NM_ELF_SHEADER	*current;

	sections_start	= (NM_ELF_SHEADER *) NM_CONTEXT.sections_hdr;
	sections_end	= sections_start + NM_CONTEXT.sections_count;

	for (
		current = sections_start;
		current != sections_end && CELF_FIELD(current->sh_type) != SHT_SYMTAB;
		++current
	) {}
	if (current == sections_end)
		nm_error("No symbol table found.", 4);

	NM_ELF_SHEADER	*symtab_hdr;		
	NM_ELF_SHEADER	*symtab_str_hdr;
	NM_ELF_ULONG		symtab_off;
	NM_ELF_ULONG		symtab_str_off;

	symtab_hdr		= current;
	symtab_off		= CELF_FIELD(symtab_hdr->sh_offset);
	symtab_str_hdr	= sections_start + CELF_FIELD(symtab_hdr->sh_link);
	symtab_str_off	= CELF_FIELD(symtab_str_hdr->sh_offset);

	NM_CONTEXT.symtab_str		= (char *)NM_FILE_OFF(symtab_str_off);
	NM_CONTEXT.NM_ELF_SYMTAB	= (NM_ELF_SYMBOL *)NM_FILE_OFF(symtab_off);

	NM_ELF_ULONG		symtab_size;

	symtab_size				= CELF_FIELD(symtab_hdr->sh_size);
	NM_CONTEXT.symbol_count	= symtab_size / sizeof(NM_ELF_SYMBOL);
}

static void	NM_ELF_FUNC(fetchSymbolNames)(char **names)
{
	for (uint32_t i = 0; i < NM_CONTEXT.symbol_count; ++i)
	{
		NM_ELF_SYMBOL	*current	= NM_CONTEXT.NM_ELF_SYMTAB + i;
		uint32_t		idx			= CELF_FIELD(current->st_name);

		if (idx == 0)
		{
			uint16_t		section_index = CELF_FIELD(current->st_shidx);
			
			if (section_index >= NM_CONTEXT.sections_count)
			{
				names[i] = "";
				continue ;
			}	

			uint8_t			*raw = (NM_CONTEXT.sections_hdr + section_index * sizeof(NM_ELF_SHEADER));
			NM_ELF_SHEADER	*section = (NM_ELF_SHEADER *)raw;
			
			idx = CELF_FIELD(section->sh_name);

			NM_ELF_HEADER	*header = NM_CONCAT(NM_CONTEXT.file.hdr, NM_ELF_CLASS);
			section_index = CELF_FIELD(header->footer.e_shstridx);
			raw = (NM_CONTEXT.sections_hdr + section_index * sizeof(NM_ELF_SHEADER));
			section = (NM_ELF_SHEADER *)raw;
			uint8_t	*shstrtab = NM_FILE_OFF(CELF_FIELD(section->sh_offset));

			names[i] = (char *)(shstrtab + idx);
		}
		else
			names[i] = NM_CONTEXT.symtab_str + idx;
	}
}

static void	NM_ELF_FUNC(sortSymbols)(NM_ELF_SYMBOL *symbols, char **names)
{
	uint32_t	nsym = NM_CONTEXT.symbol_count;

	if (!NM_OPT(NM_NOSORT))
	{
		uint8_t	swapped;
		for (uint32_t i = 0; i < nsym - 1; ++i)
		{
			swapped = 0;
			for (uint32_t j = 0; j < nsym - 1 - i; ++j)
			{
				if (nm_strcmp(names[j], names[j + 1]) > 0)
				{
					nm_memswap(symbols + j, symbols + j + 1, sizeof(NM_ELF_SYMBOL));
					nm_memswap(names + j, names + j + 1, sizeof(char *));
					swapped = 1;
				}
			}
			if (!swapped)
				break;
		}
		if (NM_OPT(NM_REVSORT))
		{
			for (uint32_t i = 0; i < nsym / 2; ++i)
			{
				nm_memswap(symbols + i, symbols + nsym - i - 1, sizeof(NM_ELF_SYMBOL));
				nm_memswap(names + i, names + nsym - i - 1, sizeof(char *));
			}
		}
	}
}

void	NM_ELF_FUNC(processSymbols)(void)
{
	uint32_t		symbol_bsize;
	NM_ELF_SYMBOL	*symtab_sort		= NULL;
	char			**symtab_str_sort	= NULL;

	symbol_bsize = NM_CONTEXT.symbol_count * sizeof(NM_ELF_SYMBOL);

	symtab_sort = malloc(symbol_bsize);
	if (!symtab_sort)
		goto defer;

	symtab_str_sort = malloc(NM_CONTEXT.symbol_count * sizeof(char *));
	if (!symtab_str_sort)
		goto defer;

	nm_memcpy(symtab_sort, NM_CONTEXT.NM_ELF_SYMTAB, symbol_bsize);
	
	NM_ELF_FUNC(fetchSymbolNames)(symtab_str_sort);
	NM_ELF_FUNC(sortSymbols)(symtab_sort, symtab_str_sort);

	NM_CONTEXT.NM_ELF_SYMTAB	= symtab_sort;
	NM_CONTEXT.symbol_names		= symtab_str_sort;
	return ;

defer:
	free(symtab_sort);
	free(symtab_str_sort);
	nm_error("Intern allocation failed.", 3);
}

static char	*NM_ELF_FUNC(fetchSymbolLetter)(NM_ELF_SYMBOL *sym)
{
	static char	letter = 0;

	uint16_t	section_index = CELF_FIELD(sym->st_shidx);
	uint8_t		symbol_info = CELF_FIELD(sym->st_info);	
	uint8_t		symbol_type = NM_CONCAT_3(ELF, NM_ELF_CLASS, _ST_TYPE)(symbol_info);
	uint8_t		symbol_bind = NM_CONCAT_3(ELF, NM_ELF_CLASS, _ST_BIND)(symbol_info);

	letter = '?';
	if (symbol_bind == STB_WEAK)
	{
		if (symbol_type == STT_OBJECT)
			letter = 'V';
		else
			letter = 'W';
		if (section_index == SHN_UNDEF)
			letter |= 32;
	}
	else if (section_index == SHN_UNDEF)
		letter = 'U';
	else if (section_index == SHN_ABS)
		letter = 'A';
	else if (section_index == SHN_COMMON)
		letter = 'C';
	else
	{
		if (symbol_type == STT_GNU_IFUNC)
			return (letter = 'I', &letter);
		else
		{
			NM_ELF_SHEADER	*section = ((NM_ELF_SHEADER *)NM_CONTEXT.sections_hdr) + section_index;
			NM_ELF_ULONG	section_flags = CELF_FIELD(section->sh_flags);
			uint32_t		section_type = CELF_FIELD(section->sh_type);


			if (section_flags & SHF_EXECINSTR)
				letter = 'T';
			else if (section_flags & SHF_WRITE)
			{
				letter = 'D';
				if (section_type == SHT_NOBITS)
					letter = 'B';
			}
			else if (section_flags & SHF_ALLOC)
				letter = 'R';
			else if (section_type == SHT_PROGBITS)
				letter = 'N';
		}
	}	
	if (symbol_bind == STB_LOCAL && letter >= 'A' && letter <= 'Z')
		letter |= 32;
	return (&letter);
}

void	NM_ELF_FUNC(printSymbol)(uint64_t value, NM_ELF_SYMBOL *sym, char *name)
{
	uint16_t	section_index = CELF_FIELD(sym->st_shidx);

	if (section_index == SHN_UNDEF)
	{
		if (!*name)
			return ;
		write(STDOUT_FILENO, "                ", NM_ELF_CLASS / 4);
	}
	else
		write(STDOUT_FILENO, nm_hexitoa(value, NM_ELF_CLASS / 4), NM_ELF_CLASS / 4);
	write(STDOUT_FILENO, " ", 1);
	write(STDOUT_FILENO, NM_ELF_FUNC(fetchSymbolLetter)(sym), 1);
	write(STDOUT_FILENO, " ", 1);
	write(STDOUT_FILENO, name, nm_strlen(name));
	write(STDOUT_FILENO, "\n", 1);
}

void	NM_ELF_FUNC(displaySymbols)(void)
{
	NM_ELF_SYMBOL	*symtab = NM_CONTEXT.NM_ELF_SYMTAB;
	
	for (uint32_t i = 0; i < NM_CONTEXT.symbol_count; ++i)
	{
		NM_ELF_SYMBOL	*current = symtab + i;
		NM_ELF_ULONG	sym_value = CELF_FIELD(current->st_value);
		uint64_t		value = (uint64_t)sym_value;

		NM_ELF_FUNC(printSymbol)(value, current, NM_CONTEXT.symbol_names[i]);
	}
}

#undef	NM_ELF_CLASS

#undef	NM_ELF_TYPEDEF
#undef	NM_ELF_TYPENAME

#undef	NM_ELF_FUNC

#undef	NM_ELF_HEADER
#undef	NM_ELF_SHEADER
#undef	NM_ELF_SYMBOL
#undef	NM_ELF_SYMTAB
#undef	NM_ELF_ULONG
