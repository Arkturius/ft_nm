/**
 *	nm.c
 * -------------------------------------------------------------------------- */

#include <nm.h>

U_FLAGS_GEN_VARS(nm);
U_FLAGS_GEN_FUNCS(nm, NM_FLAGS, "List symbols in [file(s)] (a.out by default).")

int	nm_fatal
(const char *error)
{
	u_dprintf(2, "%s: %s\n", nm_exe, error);
	exit(1);
	return (1);
}

int	nm
(const char *file)
{
	if (ELF_open(file))
	{
		u_dprintf(2, "%s: '%s': No such file\n", nm_exe, file);
		return (1);
	}
	if (ELF_check())
	{
		u_dprintf(2, "%s: %s: file format not recognized\n", nm_exe, file);
		return (1);
	}

	nm_SYMTAB	*symbols = nm_symtab_init();
	if (!symbols)
		nm_fatal("malloc fail.");
	
	nm_symbols_extract(symbols);
	if (!symbols->count)
	{
		free(symbols);
		u_dprintf(2, "%s: %s: no symbols\n", nm_exe, file);
		return (1);
	}

	nm_symbols_display(file, symbols);
	free(symbols);

	ELF_close();
	return (0);
}

int main
(int argc, const char **argv)
{
	int			ret = 0;
	const char	**arge = NULL;

	nm_flags_parse(argc, argv, &arge);

	if (nm_flags_check())
		nm_usage(1);

	if (arge - argv > 2)
		nm_flags_set(NM_FILENAME_PER_FILE);
	
	if (argc == 1)
		ret = nm("a.out");
	else
	{
		const char	**file;

		for (file = argv + 1; *file; ++file)
			ret += nm(*file);
	}

	return (ret);
}

