#ifndef NM_OTOOL_H
# define NM_OTOOL_H

# include <fcntl.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/mman.h>
# include <sys/stat.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/fat.h>

void        nm(void *ptr, char *av, uint8_t mult);
void        print_for_arch(cpu_type_t cputype);
void        print_path(char *av);

int         *find_order_64(int nsyms, char *str, struct nlist_64 *el, uint32_t magic);
int         *find_order_32(int nsyms, char *str, struct nlist *el, uint32_t magic);
void        quickSort_64(int *tableau, int debut, int fin, char *str, struct nlist_64 *el, uint32_t magic);
void        quickSort_32(int *tableau, int debut, int fin, char *str, struct nlist *el, uint32_t magic);
void        swap_tab(int *tableau, int a, int b);

void        print_output_64(struct symtab_command *sym, void *ptr, uint32_t magic, uint8_t ppc);
void        print_output_32(struct symtab_command *sym, void *ptr, uint32_t magic, uint8_t ppc);
void        print_symbol(uint8_t n_type, uint8_t n_sect, uint16_t n_desc, uint32_t type);
void        print_value(uint64_t n_value, int size, uint32_t type);

void        handle_64(void *ptr, uint8_t ppc);
void        handle_32(void *ptr, uint8_t ppc);
void        handle_fat_64(void *ptr, char *av);
void        handle_fat_32(void *ptr, char *av);

int		    ft_strcmp(const char *s1, const char *s2);
int         ft_strlen(char *str);
char        *ft_str_cpy(char *src, char *dest);
char        *ft_strdup(char *src);

uint16_t    swap_endian_short(uint16_t value, uint32_t magic);
uint32_t    swap_endian(uint32_t value, uint32_t magic);
int         nb_good_sym_64(int nsyms, struct nlist_64 *el, uint32_t magic);
int         nb_good_sym_32(int nsyms, struct nlist *el, uint32_t magic);

#endif