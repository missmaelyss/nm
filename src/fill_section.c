/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_section.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marnaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/11 13:58:23 by marnaud           #+#    #+#             */
/*   Updated: 2019/09/11 14:04:26 by marnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <nm_otool.h>

void	modify_sec_sym(char *name, int *i_tab)
{
	if (ft_strcmp(name, "__text") == 0)
	{
		g_section_symbol[(*i_tab)] = 't';
		(*i_tab)++;
	}
	else if (ft_strcmp(name, "__data") == 0)
	{
		g_section_symbol[(*i_tab)] = 'd';
		(*i_tab)++;
	}
	else if (ft_strcmp(name, "__bss") == 0)
	{
		g_section_symbol[(*i_tab)] = 'b';
		(*i_tab)++;
	}
	else
	{
		g_section_symbol[(*i_tab)] = 's';
		(*i_tab)++;
	}
}

void	section_32(struct load_command *lc, t_file_ptr *ptr,
		struct mach_header *header, int *i_tab)
{
	struct segment_command	*seg;
	struct section			*sect;
	uint32_t				n;

	seg = (void *)lc;
	n = 0;
	sect = (void *)seg + sizeof(struct segment_command);
	while (n < swap_endian(seg->nsects, header->magic))
	{
		if ((void *)sect > ptr->max)
		{
			printf("Error\n");
			return ;
		}
		modify_sec_sym(sect->sectname, i_tab);
		n++;
		sect++;
	}
}

void	section_64(struct load_command *lc, t_file_ptr *ptr,
		struct mach_header_64 *header, int *i_tab)
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
			printf("Error\n");
			return ;
		}
		modify_sec_sym(sect->sectname, i_tab);
		n++;
		sect++;
	}
}

void	fill_section_32(struct s_file_ptr *ptr, uint8_t ppc)
{
	uint32_t				ncmds;
	uint32_t				i;
	struct mach_header		*header;
	struct load_command		*lc;
	int						i_tab;

	header = (struct mach_header *)(ptr->ptr);
	ncmds = header->ncmds;
	lc = (ptr->ptr) + sizeof(struct mach_header);
	i = 0;
	i_tab = 0;
	while (i < swap_endian(ncmds, header->magic))
	{
		if ((void *)lc > ptr->max)
		{
			printf("Error\n");
			return ;
		}
		if (swap_endian(lc->cmd, header->magic) == LC_SEGMENT)
			section_32(lc, ptr, header, &i_tab);
		lc = (void *)lc + swap_endian(lc->cmdsize, header->magic);
		i++;
	}
}

void	fill_section_64(struct s_file_ptr *ptr, uint8_t ppc)
{
	uint32_t				ncmds;
	uint32_t				i;
	struct mach_header_64	*header;
	struct load_command		*lc;
	int						i_tab;

	header = (struct mach_header_64 *)(ptr->ptr);
	ncmds = header->ncmds;
	lc = (ptr->ptr) + sizeof(struct mach_header_64);
	i = 0;
	i_tab = 0;
	while (i < swap_endian(ncmds, header->magic))
	{
		if ((void *)lc > ptr->max)
		{
			printf("Error\n");
			return ;
		}
		if (swap_endian(lc->cmd, header->magic) == LC_SEGMENT_64)
			section_64(lc, ptr, header, &i_tab);
		lc = (void *)lc + swap_endian(lc->cmdsize, header->magic);
		i++;
	}
}
