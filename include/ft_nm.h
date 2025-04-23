/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 15:33:12 by rgramati          #+#    #+#             */
/*   Updated: 2025/04/23 04:53:10 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NM_H
# define FT_NM_H

# include <stdlib.h>
# include <stdint.h>
# include <unistd.h>

# define	_CELF_IMPLEMENTATION
# include <celf.h>

typedef struct	s_nm_core
{
	const char		*exe_name;
	char			**files;
	uint32_t		nfiles;
	uint32_t		opts;
}	nm_core;

/*
# define	NM_CORE			_nm_core
# define	NM_CONTEXT		NM_CORE.context
# define	NM_OPTIONS		_nm_options
# define	NM_COMPARATORS	_nm_comp_funcs

# define	NM_FILE_OFF(o)	(NM_CONTEXT.file.raw + o)

static nm_core	NM_CORE = {0};
*/

INLINE uint32_t	nm_strlen(const char *str)
{
	const char	*tmp = str;

	while (*tmp)
		tmp++;
	return (tmp - str);
}

INLINE int32_t	nm_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}

INLINE void	*nm_memset(void *dst, int c, uint32_t n)
{
	uint8_t	*dst_c = dst;

	while (n)
	{
		*dst_c = c;
		dst_c++;
		n--;
	}
	return (dst);
}

INLINE void	*nm_memcpy(void *dst, const void *src, uint32_t n)
{
	uint8_t			*dst_c = dst;
	const uint8_t	*src_c = (uint8_t *)src;

	while (n)
	{
		*dst_c = *src_c;
		dst_c++;
		src_c++;
		n--;
	}
	return (dst);
}

INLINE void	nm_memswap(void *a, void *b, uint32_t n)
{
	static uint8_t	swap_buf[128];
	uint8_t			*buffer;

	buffer = swap_buf;
	if (n > sizeof(swap_buf))
		buffer = malloc(n);
	if (!buffer)
		return ;
	
	nm_memcpy(swap_buf, a, n);
	nm_memcpy(a, b, n);
	nm_memcpy(b, swap_buf, n);
}

INLINE const char	*nm_xtoa(uint64_t n, int len)
{
	static char	hex[16] = "0123456789abcdef";
	static char	buffer[17] = {0};
	int32_t		cursor;

	cursor = len;
	buffer[cursor--] = 0;

	while (n)
	{
		buffer[cursor--] = hex[n & 0xF];
		n >>= 4;
	}
	while (cursor >= 0)
		buffer[cursor--] = '0';
	return (buffer);
}

enum	e_nm_options
{
	NM_ALL		= 0,
	NM_EXTERN	= 1,
	NM_NOSORT	= 2,
	NM_REVSORT	= 3,
	NM_UNDEF	= 4,
	NM_USAGE	= 5,
	NM_LASTOPT	= 6, // Used to iterate over the options.
};

# define	NM_OPT(opt)		(NM_CORE.opts & (1 << opt))

static const struct { char opletter; const char *msg; } NM_OPTIONS[] = 
{
	[NM_ALL]		= { 'a', "Display debugger-only symbols."	},
	[NM_EXTERN]		= { 'g', "Display only external symbols."	},
	[NM_NOSORT]		= { 'p', "Do not sort the symbols."			},
	[NM_REVSORT]	= { 'r', "Reverse the sense of the sort."	},
	[NM_UNDEF]		= { 'u', "Display only undefined symbols."	},
	[NM_USAGE]		= { 'h', "Display this help message."		}
};

INLINE const char	*nm_shiftArgs(char ***argv_ptr)
{
	if (!argv_ptr)
		return (NULL);

	return (*((*argv_ptr)++));
}

INLINE void	nm_usage(int fd)
{
	write(fd, "Usage: ", 7);
	write(fd, NM_CORE.exe_name, nm_strlen(NM_CORE.exe_name));
	write(fd, " [option(s)] [file(s)]\n", 23);
	write(fd, " List symbols in [file(s)] (a.out by default)\n", 46);

	for (uint32_t i = 0; i < NM_LASTOPT; ++i)
	{
		const char	*msg = NM_OPTIONS[i].msg;

		write(fd, "  -", 3);
		write(fd, &NM_OPTIONS[i].opletter, 1);
		write(fd, "    ", 4);
		write(fd, msg, nm_strlen(msg));
		write(fd, "\n", 1);
	}
}

INLINE void	nm_error(const char *msg, int code)
{
	uint32_t	msg_len;

	if (code)
	{
		msg_len = nm_strlen(msg);

		write(STDERR_FILENO, NM_CORE.exe_name, nm_strlen(NM_CORE.exe_name));
		write(STDERR_FILENO, ": Error: ", 9);
		write(STDERR_FILENO, msg, msg_len);
		write(STDERR_FILENO, "\n", 1);
	}
	ELF_close(&NM_CONTEXT.file);
	exit(code);
}

INLINE void	nm_filename(const char *filename)
{
	write(STDOUT_FILENO, "\n", 1);
	write(STDOUT_FILENO, filename, nm_strlen(filename));
	write(STDOUT_FILENO, ":\n", 2);
}

INLINE char	**nm_parseOptions(int *argc, char **argv)
{
	char		**files = NULL;
	uint32_t	file_idx = 0;

	files = malloc(*argc * sizeof(char *));
	while (files && *argv)
	{
		char	*arg = *argv;

		if (arg[0] == '-')
		{
			for (uint32_t j = 0; j < nm_strlen(arg + 1); j++)
			{
				uint32_t i = 0;
				for (; i < NM_LASTOPT; ++i)
				{
					if (arg[1] == NM_OPTIONS[i].opletter)
					{
						NM_CORE.opts |= (1 << i);
						break ;
					}
				}
				if (i == NM_LASTOPT)
				{
					free(files);
					files = NULL;
					break ;
				}
			}
		}
		else
			files[file_idx++] = arg;
		nm_shiftArgs(&argv);
	}
	*argc = file_idx;
	return (files);
}

#include <string.h>
#include <stdio.h>

// #define NM_LSIMP

#if !defined(NM_LSIMP)

# define NM_ELF_CLASS	32
# include <nm_functions.h>

# define NM_ELF_CLASS	64
# include <nm_functions.h>

#endif

int	nm_process(const char *fp)
{
	nm_memset(&NM_CONTEXT, 0, sizeof(NM_CONTEXT));

	Error	err = ELF_open(&NM_CONTEXT.file, fp);

	if (err == ERR_FILE)
		nm_error("No such file", 1);
	if (err == ERR_ALLOC)
		nm_error("File mapping failed.", 3);

	NM_CONTEXT.id = *(ELF_Ident *)(NM_CONTEXT.file.raw);

	if (NM_CONTEXT.id.e_magic[EI_MAG0] != '\x7f'	||
		NM_CONTEXT.id.e_magic[EI_MAG1] != 'E'		||
		NM_CONTEXT.id.e_magic[EI_MAG2] != 'L'		||
		NM_CONTEXT.id.e_magic[EI_MAG3] != 'F'
	) { nm_error("Not a valid ELF file.", 2); }
	
	if (NM_IS_32)
	{
		nm32_findSymbolTable();
		nm32_processSymbols();
		nm32_displaySymbols();
	}
	else if (NM_IS_64)
	{
		nm64_findSymbolTable();
		nm64_processSymbols();
		nm64_displaySymbols();
	}
	else
		nm_error("Invalid file class.", 4);

	ELF_close(&NM_CONTEXT.file);
	return (0);
}

#endif	// FT_NM_H
