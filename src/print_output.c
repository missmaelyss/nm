/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_output.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marnaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/11 19:03:40 by marnaud           #+#    #+#             */
/*   Updated: 2019/09/11 19:23:34 by marnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <nm_otool.h>

void	ft_nstab_64(struct nlist_64 el, uint32_t m, uint8_t ppc,
		char *stringtable)
{
	print_value(swap_endian_64(el.n_value, m), 16, !ppc ?
			swap_endian(el.n_type, m) : el.n_type);
	print_symbol(el.n_type, el.n_sect,
			swap_endian_short(el.n_desc, m), 16);
	write(1, stringtable + swap_endian(el.n_un.n_strx, m),
			ft_strlen(stringtable + swap_endian(el.n_un.n_strx, m)));
	write(1, "\n", 1);
}

void	ft_nstab_32(struct nlist el, uint32_t m, uint8_t ppc,
		char *stringtable)
{
	print_value(swap_endian(el.n_value, m), 8, !ppc ?
			swap_endian(el.n_type, m) : el.n_type);
	print_symbol(el.n_type, el.n_sect,
			swap_endian_short(el.n_desc, m), 16);
	write(1, stringtable + swap_endian(el.n_un.n_strx, m),
			ft_strlen(stringtable + swap_endian(el.n_un.n_strx, m)));
	write(1, "\n", 1);
}

void	print_output_64(struct symtab_command *sym,
	struct s_file_ptr *ptr, uint32_t m, uint8_t ppc)
{
	int				i;
	int				*itab;
	int				good_sym;
	char			*stringtable;
	struct nlist_64 *el;

	if (ppc)
		OSSwapConstInt32(m);
	i = 0;
	stringtable = ptr->ptr + swap_endian(sym->stroff, m);
	el = ptr->ptr + swap_endian(sym->symoff, m);
	if ((void *)el > ptr->max)
		return (ft_error("ft_nm", "Error\n"));
	good_sym = nb_good_sym_64(swap_endian(sym->nsyms, m), el, m);
	itab = find_order_64(swap_endian(sym->nsyms, m), stringtable, el, m);
	while (i < good_sym)
	{
		if (!(swap_endian(el[itab[i]].n_type, m) & N_STAB))
			ft_nstab_64(el[itab[i]], m, ppc, stringtable);
		i++;
	}
	if (itab)
		free(itab);
}

void	print_output_32(struct symtab_command *sym,
	struct s_file_ptr *ptr, uint32_t m, uint8_t ppc)
{
	int				i;
	int				*itab;
	int				good_sym;
	char			*stringtable;
	struct nlist	*el;

	if (ppc)
		OSSwapConstInt32(m);
	i = 0;
	stringtable = ptr->ptr + swap_endian(sym->stroff, m);
	el = ptr->ptr + swap_endian(sym->symoff, m);
	if ((void *)el > ptr->max)
		return (ft_error("ft_nm", "Error\n"));
	good_sym = nb_good_sym_32(swap_endian(sym->nsyms, m), el, m);
	itab = find_order_32(swap_endian(sym->nsyms, m), stringtable, el, m);
	while (i < good_sym)
	{
		if (!(swap_endian(el[itab[i]].n_type, m) & N_STAB))
			ft_nstab_32(el[itab[i]], m, ppc, stringtable);
		i++;
	}
	if (itab)
		free(itab);
}
