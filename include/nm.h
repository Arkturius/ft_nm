/**
 *	nm.h
 * -------------------------------------------------------------------------- */

#ifndef _NM_H
# define _NM_H

# include <celf.h>

# include <u_flags.h>
# include <u_algos.h>

# include <nm_symbols.h>

inline int	nm_fatal
(const char *error);

inline int	nm_file_error
(const char *error, const char *file);

# define	NM_FLAGS(X)														\
	X(																		\
		NM_DISP_DEBUG, U_NONE, 'a', "debug-syms",							\
		"Display debugger-only symbols"										\
	)																		\
	X(																		\
		NM_DISP_EXTRN, U_NONE, 'g', "extern-only",							\
		"Display only external symbols"										\
	)																		\
	X(																		\
		NM_SORT_NONE, U_NONE, 'p', "no-sort",								\
		"Do not sort the symbols"											\
	)																		\
	X(																		\
		NM_SORT_REVERSE, U_NONE, 'r', "reverse-sort",						\
		"Reverse the sense of the sort"										\
	)																		\
	X(																		\
		NM_DISP_UNDEF, U_NONE, 'u', "undefined-only",						\
		"Display only undefined symbols"									\
	)

U_FLAGS_GEN_ENUM(nm, NM_FLAGS, NM_FILENAME_PER_FILE, NM_FILENAME_PER_SYM);

U_FLAGS_GEN_VARS_DECLS(nm);
U_FLAGS_GEN_FUNCS_DECLS(nm);

#endif // _NM_H
