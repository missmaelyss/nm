/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marnaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/11 18:12:41 by marnaud           #+#    #+#             */
/*   Updated: 2019/09/11 19:00:36 by marnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <nm_otool.h>

void		handle_lyb_otool(struct s_file_ptr *ptr, char *av)
{
	struct ar_hdr	*h;

	h = (struct ar_hdr *)(ptr->ptr + 0x8);
    write(1, "Archive : ", 10);
    write(1, av, ft_strlen(av));
	write(1, "\n", 1);
	while (atoi(h->ar_size))
	{
		if ((void *)h > ptr->max)
			return ;
		if (ft_strcmp(h->ar_fmag, "`\n__.SYMDEF SORTED") != 0
				&& ft_strcmp(h->ar_fmag, "`\n__.SYMDEF"))
		{
            print_lyb(av, h, 0);
			otool(&(struct s_file_ptr){(void *)(h->ar_fmag) + 2 + (ft_strlen(&((h->
			ar_fmag)[2])) / 8 + (ft_strlen(&((h->ar_fmag)[2])) % 8 > 0 ? 1 : 0))
			* 0x8 + 0x4, ptr->max}, av, 0);
		}
		ptr->ptr = (void *)(h->ar_fmag) + 2 + atoi(h->ar_size);
		if (OSSwapConstInt64(*(uint64_t *)(ptr->ptr)) == 0x213C617263683E0A)
			break ;
		h = (struct ar_hdr *)(ptr->ptr);
	}
}

int    find_hexa(uint8_t nb, cpu_type_t cputype, int n, char *buffer)
{
    const char alpha[16] = "0123456789abcdef";

	buffer[0] = alpha[nb >> 4];
	buffer[1] = alpha[nb & 0x0f];
	if (OSSwapConstInt32(cputype) != CPU_TYPE_POWERPC || !((n + 1) % 4))
	{
    	buffer[2] = ' ';
		return (3);
	}
	return (2);
}

void	find_value(uint64_t n_value, int size, uint32_t n_type, char *buffer)
{
	// char		*str;
	int			n;
	const char	alpha[16] = "0123456789abcdef";

	// if (!(str = (char *)malloc(sizeof(char) * (size + 1))))
		// return (ft_error("malloc", "Error."));
	n = size - 1;
	// buffer[size] = '\0';
	while (n >= 0)
	{
		buffer[n] = alpha[n_value % 16];
		if ((size == 16 && n_type == 1) || (size == 8 && n_type == 0x1))
			buffer[n] = ' ';
		n_value = n_value / 16;
		n--;
	}
	// write(1, buffer, size);
	// return(buffer);
}

void	section_64_otool(struct load_command *lc, t_file_ptr *ptr,
		struct mach_header_64 *header)
{
	struct segment_command_64	*seg;
	struct section_64			*sect;
	uint32_t					n;

	seg = (void *)lc;
	n = 0;
	sect = (void *)seg + sizeof(struct segment_command_64);
	while (n < seg->nsects)
	{
		if ((void *)sect > ptr->max)
		{
			// printf("Error\n");
			return ;
		}
        if (ft_strcmp(sect->sectname, "__text") == 0)
        {
			int size;
            int i;
            int i_b;
			char						*buffer;

			size = sect->size * 4 + (sect->size / 16 + 1) * 16;
			buffer = (char *)malloc(sizeof(char) * size);
            write(1, "Contents of (__TEXT,__text) section\n", 36);
            i = 0;
			i_b = 0;
            while (i < sect->size)
            {
                if (!(i % 16))
				{
                    find_value(sect->addr + i, 16, 0, &buffer[i_b]);
					buffer[i_b + 16] = '\t';
					i_b += 16 + 1;
				}
				i_b += find_hexa(*(uint8_t *)(ptr->ptr + swap_endian(sect->offset, header->magic) + i), header->cputype, i, &buffer[i_b]);
                i += 1;
                if (!(i % 16) || i == sect->size)
                {
					buffer[i_b] = '\n';
					i_b++;
				}
            }
			write(1, buffer, i_b);
			free(buffer);
        }
		n++;
		sect++;
	}
}

void	section_32_otool(struct load_command *lc, t_file_ptr *ptr,
		struct mach_header *header)
{
	struct segment_command	*seg;
	struct section			*sect;
	uint32_t					n;

	seg = (void *)lc;
	n = 0;
	sect = (void *)seg + sizeof(struct segment_command);
	while (n < seg->nsects)
	{
		if ((void *)sect > ptr->max)
		{
			// printf("Error\n");
			return ;
		}
		if (ft_strcmp(sect->sectname, "__text") == 0)
        {
			int size;
            int i;
            int i_b;
			char						*buffer;

			size = swap_endian(sect->size, header->magic) * 4 + (swap_endian(sect->size, header->magic) / 16 + 1) * 8;
			buffer = (char *)malloc(sizeof(char) * size);
            write(1, "Contents of (__TEXT,__text) section\n", 36);
            i = 0;
			i_b = 0;
            while (i < swap_endian(sect->size, header->magic))
            {
                if (!(i % 16))
				{
                    find_value(swap_endian(sect->addr, header->magic) + i, 8, 0, &buffer[i_b]);
					buffer[i_b + 8] = '\t';
					i_b += 8 + 1;
				}
				i_b += find_hexa(*(uint8_t *)(ptr->ptr + swap_endian(sect->offset, header->magic) + i), header->cputype, i, &buffer[i_b]);
                i += 1;
                if (!(i % 16) || i == swap_endian(sect->size, header->magic))
                {
					buffer[i_b] = '\n';
					i_b++;
				}
            }
			write(1, buffer, i_b);
			free(buffer);
        }
		n++;
		sect++;
	}
}

void		handle_64_otool(struct s_file_ptr *ptr, uint8_t ppc, char *av)
{
    uint32_t				ncmds;
	uint32_t				i;
	struct mach_header_64	*header;
	struct load_command		*lc;

	header = (struct mach_header_64 *)(ptr->ptr);
	ncmds = header->ncmds;
	lc = (ptr->ptr) + sizeof(struct mach_header_64);
	i = 0;
	while (i < swap_endian(ncmds, header->magic))
	{
		if ((void *)lc > ptr->max)
		{
			// printf("Error\n");
			return ;
		}
		if (swap_endian(lc->cmd, header->magic) == LC_SEGMENT_64)
            section_64_otool(lc, ptr, header);
        lc = (void *)lc + swap_endian(lc->cmdsize, header->magic);
		i++;
	}
}

void		handle_32_otool(struct s_file_ptr *ptr, uint8_t ppc, char *av)
{
    uint32_t				ncmds;
	uint32_t				i;
	struct mach_header	*header;
	struct load_command		*lc;

	header = (struct mach_header *)(ptr->ptr);
	ncmds = header->ncmds;
	lc = (ptr->ptr) + sizeof(struct mach_header);
	i = 0;
	while (i < swap_endian(ncmds, header->magic))
	{
		if ((void *)lc > ptr->max)
		{
			// printf("Error\n");
			return ;
		}
		if (swap_endian(lc->cmd, header->magic) == LC_SEGMENT)
            section_32_otool(lc, ptr, header);
        lc = (void *)lc + swap_endian(lc->cmdsize, header->magic);
		i++;
	}
}


void		handle_fat_64_otool(struct s_file_ptr *p, char *av, uint64_t s)
{
	uint32_t			i;
	struct fat_header	*header;
	struct fat_arch_64	*arch;

	i = 0;
	header = (struct fat_header *)(p->ptr);
	arch = p->ptr + sizeof(struct fat_header);
	while (i < si32(header->nfat_arch))
	{
		if (!print_fat_64(p, av, header, arch))
			return ;
		if (si64(*(uint64_t *)(p->ptr + si32(arch->offset))) == s
		|| *(uint64_t *)(p->ptr + si32(arch->offset)) == s)
			handle_lyb_otool(&(t_file_ptr){p->ptr + si32(arch->offset), p->max}, av);
		if ((si32(arch->cputype) & CPU_ARCH_ABI64) == CPU_ARCH_ABI64)
			handle_64_otool(&(t_file_ptr){p->ptr + si32(arch->offset), p->max},
			(si32(arch->cputype) & CPU_TYPE_POWERPC) == CPU_TYPE_POWERPC, av);
		else
			handle_32_otool(&(t_file_ptr){p->ptr + si32(arch->offset), p->max},
			(si32(arch->cputype) & CPU_TYPE_POWERPC) == CPU_TYPE_POWERPC, av);
		i++;
		arch++;
	}
}

void		handle_fat_32_otool(struct s_file_ptr *p, char *av, uint64_t s)
{
	uint32_t			i;
	struct fat_header	*header;
	struct fat_arch		*arch;

	i = 0;
	header = (struct fat_header *)(p->ptr);
	arch = p->ptr + sizeof(struct fat_header);
	while (i < si32(header->nfat_arch))
	{
		if (!print_fat_32(p, av, header, arch))
			return ;
		if (si64(*(uint64_t *)(p->ptr + si32(arch->offset))) == s
		|| *(uint64_t *)(p->ptr + si32(arch->offset)) == s)
			handle_lyb_otool(&(t_file_ptr){p->ptr + si32(arch->offset), p->max}, av);
		if ((si32(arch->cputype) & CPU_ARCH_ABI64) == CPU_ARCH_ABI64)
			handle_64_otool(&(t_file_ptr){p->ptr + si32(arch->offset), p->max},
			(si32(arch->cputype) & CPU_TYPE_POWERPC) == CPU_TYPE_POWERPC, av);
		else
			handle_32_otool(&(t_file_ptr){p->ptr + si32(arch->offset), p->max},
			(si32(arch->cputype) & CPU_TYPE_POWERPC) == CPU_TYPE_POWERPC, av);
		i++;
		arch++;
	}
}
