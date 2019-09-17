/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_otool.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marnaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/17 14:46:17 by marnaud           #+#    #+#             */
/*   Updated: 2019/09/17 14:46:20 by marnaud          ###   ########.fr       */
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
			otool(&(struct s_file_ptr){(void *)(h->ar_fmag) + 2 + (ft_strlen(&((
				h->ar_fmag)[2])) / 8 + (ft_strlen(&((h->ar_fmag)[2])) % 8 > 0 ?
					1 : 0)) * 0x8 + 0x4, ptr->max}, av, 0);
		}
		ptr->ptr = (void *)(h->ar_fmag) + 2 + atoi(h->ar_size);
		if (OSSwapConstInt64(*(uint64_t *)(ptr->ptr)) == 0x213C617263683E0A)
			break ;
		h = (struct ar_hdr *)(ptr->ptr);
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
			return ;
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
	struct mach_header		*header;
	struct load_command		*lc;

	header = (struct mach_header *)(ptr->ptr);
	ncmds = header->ncmds;
	lc = (ptr->ptr) + sizeof(struct mach_header);
	i = 0;
	while (i < swap_endian(ncmds, header->magic))
	{
		if ((void *)lc > ptr->max)
			return ;
		if (swap_endian(lc->cmd, header->magic) == LC_SEGMENT)
			section_32_otool(lc, ptr, header);
		lc = (void *)lc + swap_endian(lc->cmdsize, header->magic);
		i++;
	}
}

void		handle_fat_64_otool(struct s_file_ptr *p, char *av, uint64_t s)
{
	uint32_t			i;
	struct fat_header	*h;
	struct fat_arch_64	*a;

	i = 0;
	h = (struct fat_header *)(p->ptr);
	a = p->ptr + sizeof(struct fat_header);
	while (i < si32(h->nfat_arch))
	{
		if (!print_fat_64(p, av, h, a))
			return ;
		if (si64(*(uint64_t *)(p->ptr + si32(a->offset))) == s
				|| *(uint64_t *)(p->ptr + si32(a->offset)) == s)
			handle_lyb_otool(&(t_file_ptr){p->ptr
				+ si32(a->offset), p->max}, av);
		if ((si32(a->cputype) & CPU_ARCH_ABI64) == CPU_ARCH_ABI64)
			handle_64_otool(&(t_file_ptr){p->ptr + si32(a->offset), p->max},
				(si32(a->cputype) & CPU_TYPE_POWERPC) == CPU_TYPE_POWERPC, av);
		else
			handle_32_otool(&(t_file_ptr){p->ptr + si32(a->offset), p->max},
				(si32(a->cputype) & CPU_TYPE_POWERPC) == CPU_TYPE_POWERPC, av);
		i++;
		a++;
	}
}

void		handle_fat_32_otool(struct s_file_ptr *p, char *av, uint64_t s)
{
	uint32_t			i;
	struct fat_header	*h;
	struct fat_arch		*a;

	i = 0;
	h = (struct fat_header *)(p->ptr);
	a = p->ptr + sizeof(struct fat_header);
	while (i < si32(h->nfat_arch))
	{
		if (!print_fat_32(p, av, h, a))
			return ;
		if (si64(*(uint64_t *)(p->ptr + si32(a->offset))) == s
				|| *(uint64_t *)(p->ptr + si32(a->offset)) == s)
			handle_lyb_otool(&(t_file_ptr){p->ptr
				+ si32(a->offset), p->max}, av);
		if ((si32(a->cputype) & CPU_ARCH_ABI64) == CPU_ARCH_ABI64)
			handle_64_otool(&(t_file_ptr){p->ptr + si32(a->offset), p->max},
				(si32(a->cputype) & CPU_TYPE_POWERPC) == CPU_TYPE_POWERPC, av);
		else
			handle_32_otool(&(t_file_ptr){p->ptr + si32(a->offset), p->max},
				(si32(a->cputype) & CPU_TYPE_POWERPC) == CPU_TYPE_POWERPC, av);
		i++;
		a++;
	}
}
