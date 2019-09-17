/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_otool.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marnaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/17 14:45:53 by marnaud           #+#    #+#             */
/*   Updated: 2019/09/17 14:45:58 by marnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <nm_otool.h>

static char	*create_buffer(int size)
{
	char	*buffer;

	if (!(buffer = (char *)malloc(size * sizeof(char))))
		exit(0);
	return (buffer);
}

void		print_32_otool(struct section *sect, t_file_ptr *ptr,
		struct mach_header *h)
{
	int		i;
	int		i_b;
	char	*buffer;

	buffer = create_buffer(swap_endian(sect->size, h->magic) * 4 +
		(swap_endian(sect->size, h->magic) / 16 + 1) * 8);
	i = 0;
	i_b = 0;
	while (i < swap_endian(sect->size, h->magic))
	{
		if (!(i % 16))
			value(swap_endian(sect->addr, h->magic) + i, buffer, &i_b, 8);
		if (ptr->ptr + swap_endian(sect->offset, h->magic) + i > ptr->max)
			return (free(buffer));
		i_b += find_hexa(*(uint8_t *)(ptr->ptr + swap_endian(sect->offset,
			h->magic) + i), h->cputype, i, &buffer[i_b]);
		i += 1;
		if (!(i % 16) || i == swap_endian(sect->size, h->magic))
		{
			buffer[i_b] = '\n';
			i_b++;
		}
	}
	write(1, buffer, i_b);
	free(buffer);
}

void		print_64_otool(struct section_64 *sect, t_file_ptr *ptr,
		struct mach_header_64 *h)
{
	int		i;
	int		i_b;
	char	*buffer;

	buffer = create_buffer(sect->size * 4 + (sect->size / 16 + 1) * 16);
	i = 0;
	i_b = 0;
	while (i < sect->size)
	{
		if (!(i % 16))
			value(swap_endian_64(sect->addr, h->magic) + i, buffer, &i_b, 16);
		if (ptr->ptr + swap_endian(sect->offset, h->magic) + i > ptr->max)
			return (free(buffer));
		i_b += find_hexa(*(uint8_t *)(ptr->ptr + swap_endian(sect->offset,
			h->magic) + i), h->cputype, i, &buffer[i_b]);
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
