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

int			find_hexa(uint8_t nb, cpu_type_t cputype, int n, char *buffer)
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

void		find_value(uint64_t n_value, int size, uint32_t n_type, char *b)
{
	int			n;
	const char	alpha[16] = "0123456789abcdef";

	n = size - 1;
	while (n >= 0)
	{
		b[n] = alpha[n_value % 16];
		if ((size == 16 && n_type == 1) || (size == 8 && n_type == 0x1))
			b[n] = ' ';
		n_value = n_value / 16;
		n--;
	}
}

void		value(uint64_t n_value, char *buffer, int *i_b, int size)
{
	find_value(n_value, size, 0, &buffer[*i_b]);
	buffer[*i_b + size] = '\t';
	*i_b += size + 1;
}

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
