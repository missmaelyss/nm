/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_order.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marnaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/11 19:39:32 by marnaud           #+#    #+#             */
/*   Updated: 2019/09/11 19:39:41 by marnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <nm_otool.h>

int		*find_order_64(struct symtab_command *sym, char *str,
	struct nlist_64 *el, uint32_t m)
{
	int		*itab;
	int		i;
	int		good_sym;

	good_sym = nb_good_sym_64(sym, el, m, NULL);
	if (!(itab = (int *)malloc(good_sym * sizeof(int))))
		return (0);
	i = 0;
	good_sym = 0;
	while (i < swap_endian(sym->nsyms, m))
	{
		if (!(el[i].n_type & N_STAB))
		{
			itab[good_sym] = i;
			good_sym++;
		}
		i++;
	}
	quicksort_64(&(struct s_quick){0, good_sym - 1, itab, str, m}, el);
	return (itab);
}

int		*find_order_32(struct symtab_command *sym, char *str,
	struct nlist *el, uint32_t m)
{
	int		*itab;
	int		i;
	int		good_sym;

	good_sym = nb_good_sym_32(sym, el, m, NULL);
	if (!(itab = (int *)malloc(good_sym * sizeof(int))))
		return (0);
	i = 0;
	good_sym = 0;
	while (i < swap_endian(sym->nsyms, m))
	{
		if (!(el[i].n_type & N_STAB))
		{
			itab[good_sym] = i;
			good_sym++;
		}
		i++;
	}
	quicksort_32(&(struct s_quick){0, good_sym - 1, itab, str, m}, el);
	return (itab);
}
