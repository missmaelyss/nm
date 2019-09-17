/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   section_otool.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marnaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/17 14:46:04 by marnaud           #+#    #+#             */
/*   Updated: 2019/09/17 15:14:28 by marnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <nm_otool.h>

void		section_64_otool(struct load_command *lc, t_file_ptr *ptr,
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
			return ;
		if (ft_strcmp(sect->sectname, "__text") == 0)
		{
			write(1, "Contents of (__TEXT,__text) section\n", 36);
			print_64_otool(sect, ptr, header);
		}
		n++;
		sect++;
	}
}

void		section_32_otool(struct load_command *lc, t_file_ptr *ptr,
		struct mach_header *header)
{
	struct segment_command	*seg;
	struct section			*sect;
	uint32_t				n;

	seg = (void *)lc;
	n = 0;
	sect = (void *)seg + sizeof(struct segment_command);
	while (n < seg->nsects)
	{
		if ((void *)sect > ptr->max)
			return ;
		if (ft_strcmp(sect->sectname, "__text") == 0)
		{
			write(1, "Contents of (__TEXT,__text) section\n", 36);
			print_32_otool(sect, ptr, header);
		}
		n++;
		sect++;
	}
}
