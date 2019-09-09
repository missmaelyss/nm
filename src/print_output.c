#include <nm_otool.h>

void    print_symbol(uint8_t n_type, uint8_t n_sect, uint16_t n_desc, uint32_t type)
{
    char    type_symbol;

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

void    print_value(uint64_t n_value, int size, uint32_t n_type)
{
    char    *str;
    int     n;
    char	alpha[16] = "0123456789abcdef";

    str = (char *)malloc(sizeof(char) * (size + 1));
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
}

void    print_output_64(struct symtab_command *sym, void *ptr, uint32_t magic, uint8_t ppc)
{
    int             i;
    int             *itab;
    int             good_sym;
    char            *stringtable;
    struct nlist_64 *el;

    if (ppc)
        OSSwapConstInt32(magic);
    i = 0;
    stringtable = ptr + swap_endian(sym->stroff, magic);
    el = ptr + swap_endian(sym->symoff, magic);
    good_sym = nb_good_sym_64(swap_endian(sym->nsyms, magic), el, magic);
    itab = find_order_64(swap_endian(sym->nsyms, magic), stringtable, el, magic);
    while (i < good_sym)
    {    
        if (!(swap_endian(el[itab[i]].n_type, magic) & N_STAB))
        {
            // printf("%s\t", stringtable + swap_endian(el[itab[i]].n_un.n_strx, magic));
            print_value(swap_endian_64(el[itab[i]].n_value, magic), 16, !ppc ? swap_endian(el[itab[i]].n_type, magic) : el[itab[i]].n_type);
            
            
            print_symbol(el[itab[i]].n_type, el[itab[i]].n_sect, swap_endian_short(el[itab[i]].n_desc, magic), 16);
            write(1, stringtable + swap_endian(el[itab[i]].n_un.n_strx, magic), ft_strlen(stringtable + swap_endian(el[itab[i]].n_un.n_strx, magic)));
            write(1, "\n", 1);
        }
        i++;
    }
}

void    print_output_32(struct symtab_command *sym, void *ptr, uint32_t magic, uint8_t ppc)
{
    int             i;
    int             *itab;
    int             good_sym;
    char            *stringtable;
    struct nlist    *el;

    if (ppc)
        OSSwapConstInt32(magic);
    i = 0;
    stringtable = ptr + swap_endian(sym->stroff, magic);
    el = ptr + swap_endian(sym->symoff, magic);
    good_sym = nb_good_sym_32(swap_endian(sym->nsyms, magic), el, magic);
    itab = find_order_32(swap_endian(sym->nsyms, magic), stringtable, el, magic);
    while (i < good_sym)
    {    
        if (!(swap_endian(el[itab[i]].n_type, magic) & N_STAB))
        {
            // printf("%s\t", stringtable + swap_endian(el[itab[i]].n_un.n_strx, magic));
            print_value(swap_endian(el[itab[i]].n_value, magic), 8, !ppc ? swap_endian(el[itab[i]].n_type, magic) : el[itab[i]].n_type);
            
            
            print_symbol(el[itab[i]].n_type, el[itab[i]].n_sect, swap_endian_short(el[itab[i]].n_desc, magic), 8);
            write(1, stringtable + swap_endian(el[itab[i]].n_un.n_strx, magic), ft_strlen(stringtable + swap_endian(el[itab[i]].n_un.n_strx, magic)));
            write(1, "\n", 1);
        }
        i++;
    }
}