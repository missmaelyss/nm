/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marnaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/11 13:21:06 by marnaud           #+#    #+#             */
/*   Updated: 2019/09/11 13:52:04 by marnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <nm_otool.h>

char	*g_cpu_type_tab[18] =
{
	"vax", "", "", "", "", "mc680x0", "x86", "", "", "mc98000", "hppa", "arm",
	"mc88000", "sparc", "i860", "", "", "ppc"
};
char	g_section_symbol[30] = "";

void	pick_h(uint32_t mg, struct s_file_ptr *ptr, cpu_type_t c, char *av)
{
	if (mg == MH_MAGIC_64 || mg == MH_CIGAM_64)
		handle_64(ptr, (OSSwapConstInt32(c)
					& CPU_TYPE_POWERPC) == CPU_TYPE_POWERPC);
	if (mg == MH_MAGIC || mg == MH_CIGAM)
		handle_32(ptr, (OSSwapConstInt32(c)
					& CPU_TYPE_POWERPC) == CPU_TYPE_POWERPC);
	if (mg == FAT_MAGIC || mg == FAT_CIGAM)
		handle_fat_32(ptr, av, 0x213C617263683E0A);
	if (mg == FAT_MAGIC_64 || mg == FAT_CIGAM_64)
		handle_fat_64(ptr, av, 0x213C617263683E0A);
}

void	nm(struct s_file_ptr *ptr, char *av, uint8_t mult)
{
	uint32_t	magic_number;
	cpu_type_t	cputype;
	uint32_t	filetype;

	magic_number = *(uint32_t *)(ptr->ptr);
	cputype = (cpu_type_t)((ptr->ptr) + sizeof(uint32_t));
	filetype = *(uint32_t *)((ptr->ptr) + sizeof(uint32_t)
			+ sizeof(cpu_type_t) + sizeof(cpu_subtype_t));
	if (OSSwapConstInt64(*(uint64_t *)(ptr->ptr)) == 0x213C617263683E0A
			|| *(uint64_t *)(ptr->ptr) == 0x213C617263683E0A)
	{
		handle_lyb(ptr, av);
		return ;
	}
	filetype = ((magic_number == MH_CIGAM || magic_number == MH_CIGAM_64)
			? OSSwapConstInt32(filetype) : filetype);
	if (!(filetype == 0x0 || filetype == 0x1 || filetype == 0x2
		|| filetype == 0x6 || filetype == 0x3000080 || filetype == 0x3000000))
		return (ft_error(av, "The file is not a valid object file\n"));
	if (mult > 2 && magic_number != FAT_MAGIC && magic_number != FAT_MAGIC_64
			&& magic_number != FAT_CIGAM && magic_number != FAT_CIGAM_64)
		print_path(av);
	pick_h(magic_number, ptr, cputype, av);
}

void	ft_error(char *av, char *error)
{
	write(2, av, ft_strlen(av));
	write(2, ": ", 2);
	write(2, error, ft_strlen(error));
	write(2, "\n", 1);
}

int		check_file(int *fd, char *av, struct s_file_ptr	*ptr, struct stat *buf)
{
	if ((*fd = open(av, O_RDONLY)) < 0)
	{
		ft_error(av, "Permission denied.");
		return (0);
	}
	if (fstat(*fd, buf) < 0)
	{
		ft_error(av, "Permission denied.");
		close(*fd);
		return (0);
	}
	if ((ptr->ptr = mmap(0, buf->st_size, PROT_READ, MAP_PRIVATE, *fd, 0))
			== MAP_FAILED)
	{
		ft_error(av, "Is a directory.");
		close(*fd);
		return (0);
	}
	return (1);
}

int		main(int ac, char **av)
{
	int					fd;
	int					n;
	struct stat			buf;
	struct s_file_ptr	ptr;

	n = 0;
	if (ac < 2)
		n--;
	while (++n < ac)
	{
		if (!check_file(&fd, av[n], &ptr, &buf))
			continue;
		ptr.max = ptr.ptr + buf.st_size;
		nm(&ptr, av[n], ac);
		if (munmap(ptr.ptr, buf.st_size) < 0)
		{
			close(fd);
			continue;
		}
		close(fd);
	}
	return (EXIT_SUCCESS);
}
