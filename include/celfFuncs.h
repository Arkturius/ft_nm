/**
 * celfFuncs
 *
 *		ELF Functions header.
 */

// #define _CELF_LSP_FRIENDLY

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

#undef ELF_CLASS
