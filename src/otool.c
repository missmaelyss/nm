#include <nm_otool.h>

char	*g_cpu_type_tab[18] =
{
	"vax", "", "", "", "", "mc680x0", "x86", "", "", "mc98000", "hppa", "arm",
	"mc88000", "sparc", "i860", "", "", "ppc"
};

void	pick_h_otool(uint32_t mg, struct s_file_ptr *ptr, cpu_type_t c, char *av)
{
	if (mg == MH_MAGIC_64 || mg == MH_CIGAM_64)
		handle_64_otool(ptr, (OSSwapConstInt32(c)
					& CPU_TYPE_POWERPC) == CPU_TYPE_POWERPC, av);
	if (mg == MH_MAGIC || mg == MH_CIGAM)
		handle_32_otool(ptr, (OSSwapConstInt32(c)
					& CPU_TYPE_POWERPC) == CPU_TYPE_POWERPC, av);
	if (mg == FAT_MAGIC || mg == FAT_CIGAM)
		handle_fat_32_otool(ptr, av, 0x213C617263683E0A);
	if (mg == FAT_MAGIC_64 || mg == FAT_CIGAM_64)
		handle_fat_64_otool(ptr, av, 0x213C617263683E0A);
}

void	otool(struct s_file_ptr *ptr, char *av, uint8_t mult)
{
	uint32_t	mg;
	cpu_type_t	cputype;
	
	mg = *(uint32_t *)(ptr->ptr);
	cputype = (cpu_type_t)((ptr->ptr) + sizeof(uint32_t));
	if (OSSwapConstInt64(*(uint64_t *)(ptr->ptr)) == 0x213C617263683E0A
			|| *(uint64_t *)(ptr->ptr) == 0x213C617263683E0A)
		handle_lyb_otool(ptr, av);
	else
	{
		if (mult && !(mg == FAT_MAGIC || mg == FAT_CIGAM || mg == FAT_MAGIC_64 || mg == FAT_CIGAM_64))
			print_path(av, 0);
		pick_h_otool(mg, ptr, cputype, av);
	}
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
			// return (EXIT_FAILURE);
			continue;
		ptr.max = ptr.ptr + buf.st_size;
		otool(&ptr, av[n], ac);
		if (munmap(ptr.ptr, buf.st_size) < 0)
		{
			close(fd);
			continue;
		}
		close(fd);
	}
	return (EXIT_SUCCESS);
}