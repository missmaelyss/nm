/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marnaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/11 18:12:41 by marnaud           #+#    #+#             */
/*   Updated: 2019/09/11 19:00:36 by marnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <nm_otool.h>

void		print_lyb(char *av, struct ar_hdr *h, uint8_t nm)
{
	if (nm)
		write(1, "\n", 1);
	write(1, av, ft_strlen(av));
	write(1, "(", 1);
	write(1, &((h->ar_fmag)[2]), ft_strlen(&((h->ar_fmag)[2])));
	write(1, "):\n", 3);
}

int			print_fat_64(struct s_file_ptr *ptr, char *av,
	struct fat_header *header, struct fat_arch_64 *arch)
{
	if ((void *)arch > ptr->max)
	{
		ft_error("ft_nm", "Error\n");
		return (0);
	}
	// write(1, "\n", 1);
	write(1, av, ft_strlen(av));
	if (OSSwapConstInt32(header->nfat_arch) > 1)
		print_for_arch((OSSwapConstInt64(arch->cputype)));
	write(1, "\n", 1);
	return (1);
}

int			print_fat_32(struct s_file_ptr *ptr, char *av,
	struct fat_header *header, struct fat_arch *arch)
{
	if ((void *)arch > ptr->max)
	{
		ft_error("ft_nm", "Error\n");
		return (0);
	}
	// write(1, "\n", 1);
	write(1, av, ft_strlen(av));
	if (OSSwapConstInt32(header->nfat_arch) > 1)
		print_for_arch((OSSwapConstInt32(arch->cputype)));
	write(1, "\n", 1);
	return (1);
}

uint32_t	si32(uint32_t nb)
{
	return (OSSwapConstInt32(nb));
}

uint64_t	si64(uint64_t nb)
{
	return (OSSwapConstInt64(nb));
}
