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

void		print_32_otool(struct section *sect, t_file_ptr *ptr,
		struct mach_header *h)
{
	int		size;
	int		i;
	int		i_b;
	char	*buffer;

	size = swap_endian(sect->size, h->magic) * 4 +
		(swap_endian(sect->size, h->magic) / 16 + 1) * 8;
	buffer = (char *)malloc(sizeof(char) * size);
	i = 0;
	i_b = 0;
	while (i < swap_endian(sect->size, h->magic))
	{
		if (!(i % 16))
			value(swap_endian(sect->addr, h->magic) + i, buffer, &i_b, 8);
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
	int		size;
	int		i;
	int		i_b;
	char	*buffer;

	size = sect->size * 4 + (sect->size / 16 + 1) * 16;
	buffer = (char *)malloc(sizeof(char) * size);
	i = 0;
	i_b = 0;
	while (i < sect->size)
	{
		if (!(i % 16))
			value(swap_endian_64(sect->addr, h->magic) + i, buffer, &i_b, 16);
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
