/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_otool.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marnaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/11 19:52:59 by marnaud           #+#    #+#             */
/*   Updated: 2019/09/11 19:53:10 by marnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_OTOOL_H
# define NM_OTOOL_H

# include <ar.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/mman.h>
# include <sys/stat.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/fat.h>

typedef struct	s_file_ptr
{
	void	*ptr;
	void	*max;
}				t_file_ptr;

typedef struct	s_quick
{
	int			start;
	int			end;
	int			*t;
	char		*s;
	uint32_t	m;
}				t_quick;

typedef struct	s_pivot
{
	int			r;
	int			l;
	char		*str;
	uint32_t	value;
}				t_pivot;

char			*g_cpu_type_tab[18];
char			g_section_symbol[30];

void			fill_section_32(struct s_file_ptr *ptr, uint8_t ppc);
void			fill_section_64(struct s_file_ptr *ptr, uint8_t ppc);
void			modify_sec_sym(char *name, int *i_tab);
void			section_32(struct load_command *lc, t_file_ptr *ptr,
	struct mach_header *header, int *i_tab);
void			section_64(struct load_command *lc, t_file_ptr *ptr,
	struct mach_header_64 *header, int *i_tab);

int				*find_order_32(int nsyms, char *str, struct nlist *el,
	uint32_t magic);
int				*find_order_64(int nsyms, char *str, struct nlist_64 *el,
	uint32_t magic);

void			handle_32(struct s_file_ptr *ptr, uint8_t ppc);
void			handle_64(struct s_file_ptr *ptr, uint8_t ppc);
void			handle_fat_32(struct s_file_ptr *p, char *av, uint64_t s);
void			handle_fat_64(struct s_file_ptr *p, char *av, uint64_t s);
void			handle_lyb(struct s_file_ptr *ptr, char *av);

int				check_file(int *fd, char *av, struct s_file_ptr	*ptr,
	struct stat *buf);
void			ft_error(char *av, char *error);
void			nm(struct s_file_ptr *ptr, char *av, uint8_t mult);
void			pick_h(uint32_t mg, struct s_file_ptr *ptr, cpu_type_t c,
	char *av);

void			print_for_arch(cpu_type_t cputype);
void			print_path(char *av, uint8_t nm);
void			print_symbol(uint8_t n_type, uint8_t n_sect, uint16_t n_desc,
	uint32_t type);
void			print_value(uint64_t n_value, int size, uint32_t n_type);

void			ft_nstab_32(struct nlist el, uint32_t m, uint8_t ppc,
	char *stringtable);
void			ft_nstab_64(struct nlist_64 el, uint32_t m, uint8_t ppc,
	char *stringtable);
void			print_output_32(struct symtab_command *sym,
	struct s_file_ptr *ptr, uint32_t m, uint8_t ppc);
void			print_output_64(struct symtab_command *sym,
	struct s_file_ptr *ptr, uint32_t m, uint8_t ppc);

void			quick_loop_32(struct s_quick *d, struct nlist *el,
	struct s_pivot *p);
void			quick_loop_64(struct s_quick *d, struct nlist_64 *el,
	struct s_pivot *p);
void			quicksort_32(struct s_quick *data, struct nlist *el);
void			quicksort_64(struct s_quick *data, struct nlist_64 *el);
void			swap_tab(int *tableau, int a, int b);

char			*ft_strcpy(char *src, char *dest);
int				ft_strcmp(const char *s1, const char *s2);
char			*ft_strdup(char *src);
int				ft_strlen(char *str);

int				nb_good_sym_32(int nsyms, struct nlist *el, uint32_t magic);
int				nb_good_sym_64(int nsyms, struct nlist_64 *el, uint32_t magic);
uint32_t		swap_endian(uint32_t value, uint32_t magic);
uint64_t		swap_endian_64(uint64_t value, uint32_t magic);
uint16_t		swap_endian_short(uint16_t value, uint32_t magic);

int				print_fat_32(struct s_file_ptr *ptr, char *av,
	struct fat_header *header, struct fat_arch *arch);
int				print_fat_64(struct s_file_ptr *ptr, char *av,
	struct fat_header *header, struct fat_arch_64 *arch);
void			print_lyb(char *av, struct ar_hdr *h, uint8_t nm);
uint32_t		si32(uint32_t nb);
uint64_t		si64(uint64_t nb);

void			handle_32_otool(struct s_file_ptr *ptr, uint8_t ppc, char *av);
void			handle_64_otool(struct s_file_ptr *ptr, uint8_t ppc, char *av);
void			handle_fat_32_otool(struct s_file_ptr *p, char *av, uint64_t s);
void			handle_fat_64_otool(struct s_file_ptr *p, char *av, uint64_t s);
void			handle_lyb_otool(struct s_file_ptr *ptr, char *av);

void			pick_h_otool(uint32_t mg, struct s_file_ptr *ptr,
	cpu_type_t c, char *av);
void			otool(struct s_file_ptr *ptr, char *av, uint8_t mult);

int				find_hexa(uint8_t nb, cpu_type_t cputype, int n, char *buffer);
void			find_value(uint64_t n_value, int s, uint32_t n_type, char *b);
void			value(uint64_t n_value, char *buffer, int *i_b, int size);
void			print_32_otool(struct section *sect, t_file_ptr *ptr,
		struct mach_header *h);
void			print_64_otool(struct section_64 *sect, t_file_ptr *ptr,
		struct mach_header_64 *h);

void			section_64_otool(struct load_command *lc, t_file_ptr *ptr,
		struct mach_header_64 *header);
void			section_32_otool(struct load_command *lc, t_file_ptr *ptr,
		struct mach_header *header);

#endif
