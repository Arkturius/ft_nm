/**
 * celfEnums
 *
 *		ELF Enumerates header.
 */

#ifndef _CELF_ENUMS_H
# define _CELF_ENUMS_H

# ifndef _CELF_ENUMS_TO_STRINGS

enum	ELF_IdentIndices
{
	EI_MAG0			= 0,
	EI_MAG1			= 1,
	EI_MAG2			= 2,
	EI_MAG3			= 3,
	EI_CLASS		= 4,
	EI_DATA			= 5,
	EI_VERSION		= 6,
	EI_OSABI		= 7,
	EI_ABIVERSION	= 8,
};

enum	ELF_Class
{
	ELF_32BIT		= 1,
	ELF_64BIT		= 2,
};

enum	ELF_Endianness
{
	ELF_DATANONE	= 0x00,
	ELF_DATA2LSB	= 0x01,
	ELF_DATA2MSB	= 0x02,
};

enum	ELF_OsAbi
{
	EABI_SYSV		= 0x00,
	EABI_HPUX		= 0x01,
	EABI_NETBSD		= 0x02,
	EABI_LINUX		= 0x03,
	EABI_HURD		= 0x04,
	EABI_SOLARIS	= 0x06,
	EABI_AIX		= 0x07,
	EABI_IRIX		= 0x08,
	EABI_FREEBSD	= 0x09,
	EABI_TRU64		= 0x0A,
	EABI_MODESTO	= 0x0B,
	EABI_OPENBSD	= 0x0C,
	EABI_OPENVMS	= 0x0D,
	EABI_NSK		= 0x0E,
	EABI_AROS		= 0x0F,
	EABI_FENIXOS	= 0x10,
	EABI_CLOUDABI	= 0x11,
	EABI_OPENVOS	= 0x12,
};

enum	ELF_ExType
{
	ET_NONE		= 0x00,
	ET_REL		= 0x01,
	ET_EXEC		= 0x02,
	ET_DYN		= 0x03,
	ET_CORE		= 0x04,
	ET_LOOS		= 0xFE00,
	ET_HIOS		= 0xFEFF,
	ET_LOPROC	= 0xFF00,
	ET_HIPROC	= 0xFFFF
};

enum	ELF_Machine
{
    EM_NONE        = 0x00,
    EM_M32         = 0x01,
    EM_SPARC       = 0x02,
    EM_386         = 0x03,
    EM_68K         = 0x04,
    EM_88K         = 0x05,
    EM_IAMCU       = 0x06,
    EM_860         = 0x07,
    EM_MIPS        = 0x08,
    EM_ARM         = 0x28,
    EM_X86_64      = 0x3E,
	EM_AARCH64     = 0xB7,
    EM_RISCV       = 0xF3,
};

enum	ELF_Version
{
	EV_NONE		=	0x00,
	EV_CURRENT	=	0x01,
	EV_NUM		=	0x02,
};

enum	ELF_ProgType
{
	PT_NULL			= 0x00000000,
	PT_LOAD			= 0x00000001,
	PT_DYNAMIC		= 0x00000002,
	PT_INTERP		= 0x00000003,
	PT_NOTE			= 0x00000004,
	PT_SHLIB		= 0x00000005,
	PT_PHDR			= 0x00000006,
	PT_TLS			= 0x00000007,
	PT_LOOS			= 0x60000000,
	PT_GNU_EH_FRAME	= 0x6474e550,
	PT_GNU_STACK	= 0x6474e551,
	PT_GNU_RELRO	= 0x6474e552,
	PT_GNU_PROPERTY	= 0x6474e553,
	PT_LOSUNW		= 0x6ffffffa,
	PT_SUNWBSS		= 0x6ffffffa,
	PT_SUNWSTACK	= 0x6ffffffb,
	PT_HISUNW		= 0x6fffffff,
	PT_HIOS			= 0x6FFFFFFF,
	PT_LOPROC		= 0x70000000,
	PT_HIPROC		= 0x7FFFFFFF,
};

enum	ELF_ProgFlags
{
	PF_X		= (1 << 0),
	PF_W		= (1 << 1),
	PF_R		= (1 << 2),
	PF_MASKOS	= 0x0FF00000,
	PF_MASKPROC	= 0xF0000000,
};

enum	ELF_SectionType
{
	SHT_NULL			= 0x0,
	SHT_PROGBITS		= 0x1,
	SHT_SYMTAB			= 0x2,
	SHT_STRTAB			= 0x3,
	SHT_RELA			= 0x4,
	SHT_HASH			= 0x5,
	SHT_DYNAMIC			= 0x6,
	SHT_NOTE			= 0x7,
	SHT_NOBITS			= 0x8,
	SHT_REL				= 0x9,
	SHT_SHLIB			= 0x0A,
	SHT_DYNSYM			= 0x0B,
	SHT_INIT_ARRAY		= 0x0E,
	SHT_FINI_ARRAY		= 0x0F,
	SHT_PREINIT_ARRAY	= 0x10,
	SHT_GROUP			= 0x11,
	SHT_SYMTAB_SHNDX	= 0x12,
	SHT_NUM				= 0x13,
	SHT_LOOS			= 0x60000000,
	SHT_GNU_VERSYM		= 0x6FFFFFF0,
	SHT_GNU_HASH		= 0x6FFFFFF6,
	SHT_GNU_LIBLIST		= 0x6FFFFFF7,
	SHT_GNU_VERDEF		= 0x6FFFFFFD,
	SHT_GNU_VERNEED		= 0x6FFFFFFE,
	SHT_HIOS			= 0xFFFFFFFF,
};

enum	ELF_SectionFlags
{
	SHF_WRITE				= 0x1,
	SHF_ALLOC				= 0x2,
	SHF_EXECINSTR			= 0x4,
	SHF_MERGE				= 0x10,
	SHF_STRINGS				= 0x20,
	SHF_INFO_LINK			= 0x40,
	SHF_LINK_ORDER			= 0x80,
	SHF_OS_NONCONFORMING	= 0x100,
	SHF_GROUP				= 0x200,
	SHF_TLS					= 0x400,
	SHF_MASKOS				= 0x0FF00000,
	SHF_MASKPROC			= 0xF0000000,
	SHF_ORDERED				= 0x4000000,
	SHF_EXCLUDE				= 0x8000000
};

enum	ELF_SymbolVisibility
{
	STV_DEFAULT		= 0,
	STV_INTERNAL	= 1,
	STV_HIDDEN		= 2,
	STV_PROTECTED	= 3,
};

enum	ELF_SymbolBind
{
	STB_LOCAL		= 0,
	STB_GLOBAL		= 1,
	STB_WEAK		= 2,
	STB_NUM			= 3,
	STB_LOOS		= 10,
	STB_GNU_UNIQUE	= 10,
	STB_HIOS		= 12,
	STB_LOPROC		= 13,
	STB_HIPROC		= 15, 
};

enum	ELF_SymbolType
{
	STT_NOTYPE		= 0,
	STT_OBJECT		= 1,
	STT_FUNC		= 2,
	STT_SECTION		= 3,
	STT_FILE		= 4,
	STT_COMMON		= 5,
	STT_TLS			= 6,
	STT_NUM			= 7,
	STT_LOOS		= 10,
	STT_GNU_IFUNC	= 10,
	STT_HIOS		= 12,
	STT_LOPROC		= 13,
	STT_HIPROC		= 15,
};

enum	ELF_DynamicTag
{
	DT_NULL				= 0,
	DT_NEEDED			= 1,
	DT_PLTRELSZ			= 2,
	DT_PLTGOT			= 3,
	DT_HASH				= 4,
	DT_STRTAB			= 5,
	DT_SYMTAB			= 6,
	DT_RELA				= 7,
	DT_RELASZ			= 8,
	DT_RELAENT			= 9,
	DT_STRSZ			= 10,
	DT_SYMENT			= 11,
	DT_INIT				= 12,
	DT_FINI				= 13,
	DT_SONAME			= 14,
	DT_RPATH			= 15,
	DT_SYMBOLIC			= 16,
	DT_REL				= 17,
	DT_RELSZ			= 18,
	DT_RELENT			= 19,
	DT_PLTREL			= 20,
	DT_DEBUG			= 21,
	DT_TEXTREL			= 22,
	DT_JMPREL			= 23,
	DT_BIND_NOW			= 24,
	DT_INIT_ARRAY		= 25,
	DT_FINI_ARRAY		= 26,
	DT_INIT_ARRAYSZ		= 27,
	DT_FINI_ARRAYSZ		= 28,
	DT_RUNPATH			= 29,
	DT_FLAGS			= 30,
	DT_ENCODING			= 32,
	DT_PREINIT_ARRAY	= 32,
	DT_PREINIT_ARRAYSZ	= 33,
	DT_SYMTAB_SHNDX		= 34,
	DT_NUM				= 35,
	DT_LOOS				= 0x6000000d,
	DT_HIOS				= 0x6ffff000,
	DT_LOPROC			= 0x70000000,
	DT_HIPROC			= 0x7fffffff
};

# else

/**
 * @enum	ELF_IdentIndices
 */
#  define	ELF_IdentIndicesList(EXPAND)										\
	EXPAND(EI_MAG0,			0)													\
	EXPAND(EI_MAG1,			1)													\
	EXPAND(EI_MAG2,			2)													\
	EXPAND(EI_MAG3,			3)													\
	EXPAND(EI_CLASS,		4)													\
	EXPAND(EI_DATA,			5)													\
	EXPAND(EI_VERSION,		6)													\
	EXPAND(EI_OSABI,		7)													\
	EXPAND(EI_ABIVERSION,	8)													\

#  define	ENUM_TYPE	ELF_IdentIndices
#  define	ENUM_LIST	ELF_IdentIndicesList
#  include	"enumToString.h"

/**
 * @enum	ELF_Class
 */
#  define	ELF_ClassList(EXPAND)												\
	EXPAND(ELF_32BIT,	0x01)													\
	EXPAND(ELF_64BIT,	0x02)													\

#  define	ENUM_TYPE	ELF_Class
#  define	ENUM_LIST	ELF_ClassList
#  include	"enumToString.h"

/**
 * @enum	ELF_Endianness
 */
#  define	ELF_EndiannessList(EXPAND)											\
	EXPAND(ELF_DATANONE,	0x00)												\
	EXPAND(ELF_DATA2LSB,	0x01)												\
	EXPAND(ELF_DATA2MSB,	0x02)												\

#  define	ENUM_TYPE	ELF_Endianness
#  define	ENUM_LIST	ELF_EndiannessList
#  include	"enumToString.h"

/**
 * @enum	ELF_OsAbi
 */
#  define	ELF_OsAbiList(EXPAND)												\
	EXPAND(EABI_SYSV,			0x00)   										\
	EXPAND(EABI_HPUX,			0x01)   										\
	EXPAND(EABI_NETBSD,			0x02)   										\
	EXPAND(EABI_LINUX,			0x03)   										\
	EXPAND(EABI_HURD,			0x04)   										\
	EXPAND(EABI_SOLARIS,		0x06)   										\
	EXPAND(EABI_AIX,			0x07)   										\
	EXPAND(EABI_IRIX,			0x08)   										\
	EXPAND(EABI_FREEBSD,		0x09)   										\
	EXPAND(EABI_TRU64,			0x0A)   										\
	EXPAND(EABI_MODESTO,		0x0B)   										\
	EXPAND(EABI_OPENBSD,		0x0C)   										\
	EXPAND(EABI_OPENVMS,		0x0D)   										\
	EXPAND(EABI_NSK,			0x0E)   										\
	EXPAND(EABI_AROS,			0x0F)   										\
	EXPAND(EABI_FENIXOS,		0x10)   										\
	EXPAND(EABI_CLOUDABI,		0x11)   										\
	EXPAND(EABI_OPENVOS,		0x12)											\

#  define	ENUM_TYPE	ELF_OsAbi
#  define	ENUM_LIST	ELF_OsAbiList
#  include	"enumToString.h"

/**
 * @enum	ELF_ExType
 */
#  define	ELF_ExTypeList(EXPAND)												\
	EXPAND(ET_NONE,			0x00)												\
	EXPAND(ET_REL,			0x01)												\
	EXPAND(ET_EXEC,			0x02)												\
	EXPAND(ET_DYN,			0x03)												\
	EXPAND(ET_CORE,			0x04)												\
	EXPAND(ET_LOOS,			0xFE00)												\
	EXPAND(ET_HIOS,			0xFEFF)												\
	EXPAND(ET_LOPROC,   	0xFF00)												\
	EXPAND(ET_HIPROC,   	0xFFFF)												\

#  define	ELF_ExTypeSpec(EXPAND)												\
	EXPAND((ENUM_VAR >= ET_LOOS && ENUM_VAR <= ET_HIOS), "ET_OS_SPECIFIC")		\
	EXPAND((ENUM_VAR >= ET_LOPROC), "ET_PROCESSOR_SPECIFIC")					\

#  define	ENUM_TYPE	ELF_ExType
#  define	ENUM_LIST	ELF_ExTypeList
#  define	ENUM_SPEC	ELF_ExTypeSpec
#  include	"enumToString.h"

/**
 * @enum	ELF_Machine
 */
#  define	ELF_MachineList(EXPAND)												\
	EXPAND(EM_NONE,			0x00)												\
	EXPAND(EM_M32,			0x01)												\
	EXPAND(EM_SPARC,		0x02)												\
	EXPAND(EM_386,			0x03)												\
	EXPAND(EM_68K,			0x04)												\
	EXPAND(EM_88K,			0x05)												\
	EXPAND(EM_IAMCU,		0x06)												\
	EXPAND(EM_860,			0x07)												\
	EXPAND(EM_MIPS,			0x08)												\
	EXPAND(EM_ARM,			0x28)												\
	EXPAND(EM_X86_64,   	0x3E)												\
	EXPAND(EM_AARCH64,  	0xB7)												\
	EXPAND(EM_RISCV,		0xF3)												\

#  define	ENUM_TYPE	ELF_Machine
#  define	ENUM_LIST	ELF_MachineList
#  include	"enumToString.h"

/**
 * @enum	ELF_Version
 */
#  define	ELF_VersionList(EXPAND)												\
	EXPAND(EV_NONE,			0x00)												\
	EXPAND(EV_CURRENT,		0x01)												\
	EXPAND(EV_NUM,			0x02)												\

#  define	ENUM_TYPE	ELF_Version
#  define	ENUM_LIST	ELF_VersionList
#  include	"enumToString.h"

/**
 * @enum	ELF_ProgType
 */
#  define	ELF_ProgTypeList(EXPAND)											\
	EXPAND(PT_NULL,			0x00000000)											\
	EXPAND(PT_LOAD,			0x00000001)											\
	EXPAND(PT_DYNAMIC,		0x00000002)											\
	EXPAND(PT_INTERP,		0x00000003)											\
	EXPAND(PT_NOTE,			0x00000004)											\
	EXPAND(PT_SHLIB,		0x00000005)											\
	EXPAND(PT_PHDR,			0x00000006)											\
	EXPAND(PT_TLS,			0x00000007)											\
	EXPAND(PT_LOOS,			0x60000000)											\
	EXPAND(PT_GNU_EH_FRAME,	0x6474e550)											\
	EXPAND(PT_GNU_STACK,	0x6474e551)											\
	EXPAND(PT_GNU_RELRO,	0x6474e552)											\
	EXPAND(PT_GNU_PROPERTY,	0x6474e553)											\
	EXPAND(PT_HIOS,			0x6FFFFFFF)											\
	EXPAND(PT_LOPROC,		0x70000000)											\
	EXPAND(PT_HIPROC,		0x7FFFFFFF)											\

#  define	ELF_ProgTypeSpec(EXPAND)											\
	EXPAND(((ENUM_VAR >= PT_LOOS &&	ENUM_VAR < PT_GNU_EH_FRAME)	||				\
			(ENUM_VAR > PT_GNU_PROPERTY && ENUM_VAR <= PT_HIOS)					\
			), "PT_OS_SPECIFIC")												\
	EXPAND((ENUM_VAR >= PT_LOPROC && ENUM_VAR <= PT_HIPROC						\
			), "PT_PROCESSOR_SPECIFIC")											\

#  define	ENUM_TYPE	ELF_ProgType
#  define	ENUM_LIST	ELF_ProgTypeList
#  define	ENUM_SPEC	ELF_ProgTypeSpec
#  include	"enumToString.h"

/**
 * @enum	ELF_ProgFlags
 */
#  define	ELF_ProgFlagsList(EXPAND)											\
	EXPAND(PF_X,			(1<<0))												\
	EXPAND(PF_W,			(1<<1))												\
	EXPAND(PF_R,			(1<<2))												\
	EXPAND(PF_MASKOS,	0x0FF00000)												\
	EXPAND(PF_MASKPROC,	0xF0000000)												\

#  define	ENUM_TYPE	ELF_ProgFlags
#  define	ENUM_LIST	ELF_ProgFlagsList
#  include	"enumToString.h"

/**
 * @enum	ELF_SectionType
 */
#  define	ELF_SectionTypeList(EXPAND)											\
	EXPAND(SHT_NULL,			0x0)											\
	EXPAND(SHT_PROGBITS,		0x1)											\
	EXPAND(SHT_SYMTAB,			0x2)											\
	EXPAND(SHT_STRTAB,			0x3)											\
	EXPAND(SHT_RELA,			0x4)											\
	EXPAND(SHT_HASH,			0x5)											\
	EXPAND(SHT_DYNAMIC,			0x6)											\
	EXPAND(SHT_NOTE,			0x7)											\
	EXPAND(SHT_NOBITS,			0x8)											\
	EXPAND(SHT_REL,				0x9)											\
	EXPAND(SHT_SHLIB,			0x0A)											\
	EXPAND(SHT_DYNSYM,			0x0B)											\
	EXPAND(SHT_INIT_ARRAY,		0x0E)											\
	EXPAND(SHT_FINI_ARRAY,		0x0F)											\
	EXPAND(SHT_PREINIT_ARRAY,	0x10)											\
	EXPAND(SHT_GROUP,			0x11)											\
	EXPAND(SHT_SYMTAB_SHNDX,	0x12)											\
	EXPAND(SHT_NUM,				0x13)											\
	EXPAND(SHT_LOOS,			0x60000000)										\
	EXPAND(SHT_GNU_VERSYM,		0x6FFFFFF0)										\
	EXPAND(SHT_GNU_HASH,		0x6FFFFFF6)										\
	EXPAND(SHT_GNU_LIBLIST,		0x6FFFFFF7)										\
	EXPAND(SHT_GNU_VERDEF,		0x6FFFFFFD)										\
	EXPAND(SHT_GNU_VERNEED,		0x6FFFFFFE)										\
	EXPAND(SHT_HIOS,			0xFFFFFFFF)										\

#  define	ELF_SectionTypeSpec(EXPAND)											\
	EXPAND(((ENUM_VAR >= SHT_LOOS && ENUM_VAR < SHT_GNU_VERSYM) ||				\
			(ENUM_VAR > SHT_GNU_VERNEED && ENUM_VAR <= SHT_HIOS)				\
			), "SHT_OS_SPECIFIC")												\

#  define	ENUM_TYPE	ELF_SectionType
#  define	ENUM_LIST	ELF_SectionTypeList
#  define	ENUM_SPEC	ELF_SectionTypeSpec
#  include	"enumToString.h"

/**
 * @enum	ELF_SectionFlags
 */
#  define	ELF_SectionFlagsList(EXPAND)										\
	EXPAND(SHF_WRITE,				0x1)										\
	EXPAND(SHF_ALLOC,				0x2)										\
	EXPAND(SHF_EXECINSTR,			0x4)										\
	EXPAND(SHF_MERGE,				0x10)										\
	EXPAND(SHF_STRINGS,				0x20)										\
	EXPAND(SHF_INFO_LINK,			0x40)										\
	EXPAND(SHF_LINK_ORDER,			0x80)										\
	EXPAND(SHF_OS_NONCONFORMING,	0x100)										\
	EXPAND(SHF_GROUP,				0x200)										\
	EXPAND(SHF_TLS,					0x400)										\
	EXPAND(SHF_MASKOS,				0x0FF00000)									\
	EXPAND(SHF_MASKPROC,			0xF0000000)									\
	EXPAND(SHF_ORDERED,				0x4000000)									\
	EXPAND(SHF_EXCLUDE,				0x800000)									\

#  define	ENUM_TYPE	ELF_SectionFlags
#  define	ENUM_LIST	ELF_SectionFlagsList
#  include	"enumToString.h"

/**
 * @enum	ELF_SymbolVisibility
 */
#  define	ELF_SymbolVisibilityList(EXPAND)									\
	EXPAND(STV_DEFAULT,		0)													\
	EXPAND(STV_INTERNAL,	1)													\
	EXPAND(STV_HIDDEN,		2)													\
	EXPAND(STV_PROTECTED,   3)													\

#  define	ENUM_TYPE	ELF_SymbolVisibility
#  define	ENUM_LIST	ELF_SymbolVisibilityList
#  include	"enumToString.h"

/**
 * @enum	ELF_SymbolBind
 */
#  define	ELF_SymbolBindList(EXPAND)											\
	EXPAND(STB_LOCAL,		0)													\
	EXPAND(STB_GLOBAL,		1)													\
	EXPAND(STB_WEAK,		2)													\
	EXPAND(STB_NUM,			3)													\
	EXPAND(STB_LOOS,		10)													\
	EXPAND(STB_GNU_UNIQUE,  10)													\
	EXPAND(STB_HIOS,		12)													\
	EXPAND(STB_LOPROC,		13)													\
	EXPAND(STB_HIPROC,		15)													\

#  define	ENUM_TYPE	ELF_SymbolBind
#  define	ENUM_LIST	ELF_SymbolBindList
#  include	"enumToString.h"

/**
 * @enum	ELF_SymbolType
 */
#  define	ELF_SymbolTypeList(EXPAND)											\
	EXPAND(STT_NOTYPE,		0)													\
	EXPAND(STT_OBJECT,		1)													\
	EXPAND(STT_FUNC,		2)													\
	EXPAND(STT_SECTION,		3)													\
	EXPAND(STT_FILE,		4)													\
	EXPAND(STT_COMMON,		5)													\
	EXPAND(STT_TLS,			6)													\
	EXPAND(STT_NUM,			7)													\
	EXPAND(STT_GNU_IFUNC,   10)													\
	EXPAND(STT_LOOS,		10)													\
	EXPAND(STT_HIOS,		12)													\
	EXPAND(STT_LOPROC,		13)													\
	EXPAND(STT_HIPROC,		15)													\

#  define	ENUM_TYPE	ELF_SymbolType
#  define	ENUM_LIST	ELF_SymbolTypeList
#  include	"enumToString.h"

/**
 * @enum	ELF_SectionIndices
 */
#  define	ELF_SectionIndicesList(EXPAND)										\
	EXPAND(SHN_UNDEF,		0)													\
	EXPAND(SHN_LORESERVE,   0xFF00)												\
	EXPAND(SHN_LOPROC,		0xFF00)												\
	EXPAND(SHN_BEFORE,		0xFF00)												\
	EXPAND(SHN_AFTER,		0xFF01)												\
	EXPAND(SHN_HIPROC,		0xFF1F)												\
	EXPAND(SHN_ABS,			0xFFF1)												\
	EXPAND(SHN_COMMON,		0xFFF2)												\
	EXPAND(SHN_HIRESERVE,   0xFFFF)												\

#  define	ENUM_TYPE	ELF_SectionIndices
#  define	ENUM_LIST	ELF_SectionIndicesList
#  include	"enumToString.h"

/**
 * @enum	ELF_DynamicTag
 */

#  define	ELF_DynamicTagList(EXPAND)											\
	EXPAND(DT_NULL,				0)												\
	EXPAND(DT_NEEDED,			1)												\
	EXPAND(DT_PLTRELSZ,			2)												\
	EXPAND(DT_PLTGOT,			3)												\
	EXPAND(DT_HASH,				4)												\
	EXPAND(DT_STRTAB,			5)												\
	EXPAND(DT_SYMTAB,			6)												\
	EXPAND(DT_RELA,				7)												\
	EXPAND(DT_RELASZ,			8)												\
	EXPAND(DT_RELAENT,			9)												\
	EXPAND(DT_STRSZ,			10)												\
	EXPAND(DT_SYMENT,			11)												\
	EXPAND(DT_INIT,				12)												\
	EXPAND(DT_FINI,				13)												\
	EXPAND(DT_SONAME,			14)												\
	EXPAND(DT_RPATH,			15)												\
	EXPAND(DT_SYMBOLIC,			16)												\
	EXPAND(DT_REL,				17)												\
	EXPAND(DT_RELSZ,			18)												\
	EXPAND(DT_RELENT,			19)												\
	EXPAND(DT_PLTREL,			20)												\
	EXPAND(DT_DEBUG,			21)												\
	EXPAND(DT_TEXTREL,			22)												\
	EXPAND(DT_JMPREL,			23)												\
	EXPAND(DT_BIND_NOW,			24)												\
	EXPAND(DT_INIT_ARRAY,		25)												\
	EXPAND(DT_FINI_ARRAY,		26)												\
	EXPAND(DT_INIT_ARRAYSZ,		27)												\
	EXPAND(DT_FINI_ARRAYSZ,		28)												\
	EXPAND(DT_RUNPATH,			29)												\
	EXPAND(DT_FLAGS,			30)												\
	EXPAND(DT_ENCODING,			32)												\
	EXPAND(DT_PREINIT_ARRAY,	32)												\
	EXPAND(DT_PREINIT_ARRAYSZ,	33)												\
	EXPAND(DT_SYMTAB_SHNDX,		34)												\
	EXPAND(DT_NUM,				35)												\
	EXPAND(DT_LOOS,				0x6000000d)										\
	EXPAND(DT_HIOS,				0x6ffff000)										\
	EXPAND(DT_LOPROC,			0x70000000)										\
	EXPAND(DT_HIPROC,			0x7fffffff)										\

#  define	ELF_DynamicTagSpec(EXPAND)											\
	EXPAND((ENUM_VAR >= DT_LOOS && ENUM_VAR <= DT_HIOS), "OS_SPECIFIC")			\
	EXPAND((ENUM_VAR >= DT_LOPROC && ENUM_VAR <= DT_HIPROC), "PROC_SPECIFIC")	\

#  define ENUM_TYPE		ELF_DynamicTag
#  define ENUM_LIST		ELF_DynamicTagList
#  define ENUM_SPEC		ELF_DynamicTagSpec
#  include	"enumToString.h"

# endif	// _CELF_ENUMS_TO_STRINGS

#endif	// _CELF_ENUMS_H
