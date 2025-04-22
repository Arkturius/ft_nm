/**
 * celfFuncs
 *
 *		ELF Functions header.
 */

#include "celfUtils.h"
#include <string.h>
#define _CELF_LSP_FRIENDLY

#if defined(_CELF_LSP_FRIENDLY)
# include <celf.h>
# define ELF_CLASS	64
#endif

#if !defined(_CELF_H)
# error		"Missing dependency: celf.h"
#endif
#if !defined(_CELF_UTILS_H)
# error		"Missing dependency: celfUtils.h"
#endif
#if !defined(_CELF_ENUMS_H)
# error		"Missing dependency: celfFuncs.h"
#endif
#if !defined(ELF_CLASS)
# error		"ELF_CLASS not defined."
#endif

#define		ELF_T_PRE	CONCAT3(ELF, ELF_CLASS, _)
#define		ELF_T(X)	CONCAT(ELF_T_PRE, X)

#define		ELF_HDR		ELF_T(Hdr)
#define		ELF_PHDR	ELF_T(Phdr)
#define		ELF_SHDR	ELF_T(Shdr)
#define		ELF_SYM		ELF_T(Sym)
#define		ELF_REL		ELF_T(Rel)
#define		ELF_RELA	ELF_T(Rela)
#define		ELF_DYN		ELF_T(Dyn)

#define		ELF_UBYTE	uint8_t
#define		ELF_USHORT	uint16_t
#define		ELF_UINT	uint32_t
#define		ELF_ULONG	CONCAT3(uint, ELF_CLASS, _t)
#define		ELF_STRING	char*

#define		CELF_CALL(name)	CONCAT4(celf, ELF_CLASS, _, name)

/**
 * celf ## XX ## _getFileHeader(void)
 *
 *		- returns a pointer to a ELF File Header structure.
 */
INLINE ELF_HDR	*CELF_CALL(getFileHeader)(void)
{
	if (!ELF_CONTEXT.f_header)
		ELF_CONTEXT.f_header = ELF_CONTEXT.file.raw;

	return (ELF_HDR *) ELF_CONTEXT.f_header;
}

/**
 * celf ## XX ## _getSectionHeaders(void)
 *
 *		- returns a pointer to the Sections Header Table.
 */
INLINE ELF_SHDR	*CELF_CALL(getSectionHeaders)(void)
{
	if (!ELF_CONTEXT.s_headers)
	{
		ELF_HDR		*f_header		= CELF_CALL(getFileHeader)();
		ELF_ULONG	s_headers_off	= ELF_READ_FIELD(f_header->e_shoff);

		ELF_CONTEXT.s_headers = ELF_OFFSET(s_headers_off);

		ELF_ASSERT_PTR(ELF_CONTEXT.s_headers);
	}
	
	
	return (ELF_SHDR *) ELF_CONTEXT.s_headers;
}

INLINE ELF_STRING	CELF_CALL(getSectionNames)(void)
{
	if (!ELF_CONTEXT.section_names)
	{
		ELF_HDR		*f_header = CELF_CALL(getFileHeader)();
		ELF_USHORT	shstrtab_off = ELF_READ_FIELD(f_header->footer.e_shstridx);

		ELF_CONTEXT.section_names = ELF_OFFSET(shstrtab_off);

		ELF_ASSERT_PTR(ELF_CONTEXT.section_names);
	}

	return (ELF_STRING) ELF_CONTEXT.section_names;
}

INLINE ELF_UBYTE	*CELF_CALL(getSectionContent)(ELF_SHDR *section)
{
	ELF_UBYTE	*s_content = NULL;

	ELF_ASSERT_PTR((ELF_UBYTE *)section);

	ELF_ULONG	s_content_off	= ELF_READ_FIELD(section->sh_offset);
	s_content					= ELF_OFFSET(s_content_off);

	ELF_ASSERT_PTR(s_content);

	return s_content;
}

/**
 * celf ## ELF_CLASS ## _getSectionByIndex(ELF_UINT index)
 *
 *		- returns a pointer to the Section Header at this index.
 */
ELF_SHDR	*CELF_CALL(getSectionByIndex)(ELF_UINT index)
{
	ELF_HDR		*f_header	= CELF_CALL(getFileHeader)();
	ELF_USHORT	s_count		= ELF_READ_FIELD(f_header->footer.e_shnum);
	ELF_USHORT	s_size		= ELF_READ_FIELD(f_header->footer.e_shsize);

	if (index >= s_count)
		return (NULL);

	CELF_CALL(getSectionHeaders)();
	ELF_CONTEXT.asst = ELF_CONTEXT.s_headers + (index * s_size);
	ELF_ASSERT_PTR(ELF_CONTEXT.asst);

	return (ELF_SHDR *) ELF_CONTEXT.asst;
}

ELF_SHDR	*CELF_CALL(getSectionByName)(ELF_STRING name)
{
	ELF_HDR		*f_header	= CELF_CALL(getFileHeader)();
	ELF_USHORT	s_count		= ELF_READ_FIELD(f_header->footer.e_shnum);
	ELF_STRING	names		= CELF_CALL(getSectionNames)();
	ELF_UINT	name_ref	= 0;
	
	name_ref = strtab_find(names, name) - names;

	if (name_ref == 0)
		return (NULL);

	ELF_SHDR	*curr = NULL;
	ELF_USHORT	i = 0;
	CELF_CALL(getSectionHeaders)();

	for (; i < s_count; ++i)
	{
		curr					= CELF_CALL(getSectionByIndex)(i);
		ELF_UINT	name_idx	= ELF_READ_FIELD(curr->sh_name);

		if (name_idx == name_ref)
			break ;
	}

	ELF_CONTEXT.asst = (ELF_UBYTE *)curr;
	ELF_ASSERT_PTR(ELF_CONTEXT.asst);

	return (ELF_SHDR *) ELF_CONTEXT.asst;
}

/**
 * celf ## ELF_CLASS ## _getSectionByType(enum ELF_SectionType type)
 *
 *		- returns a pointer to the 1st Section Header with this type.
 */
ELF_SHDR	*CELF_CALL(getSectionByType)(enum ELF_SectionType type)
{
	ELF_HDR		*f_header	= CELF_CALL(getFileHeader)();
	ELF_USHORT	s_count		= ELF_READ_FIELD(f_header->footer.e_shnum);

	ELF_SHDR	*curr = NULL;
	ELF_USHORT	i = 0;
	CELF_CALL(getSectionHeaders)();
	
	for (; i < s_count; ++i)
	{
		curr				= CELF_CALL(getSectionByIndex)(i);
		ELF_UINT	comp	= ELF_READ_FIELD(curr->sh_type);

		if (comp == type)
			break ;
	}

	if (i != s_count)
		ELF_CONTEXT.asst = (ELF_UBYTE *) curr;
	ELF_ASSERT_PTR(ELF_CONTEXT.asst);

	return (ELF_SHDR *) ELF_CONTEXT.asst;
}

INLINE ELF_PHDR	*CELF_CALL(getProgramHeaders)(void)
{
	if (!ELF_CONTEXT.p_headers)
	{
		ELF_HDR		*f_header		= CELF_CALL(getFileHeader)();
		ELF_ULONG	p_headers_off	= ELF_READ_FIELD(f_header->e_phoff);

		ELF_CONTEXT.p_headers = ELF_OFFSET(p_headers_off);

		ELF_ASSERT_PTR(ELF_CONTEXT.p_headers);
	}

	return (ELF_PHDR *) ELF_CONTEXT.p_headers;
}

ELF_SYM	*CELF_CALL(getSymbols)(void)
{
	if (!ELF_CONTEXT.symbols)
	{
		ELF_SHDR	*sym_header	= CELF_CALL(getSectionByType)(SHT_SYMTAB);
		
		ELF_CONTEXT.symbols = CELF_CALL(getSectionContent)(sym_header);
	}

	return (ELF_SYM *) ELF_CONTEXT.symbols;
}

#undef ELF_CLASS
