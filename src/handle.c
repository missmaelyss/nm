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

void		handle_lyb(struct s_file_ptr *ptr, char *av)
{
	struct ar_hdr	*h;

	h = (struct ar_hdr *)(ptr->ptr + 0x8);
	while (atoi(h->ar_size))
	{
		if ((void *)h > ptr->max)
		{
			printf("Error\n");
			return ;
		}
		if (ft_strcmp(h->ar_fmag, "`\n__.SYMDEF SORTED") != 0
				&& ft_strcmp(h->ar_fmag, "`\n__.SYMDEF"))
		{
			print_lyb(av, h);
			nm(&(struct s_file_ptr){(void *)(h->ar_fmag) + 2 + (ft_strlen(&((h->
			ar_fmag)[2])) / 8 + (ft_strlen(&((h->ar_fmag)[2])) % 8 > 0 ? 1 : 0))
			* 0x8 + 0x4, ptr->max}, av, 1);
		}
		ptr->ptr = (void *)(h->ar_fmag) + 2 + atoi(h->ar_size);
		if (OSSwapConstInt64(*(uint64_t *)(ptr->ptr)) == 0x213C617263683E0A)
			break ;
		h = (struct ar_hdr *)(ptr->ptr);
	}
}

void		handle_64(struct s_file_ptr *ptr, uint8_t ppc)
{
	uint32_t				ncmds;
	uint32_t				i;
	struct mach_header_64	*header;
	struct load_command		*lc;
	struct symtab_command	*sym;

	fill_section_64(ptr, ppc);
	header = (struct mach_header_64 *)(ptr->ptr);
	ncmds = header->ncmds;
	lc = ptr->ptr + sizeof(struct mach_header_64);
	i = 0;
	while (i < swap_endian(ncmds, header->magic))
	{
		if ((void *)lc > ptr->max)
			return (ft_error("ft_nm", "Error\n"));
		if (swap_endian(lc->cmd, header->magic) == LC_SYMTAB)
		{
			sym = (struct symtab_command *)lc;
			print_output_64(sym, ptr, header->magic, ppc);
			break ;
		}
		lc = (void *)lc + lc->cmdsize;
		i++;
	}
}

void		handle_32(struct s_file_ptr *ptr, uint8_t ppc)
{
	uint32_t				ncmds;
	uint32_t				i;
	struct mach_header		*header;
	struct load_command		*lc;
	struct symtab_command	*sym;

	fill_section_32(ptr, ppc);
	header = (struct mach_header *)(ptr->ptr);
	ncmds = header->ncmds;
	lc = ptr->ptr + sizeof(struct mach_header);
	i = 0;
	while (i < swap_endian(ncmds, header->magic))
	{
		if ((void *)lc > ptr->max)
			return (ft_error("ft_nm", "Error\n"));
		if (swap_endian(lc->cmd, header->magic) == LC_SYMTAB)
		{
			sym = (struct symtab_command *)lc;
			print_output_32(sym, ptr, header->magic, ppc);
			break ;
		}
		lc = (void *)lc + swap_endian(lc->cmdsize, header->magic);
		i++;
	}
}

void		handle_fat_64(struct s_file_ptr *p, char *av, uint64_t s)
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
			handle_lyb(&(t_file_ptr){p->ptr + si32(arch->offset), p->max}, av);
		if ((si32(arch->cputype) & CPU_ARCH_ABI64) == CPU_ARCH_ABI64)
			handle_64(&(t_file_ptr){p->ptr + si32(arch->offset), p->max},
			(si32(arch->cputype) & CPU_TYPE_POWERPC) == CPU_TYPE_POWERPC);
		else
			handle_32(&(t_file_ptr){p->ptr + si32(arch->offset), p->max},
			(si32(arch->cputype) & CPU_TYPE_POWERPC) == CPU_TYPE_POWERPC);
		i++;
		arch++;
	}
}

void		handle_fat_32(struct s_file_ptr *p, char *av, uint64_t s)
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
			handle_lyb(&(t_file_ptr){p->ptr + si32(arch->offset), p->max}, av);
		if ((si32(arch->cputype) & CPU_ARCH_ABI64) == CPU_ARCH_ABI64)
			handle_64(&(t_file_ptr){p->ptr + si32(arch->offset), p->max},
			(si32(arch->cputype) & CPU_TYPE_POWERPC) == CPU_TYPE_POWERPC);
		else
			handle_32(&(t_file_ptr){p->ptr + si32(arch->offset), p->max},
			(si32(arch->cputype) & CPU_TYPE_POWERPC) == CPU_TYPE_POWERPC);
		i++;
		arch++;
	}
}
