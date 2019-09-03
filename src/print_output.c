#include <nm_otool.h>

void    print_symbol(uint8_t n_type, uint8_t n_sect, uint16_t n_desc, uint32_t type)
{
    // char    small[13] = "0t00ss0000sdb";
    // char    big[14] = "0T00S000S0SDBS";

    // printf(" %x, %x, %x\n", n_type, n_sect, n_desc);
    
    write(1, " ", 1);
    if (n_type == 0x0f)
    {
        if ((n_sect == 0x7 && n_desc != 0x10) || n_sect == 0xc || n_sect == 0x11 
        || (n_sect == 0xd && n_desc == 0x0) || n_sect == 0x5 || n_sect == 0x6 || n_sect == 0x8
        || (n_sect == 0xb && n_desc != 0x80) || (n_sect == 0x4 && n_desc != 0x10) || (n_sect == 0xe && n_desc != 0x0 && type == 8)
        || (n_sect == 0x10 && n_desc == 0x0) || (n_sect == 0xa && n_desc == 0x0 && type == 8))
            write(1, "S", 1);
        else if (n_sect == 0x7 || n_sect == 0x4 || n_sect == 0xa || n_sect == 0xf || n_sect == 0xb
        || (n_sect == 0xc && n_desc == 0x80) || n_sect == 0xd || n_sect == 0xe)
            write(1, "D", 1);
        else
            write(1, "T", 1);
    }
    else if (n_type == 0x0e || n_type == 0x1e)
    {
       if (n_sect == 0x4 || n_sect == 0x5 || n_sect == 0x6 || (n_type == 0x1e && n_sect == 0xf && n_desc != 0x80)
       || (n_type == 0x1e && n_sect == 0x16) || (n_type == 0x1e && n_sect == 0x15) || (n_sect == 0xd && n_desc != 0x0))
            write(1, "s", 1);
        else if (n_sect == 0x8 || n_sect == 0xd || n_sect == 0x10 || (n_sect == 0xe && n_desc != 0x0)
        || (n_type == 0x0e && n_sect == 0xf && n_desc != 0x0) || (n_desc == 0x11 && n_desc == 0x0)
        || (n_sect == 0xc && n_desc == 0x0 )|| (n_sect == 0x11 && n_desc == 0x0))
            write(1, "b", 1);
        else if (n_sect == 0x7 || n_sect == 0xf || n_sect == 0xb || (n_sect == 0xe && n_desc == 0x0) ||
        (n_sect == 0xa && n_desc == 0x0))
            write(1, "d", 1);
        else
            write(1, "t", 1);
    }
    else if (n_type == 0x3)
        write(1, "A", 1);
    else
        write(1, "U", 1);
    write(1, " ", 1);
}

void    print_value(uint64_t n_value, int size, uint32_t type)
{
    char    *str;
    int     n;
    char	alpha[16] = "0123456789abcdef";

    str = (char *)malloc(sizeof(char) * (size + 1));
    n = size - 1;
    str[size] = '\0';
    if (size == 16 && type != 1)
        n_value += 0x100000000;
    while (n >= 0)
    {
        str[n] = alpha[n_value % 16];
        if ((size == 16 && type == 1) || (size == 8 && type == 0x1))
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
            print_value(swap_endian(el[itab[i]].n_value, magic), 16, !ppc ? swap_endian(el[itab[i]].n_type, magic) : el[itab[i]].n_type);
            
            // printf("%s\t", stringtable + swap_endian(el[itab[i]].n_un.n_strx, magic));
            
            print_symbol(el[itab[i]].n_type, el[itab[i]].n_sect, swap_endian_short(el[itab[i]].n_desc, magic), 16);
            write(1, stringtable + swap_endian(el[itab[i]].n_un.n_strx, magic), ft_strlen(stringtable + swap_endian(el[itab[i]].n_un.n_strx, magic)));
            write(1, "\n", 1);
        }
        i++;
    }
}

void    print_output_32(struct symtab_command *sym, void *ptr, uint32_t magic, uint8_t ppc)
{
    // printf("print output 32\n");

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
            print_value(swap_endian(el[itab[i]].n_value, magic), 8, !ppc ? swap_endian(el[itab[i]].n_type, magic) : el[itab[i]].n_type);
            
            // printf("%s\t", stringtable + swap_endian(el[itab[i]].n_un.n_strx, magic));
            
            print_symbol(el[itab[i]].n_type, el[itab[i]].n_sect, swap_endian_short(el[itab[i]].n_desc, magic), 8);
            write(1, stringtable + swap_endian(el[itab[i]].n_un.n_strx, magic), ft_strlen(stringtable + swap_endian(el[itab[i]].n_un.n_strx, magic)));
            write(1, "\n", 1);
        }
        i++;
    }
}
