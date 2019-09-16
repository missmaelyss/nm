/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marnaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/11 19:40:03 by marnaud           #+#    #+#             */
/*   Updated: 2019/09/11 19:40:06 by marnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <nm_otool.h>

void	print_symbol(uint8_t n_type, uint8_t n_sect,
		uint16_t n_desc, uint32_t type)
{
	char	type_symbol;

	write(1, " ", 1);
	if ((N_TYPE & n_type) != 0x0)
	{
		if ((n_type - (N_EXT & n_type)) == N_ABS)
			type_symbol = 'a';
		else if ((n_type - (N_EXT & n_type) - (N_PEXT & n_type)) == N_SECT)
			type_symbol = g_section_symbol[n_sect - 1];
		else if ((n_type - (N_EXT & n_type)) == N_INDR)
			type_symbol = 'i';
	}
	else
		type_symbol = 'u';
	if ((N_EXT & n_type) != 0x0)
		type_symbol += 'A' - 'a';
	write(1, &type_symbol, 1);
	write(1, " ", 1);
}

void	print_value(uint64_t n_value, int size, uint32_t n_type)
{
	char		*str;
	int			n;
	const char	alpha[16] = "0123456789abcdef";

	if (!(str = (char *)malloc(sizeof(char) * (size + 1))))
		return (ft_error("malloc", "Error."));
	n = size - 1;
	str[size] = '\0';
	while (n >= 0)
	{
		str[n] = alpha[n_value % 16];
		if ((size == 16 && n_type == 1) || (size == 8 && n_type == 0x1))
			str[n] = ' ';
		n_value = n_value / 16;
		n--;
	}
	write(1, str, size);
	free(str);
}

void	print_for_arch(cpu_type_t cputype)
{
	write(1, " (for architecture ", 19);
	if ((cputype & CPU_TYPE_X86) == CPU_TYPE_X86
			&& (cputype & CPU_ARCH_ABI64) != CPU_ARCH_ABI64)
		write(1, "i386", 4);
	else if ((cputype & CPU_ARCH_ABI64) == CPU_ARCH_ABI64)
		write(1, g_cpu_type_tab[cputype - CPU_ARCH_ABI64 - 1],
				ft_strlen(g_cpu_type_tab[cputype - CPU_ARCH_ABI64 - 1]));
	else
		write(1, g_cpu_type_tab[cputype - 1],
				ft_strlen(g_cpu_type_tab[cputype - 1]));
	if ((cputype & CPU_ARCH_ABI64) == CPU_ARCH_ABI64)
		write(1, "_64", 3);
	write(1, "):", 2);
}

void	print_path(char *av, uint8_t nm)
{
	if (nm)
		write(1, "\n", 1);
	write(1, av, ft_strlen(av));
	write(1, ":\n", 2);
}
