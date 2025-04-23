/**
 * celfFuncs
 *
 *		ELF Functions header.
 */

// #define _CELF_LSP

#if defined(_CELF_LSP)
# define _CELF_IMPLEMENTATION
# include "celf.h"
# define	ELF_CLASS 64
#endif

#if !defined(_CELF_H)
# error		"Missing dependency: celf.h"
#endif
#if !defined(_CELF_UTILS_H)
# error		"Missing dependency: celfUtils.h"
#endif
#if !defined(_CELF_ENUMS_H)
# error		"Missing dependency: celfEnums.h"
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

# define	CELF_CALL(name)		CONCAT(ELF_CALL_PREFIX(ELF_CLASS), name)

/* Context pointers functions ----------------------------------------------- */

/**
 * celf_x32_getFileHeader(void)
 * celf_x64_getFileHeader(void)
 *
 * @return	current ELF File Header pointer.
 */
INLINE ELF_HDR	*CELF_CALL(getFileHeader)(void)
{
	if (!ELF_CONTEXT.f_header)
		ELF_CONTEXT.f_header = ELF_RAW;

	return (ELF_HDR *) ELF_CONTEXT.f_header;
}

/**
 * celf_x32_getSectionHeaders(void);
 * celf_x64_getSectionHeaders(void);
 *
 * @return	current ELF File Sections Header Table pointer.
 */
ELF_SHDR	*CELF_CALL(getSectionHeaders)(void)
{
	if (!ELF_CONTEXT.s_headers)
	{
		const ELF_HDR	*f_header = CELF_CALL(getFileHeader)();
		const ELF_ULONG	s_headers_off = ELF_READ_FIELD(f_header->e_shoff);

		ELF_CONTEXT.s_headers = ELF_OFFSET(s_headers_off);

		ELF_ASSERT_PTR(ELF_CONTEXT.s_headers);
	}
	
	return (ELF_SHDR *) ELF_CONTEXT.s_headers;
}

/**
 * celf_x32_getSectionCount(void);
 * celf_x64_getSectionCount(void);
 *
 * @return	current ELF File Section Header size.
 */
INLINE ELF_USHORT	CELF_CALL(getSectionCount)(void)
{
	const ELF_HDR		*f_header = CELF_CALL(getFileHeader)();
	const ELF_USHORT	s_count = ELF_READ_FIELD(f_header->e_shnum);

	return s_count;
}

/**
 * celf_x32_getSectionContent(ELF_SHDR *section);
 * celf_x64_getSectionContent(ELF_SHDR *section);
 *
 * @return	Raw bytes pointer : ELF_RAW + section->sh_offset.
 */
INLINE ELF_UBYTE	*CELF_CALL(getSectionContent)(ELF_SHDR *section)
{
	ELF_ASSERT_PTR((ELF_UBYTE *)section);

	ELF_UBYTE	*s_content = NULL;
	ELF_ULONG	s_content_off = ELF_READ_FIELD(section->sh_offset);

	s_content = ELF_OFFSET(s_content_off);
	ELF_ASSERT_PTR(s_content);

	return s_content;
}

/**
 * celf_x32_getSectionSize(ELF_SHDR *section);
 * celf_x64_getSectionSize(ELF_SHDR *section);
 *
 * @return	ELF_ULONG containing the section size.
 */
INLINE ELF_ULONG	CELF_CALL(getSectionSize)(ELF_SHDR *section)
{
	ELF_ASSERT_PTR((ELF_UBYTE *)section);

	ELF_ULONG	s_size = ELF_READ_FIELD(section->sh_size);

	return s_size;
}

/**
 * celf_x32_getSectionByIndex(ELF_UINT index);
 * celf_x64_getSectionByIndex(ELF_UINT index);
 *
 * @return	Section Header pointer : ELF_CONTEXT.s_headers[index].
 */
ELF_SHDR	*CELF_CALL(getSectionByIndex)(ELF_UINT index)
{
	const ELF_USHORT	s_count	= CELF_CALL(getSectionCount)();

	if (index >= s_count)
		return (NULL);

	const ELF_SHDR	*s_headers = CELF_CALL(getSectionHeaders)();

	ELF_ASSERT_PTR((ELF_UBYTE *)(s_headers + index));

	return (ELF_SHDR *) &s_headers[index];
}

/**
 * celf_x32_getSectionNames(void);
 * celf_x64_getSectionNames(void);
 *
 * @return	char pointer to the .shstrtab section.
 */
INLINE ELF_STRING	CELF_CALL(getSectionNames)(void)
{
	if (!ELF_CONTEXT.section_names)
	{
		const ELF_HDR		*f_header = CELF_CALL(getFileHeader)();
		const ELF_USHORT	shstridx = ELF_READ_FIELD(f_header->e_shstridx);

		ELF_SHDR	*shstr = CELF_CALL(getSectionByIndex)(shstridx);
		ELF_CONTEXT.section_names = CELF_CALL(getSectionContent)(shstr);
	}

	return (ELF_STRING) ELF_CONTEXT.section_names;
}

/**
 * celf_x32_getSectionNames(void);
 * celf_x64_getSectionNames(void);
 *
 * @return	char pointer to the .shstrtab section.
 */
INLINE ELF_STRING	CELF_CALL(getSectionName)(ELF_SHDR *section)
{
	ELF_STRING	names = CELF_CALL(getSectionNames)();
	ELF_UINT	name_off = ELF_READ_FIELD(section->sh_name);

	return names + name_off;
}

/**
 * celf_x32_getSectionByName(void);
 * celf_x64_getSectionByName(void);
 *
 * @return	ELF Section Header pointer to the section named "name".
 */
ELF_SHDR	*CELF_CALL(getSectionByName)(ELF_STRING name)
{
	const ELF_USHORT	s_count	= CELF_CALL(getSectionCount)();
	const ELF_STRING	s_names = CELF_CALL(getSectionNames)();
	const ELF_UINT		name_ref = strtab_find(s_names, name) - s_names;	

	if (name_ref == 0)
		return (NULL);

	ELF_SHDR	*s_iterator = CELF_CALL(getSectionHeaders)();

	for (ELF_USHORT i = 0; i < s_count; ++i)
	{
		ELF_UINT	name_idx = ELF_READ_FIELD(s_iterator->sh_name);

		if (name_idx == name_ref)
			return s_iterator;
	}
	return NULL;
}

/**
 * celf_x32_getSectionByType(void);
 * celf_x64_getSectionByType(void);
 *
 * @return	ELF Section Header pointer to the FIRST section typed "type"
 */
ELF_SHDR	*CELF_CALL(getSectionByType)(ELF_SectionType type)
{
	const ELF_USHORT	s_count	= CELF_CALL(getSectionCount)();

	ELF_SHDR	*s_iterator = CELF_CALL(getSectionHeaders)();

	for (ELF_USHORT i = 0; i < s_count; ++i, ++s_iterator)
	{
		ELF_UINT	s_type = ELF_READ_FIELD(s_iterator->sh_type);

		if (s_type == type)
			return s_iterator;
	}
	return NULL;
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

INLINE ELF_UINT	CELF_CALL(getSymbolCount)(void)
{
	ELF_SHDR	*sym_header	= CELF_CALL(getSectionByType)(SHT_SYMTAB);

	ELF_ULONG	size		= ELF_READ_FIELD(sym_header->sh_size);
	ELF_ULONG	sym_size	= ELF_READ_FIELD(sym_header->sh_entsize);

	return (ELF_UINT) (size / sym_size);
}

#undef ELF_CLASS
