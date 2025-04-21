/**
 *	nm.c
 *
 *		ft_nm main function.
 */

#include <ft_nm.h>

int	main(int argc, char **argv, UNUSED char **envp)
{
	NM_CORE.exe_name	= nm_shiftArgs(&argv);
	NM_CORE.files		= nm_parseOptions(&argc, argv);
	NM_CORE.nfiles		= argc;

	if (!NM_CORE.files)
	{
		nm_usage(STDERR_FILENO);
		return (1);
	}

	char	**files = NM_CORE.files;

	if (NM_OPT(NM_USAGE))
	{
		nm_usage(STDOUT_FILENO);
		return (0);
	}

	if (!*(NM_CORE.files))
		nm_process("a.out");
	else
	{
		while (*(NM_CORE.files))
		{
			const char	*filename = *(NM_CORE.files);

			if (NM_CORE.nfiles > 1)
				nm_filename(filename);
			nm_process(filename);
			nm_shiftArgs(&NM_CORE.files);
		}
	}
	free(files);
	return (0);
}
