/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quick_sort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marnaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/11 14:05:44 by marnaud           #+#    #+#             */
/*   Updated: 2019/09/11 17:27:04 by marnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <nm_otool.h>

void	swap_tab(int *tableau, int a, int b)
{
	int temp;

	temp = tableau[a];
	tableau[a] = tableau[b];
	tableau[b] = temp;
}

void	quick_loop_64(struct s_quick *d, struct nlist_64 *el, struct s_pivot *p)
{
	while (1)
	{
		while (ft_strcmp(d->s + swap_endian(el[d->t[p->r]].n_un.n_strx,
			d->m), p->str) > 0 || (ft_strcmp(d->s +
				swap_endian(el[d->t[p->r]].n_un.n_strx, d->m), p->str) == 0
					&& swap_endian(el[d->t[p->r]].n_value, d->m) > p->value))
			(p->r)--;
		while (ft_strcmp(d->s + swap_endian(el[d->t[p->l]].n_un.n_strx,
			d->m), p->str) < 0 || (ft_strcmp(d->s +
				swap_endian(el[d->t[p->l]].n_un.n_strx, d->m), p->str) == 0
					&& swap_endian(el[d->t[p->l]].n_value, d->m) < p->value))
			(p->l)++;
		if (ft_strcmp(d->s + swap_endian(el[d->t[p->l]].n_un.n_strx, d->m),
		d->s + swap_endian(el[d->t[p->r]].n_un.n_strx, d->m)) > 0
			|| (ft_strcmp(d->s + swap_endian(el[d->t[p->l]].n_un.n_strx, d->m),
			d->s + swap_endian(el[d->t[p->r]].n_un.n_strx, d->m)) == 0
				&& swap_endian(el[d->t[p->l]].n_value, d->m)
				> swap_endian(el[d->t[p->r]].n_value, d->m)))
			swap_tab(d->t, p->l, p->r);
		else
			break ;
	}
}

void	quicksort_64(struct s_quick *data, struct nlist_64 *el)
{
	struct s_pivot	p;

	if (data->start >= data->end)
		return ;
	p.l = data->start;
	p.r = data->end;
	p.str = ft_strdup(data->s
		+ swap_endian(el[data->t[data->start]].n_un.n_strx, data->m));
	p.value = swap_endian(el[data->t[data->start]].n_value, data->m);
	quick_loop_64(data, el, &p);
	if (p.str)
		free(p.str);
	quicksort_64(&(struct s_quick)
		{data->start, p.r, data->t, data->s, data->m}, el);
	quicksort_64(&(struct s_quick)
		{p.r + 1, data->end, data->t, data->s, data->m}, el);
}

void	quick_loop_32(struct s_quick *d, struct nlist *el, struct s_pivot *p)
{
	while (1)
	{
		while (ft_strcmp(d->s + swap_endian(el[d->t[p->r]].n_un.n_strx,
			d->m), p->str) > 0 || (ft_strcmp(d->s +
				swap_endian(el[d->t[p->r]].n_un.n_strx, d->m), p->str) == 0
					&& swap_endian(el[d->t[p->r]].n_value, d->m) > p->value))
			(p->r)--;
		while (ft_strcmp(d->s + swap_endian(el[d->t[p->l]].n_un.n_strx,
			d->m), p->str) < 0 || (ft_strcmp(d->s +
				swap_endian(el[d->t[p->l]].n_un.n_strx, d->m), p->str) == 0
					&& swap_endian(el[d->t[p->l]].n_value, d->m) < p->value))
			(p->l)++;
		if (ft_strcmp(d->s + swap_endian(el[d->t[p->l]].n_un.n_strx, d->m),
		d->s + swap_endian(el[d->t[p->r]].n_un.n_strx, d->m)) > 0
			|| (ft_strcmp(d->s + swap_endian(el[d->t[p->l]].n_un.n_strx, d->m),
			d->s + swap_endian(el[d->t[p->r]].n_un.n_strx, d->m)) == 0
				&& swap_endian(el[d->t[p->l]].n_value, d->m)
				> swap_endian(el[d->t[p->r]].n_value, d->m)))
			swap_tab(d->t, p->l, p->r);
		else
			break ;
	}
}

void	quicksort_32(struct s_quick *data, struct nlist *el)
{
	struct s_pivot	p;

	if (data->start >= data->end)
		return ;
	p.l = data->start;
	p.r = data->end;
	p.str = ft_strdup(data->s
		+ swap_endian(el[data->t[data->start]].n_un.n_strx, data->m));
	p.value = swap_endian(el[data->t[data->start]].n_value, data->m);
	quick_loop_32(data, el, &p);
	if (p.str)
		free(p.str);
	quicksort_32(&(struct s_quick)
		{data->start, p.r, data->t, data->s, data->m}, el);
	quicksort_32(&(struct s_quick)
		{p.r + 1, data->end, data->t, data->s, data->m}, el);
}
