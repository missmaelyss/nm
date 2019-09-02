#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <mach-o/fat.h>
#include <unistd.h>

char* g_cpu_type_tab[18] = {"vax", "", "", "", "", "mc680x0", "x86", "", "", "mc98000", "hppa", "arm", "mc88000", "sparc", "i860", "", "", "ppc"};

int     ft_strlen(char *str)
{
    int n;

    n = 0;
    while (str[n] != 0)
        n++;
    return (n);
}

void    print_value(uint64_t n_value, int size, uint64_t type)
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
        if (size == 16 && type == 1 || size == 8 && type == 0x1)
            str[n] = ' ';
        n_value = n_value / 16;
        n--;
    }
    write(1, str, size);
}

void    print_symbol(uint8_t n_type, uint8_t n_sect, uint16_t n_desc)
{
    char    small[13] = "0t00ss0000sdb";
    char    big[14] = "0T00S000S0SDBS";

    //9805

    // printf(" %x, %x, %x\n", n_type, n_sect, n_desc);
    write(1, " ", 1);
    if (n_type & 0x0f == 0x0f)
    {
        if (n_type == 0x1)
            write(1, "U", 1);
        else if (n_type == 0x3)
            write(1, "A", 1);
        else if (n_sect == 0x7 && n_desc == 0x10 || n_sect == 0xc || n_sect == 0x11 
        || n_sect == 0xd && n_desc != 0x80 || n_sect == 0x5 || n_sect == 0x6 && n_desc != 0x80 || n_sect == 0x8
        || n_sect == 0xb && n_desc != 0x80 || n_sect == 0x4 && n_desc == 0x80 || n_sect == 0xe && n_desc != 0x80)
            write(1, "S", 1);
        else if (n_sect == 0x7 || n_sect == 0x4 || n_sect == 0xa || n_sect == 0xf || n_sect == 0xb
        || n_sect == 0x6 || n_sect == 0xc && n_desc == 0x80 || n_sect == 0xd || n_sect == 0xe)
            write(1, "D", 1);
        else
            write(1, "T", 1);
    }
    else if (n_type & 0x0e)
    {
       if (n_sect == 0x4 || n_sect == 0x5 || n_sect == 0x6 || n_type == 0x1e && n_sect == 0xf
       || n_type == 0x1e && n_sect == 0x16 || n_type == 0x1e && n_sect == 0x15)
            write(1, "s", 1);
        else if (n_sect == 0x8 || n_sect == 0xd || n_sect == 0x10 || n_sect == 0xe)
            write(1, "b", 1);
        else if (n_sect == 0x7 || n_sect == 0xf || n_sect == 0xb)
            write(1, "d", 1);
        else
            write(1, "t", 1);
    }
    write(1, " ", 1);
}

char    *ft_str_cpy(char *src, char *dest)
{
    int n;

    n = 0;
    while(n < ft_strlen(src))
    {
        dest[n] = src[n];
        n++;
    }
    dest[n] = 0;
    return (dest);
}

char    *ft_strdup(char *src)
{
    int     n;
    int     len;
    char    *dest;

    n = 0;
    len = ft_strlen(src);
    dest = (char *)malloc(sizeof(char) * (len + 1));
    while(n < len)
    {
        dest[n] = src[n];
        n++;
    }
    dest[n] = 0;
    return (dest);
}

int		ft_strcmp(const char *s1, const char *s2)
{
	int n;

	n = 0;
    if (!s1 && !s2)
        return (0);
    if (!s1)
        return (-s2[0]);
    if (!s2)
        return (s1[0]);
	while (s1[n] == s2[n] && s1[n] != '\0' && s2[n] != '\0')
		n++;
	return ((unsigned char)s1[n] - (unsigned char)s2[n]);
}

void echanger(int *tableau, int a, int b)
{
    int temp = tableau[a];
    tableau[a] = tableau[b];
    tableau[b] = temp;
    // printf("In echanger\n");
}

void quickSort_64(int *tableau, int debut, int fin, char *str, struct nlist_64 *el, uint32_t magic)
{
    int gauche = debut;
    int droite = fin;
    const char *pivot = (debut >= fin) ? NULL : ft_strdup(str + swap_endian(el[tableau[debut]].n_un.n_strx, magic));
    const uint32_t   pivot_value = (debut >= fin) ? 0 : swap_endian(el[tableau[debut]].n_value, magic);

    /* Si le tableau est de longueur nulle, il n'y a rien à faire. */
    if (debut >= fin)
        return;

    // printf("%d, %d\n", debut, fin);

    // int n = 0;
    // while (n < 5) {
    //     printf("%d %s %d\n", n, str + swap_endian(el[tableau[n]].n_un.n_strx, magic), swap_endian(el[tableau[n]].n_value, magic));
    //     n++;
    // }

    /* Sinon, on parcourt le tableau, une fois de droite à gauche, et une
       autre de gauche à droite, à la recherche d'éléments mal placés,
       que l'on permute. Si les deux parcours se croisent, on arrête. */
    while(1)
    {
        // printf("\nDebut de boucle\n%d droite: %s\n%d gauche: %s\n%d pivot: %s\n\n", droite, str + swap_endian(el[tableau[droite]].n_un.n_strx, magic), gauche, str + swap_endian(el[tableau[gauche]].n_un.n_strx, magic), pivot_value, pivot);
        while(ft_strcmp(str + swap_endian(el[tableau[droite]].n_un.n_strx, magic), pivot) > 0 || 
            ft_strcmp(str + swap_endian(el[tableau[droite]].n_un.n_strx, magic), pivot) == 0 && 
            swap_endian(el[tableau[droite]].n_value, magic) > pivot_value)
        {   
            // printf("DROITE %s, %s, %d\n", str + swap_endian(el[tableau[droite]].n_un.n_strx, magic), pivot, ft_strcmp(str + swap_endian(el[tableau[droite]].n_un.n_strx, magic), pivot));
            droite--;
        }
        while(ft_strcmp(str + swap_endian(el[tableau[gauche]].n_un.n_strx, magic), pivot) < 0 ||
            ft_strcmp(str + swap_endian(el[tableau[gauche]].n_un.n_strx, magic), pivot) == 0 && 
            swap_endian(el[tableau[gauche]].n_value, magic) < pivot_value)
        {
            // printf("GAUCHE %s, %s, %d\n", str + swap_endian(el[tableau[gauche]].n_un.n_strx, magic), pivot, ft_strcmp(str + swap_endian(el[tableau[gauche]].n_un.n_strx, magic), pivot));
            gauche++;
        }            
        // do droite--; while(tableau[droite] > pivot);
        // do gauche++; while(tableau[gauche] < pivot);

        // printf("\n%d droite: %s\n%d gauche: %s\n%d pivot: %s\n\n", droite, str + swap_endian(el[tableau[droite]].n_un.n_strx, magic), gauche, str + swap_endian(el[tableau[gauche]].n_un.n_strx, magic), pivot_value, pivot);
        // printf("ON BLOQUE:\n%s, %s\n", str + swap_endian(el[tableau[droite]].n_un.n_strx, magic), str + swap_endian(el[tableau[gauche]].n_un.n_strx, magic));
        if (ft_strcmp(str + swap_endian(el[tableau[gauche]].n_un.n_strx, magic), str + swap_endian(el[tableau[droite]].n_un.n_strx, magic)) > 0 ||
            ft_strcmp(str + swap_endian(el[tableau[gauche]].n_un.n_strx, magic), str + swap_endian(el[tableau[droite]].n_un.n_strx, magic)) == 0 && 
            swap_endian(el[tableau[gauche]].n_value, magic) > swap_endian(el[tableau[droite]].n_value, magic))
        {
            // printf("On echange\n");
            echanger(tableau, gauche, droite);
        }
        else break;
    }

    /* Maintenant, tous les éléments inférieurs au pivot sont avant ceux
       supérieurs au pivot. On a donc deux groupes de cases à trier. On utilise
       pour cela... la méthode quickSort elle-même ! */
    quickSort_64(tableau, debut, droite, str, el, magic);
    quickSort_64(tableau, droite+1, fin, str, el, magic);
}

void quickSort_32(int *tableau, int debut, int fin, char *str, struct nlist *el, uint32_t magic)
{
    int gauche = debut;
    int droite = fin;
    const char *pivot = (debut >= fin) ? NULL : ft_strdup(str + swap_endian(el[tableau[debut]].n_un.n_strx, magic));
    const uint32_t   pivot_value = (debut >= fin) ? 0 : swap_endian(el[tableau[debut]].n_value, magic);


    /* Si le tableau est de longueur nulle, il n'y a rien à faire. */
    if (debut >= fin)
        return;

    // printf("%d, %d\n", debut, fin);

    // int n = 0;
    // while (n < 37) {
    //     printf("%s\n", str + swap_endian(el[tableau[n]].n_un.n_strx, magic));
    //     n++;
    // }

    /* Sinon, on parcourt le tableau, une fois de droite à gauche, et une
       autre de gauche à droite, à la recherche d'éléments mal placés,
       que l'on permute. Si les deux parcours se croisent, on arrête. */
    while(1)
    {
        // printf("\nDebut de boucle\n%d droite: %s\n%d gauche: %s\n%d pivot: %s\n\n", droite, str + swap_endian(el[tableau[droite]].n_un.n_strx, magic), gauche, str + swap_endian(el[tableau[gauche]].n_un.n_strx, magic), pivot_value, pivot);
        while(ft_strcmp(str + swap_endian(el[tableau[droite]].n_un.n_strx, magic), pivot) > 0 || 
            ft_strcmp(str + swap_endian(el[tableau[droite]].n_un.n_strx, magic), pivot) == 0 && 
            swap_endian(el[tableau[droite]].n_value, magic) > pivot_value)
        {   
            // printf("DROITE %s, %s, %d\n", str + swap_endian(el[tableau[droite]].n_un.n_strx, magic), pivot, ft_strcmp(str + swap_endian(el[tableau[droite]].n_un.n_strx, magic), pivot));
            droite--;
        }
        while(ft_strcmp(str + swap_endian(el[tableau[gauche]].n_un.n_strx, magic), pivot) < 0 ||
            ft_strcmp(str + swap_endian(el[tableau[gauche]].n_un.n_strx, magic), pivot) == 0 && 
            swap_endian(el[tableau[gauche]].n_value, magic) < pivot_value)
        {
            // printf("GAUCHE %s, %s, %d\n", str + swap_endian(el[tableau[gauche]].n_un.n_strx, magic), pivot, ft_strcmp(str + swap_endian(el[tableau[gauche]].n_un.n_strx, magic), pivot));
            gauche++;
        }            
        // do droite--; while(tableau[droite] > pivot);
        // do gauche++; while(tableau[gauche] < pivot);

        // printf("\n%d droite: %s\n%d gauche: %s\n%d pivot: %s\n\n", droite, str + swap_endian(el[tableau[droite]].n_un.n_strx, magic), gauche, str + swap_endian(el[tableau[gauche]].n_un.n_strx, magic), pivot_value, pivot);
        // printf("ON BLOQUE:\n%s, %s\n", str + swap_endian(el[tableau[droite]].n_un.n_strx, magic), str + swap_endian(el[tableau[gauche]].n_un.n_strx, magic));
        if (ft_strcmp(str + swap_endian(el[tableau[gauche]].n_un.n_strx, magic), str + swap_endian(el[tableau[droite]].n_un.n_strx, magic)) > 0 ||
            ft_strcmp(str + swap_endian(el[tableau[gauche]].n_un.n_strx, magic), str + swap_endian(el[tableau[droite]].n_un.n_strx, magic)) == 0 && 
            swap_endian(el[tableau[gauche]].n_value, magic) > swap_endian(el[tableau[droite]].n_value, magic))
        {
            // printf("On echange\n");
            echanger(tableau, gauche, droite);
        }
        else break;
    }

    /* Maintenant, tous les éléments inférieurs au pivot sont avant ceux
       supérieurs au pivot. On a donc deux groupes de cases à trier. On utilise
       pour cela... la méthode quickSort elle-même ! */
    quickSort_32(tableau, debut, droite, str, el, magic);
    quickSort_32(tableau, droite+1, fin, str, el, magic);
}

int     nb_good_sym_64(int nsyms, char *str, struct nlist_64 *el, uint32_t magic)
{
    int     i;
    int     good_sym;

    i = 0;
    good_sym = 0;
    while (i < nsyms)
    {
        if (!(swap_endian(el[i].n_type, magic) & N_STAB))
            good_sym++;
        i++;
    }
    return (good_sym);
}

int     nb_good_sym_32(int nsyms, char *str, struct nlist *el, uint32_t magic)
{
    // printf("good sym 32\n");

    int     i;
    int     good_sym;

    i = 0;
    good_sym = 0;
    while (i < nsyms)
    {
        if (!(swap_endian(el[i].n_type, magic) & N_STAB))
            good_sym++;
        i++;
    }
    return (good_sym);
}

int    *find_order_64(int nsyms, char *str, struct nlist_64 *el, uint32_t magic)
{
    int     *itab;
    int     i;
    int     good_sym;

    good_sym = nb_good_sym_64(nsyms, str, el, magic);
    itab = (int *)malloc(good_sym * sizeof(int));
    i = 0;
    good_sym = 0;
    while (i < nsyms)
    {
        if (!(swap_endian(el[i].n_type, magic) & N_STAB))
        {
            itab[good_sym] = i;
            good_sym++;
        }
        i++;
    }
    // printf("%d %d Find order 64\n", nsyms, NXSwapInt(nsyms));
    quickSort_64(itab, 0, good_sym - 1, str, el, magic);
    return (itab);
}

int    *find_order_32(int nsyms, char *str, struct nlist *el, uint32_t magic)
{
    int     *itab;
    int     i;
    int     good_sym;

    good_sym = nb_good_sym_32(nsyms, str, el, magic);
    itab = (int *)malloc(good_sym * sizeof(int));
    i = 0;
    good_sym = 0;
    while (i < nsyms)
    {
        if (!(swap_endian(el[i].n_type, magic) & N_STAB))
        {
            itab[good_sym] = i;
            good_sym++;
        }
        i++;
    }
    quickSort_32(itab, 0, good_sym - 1, str, el, magic);
    return (itab);
}

void    print_output_64(struct symtab_command *sym, void *ptr, uint32_t magic, uint8_t ppc)
{
    int             i;
    int             *itab;
    int             good_sym;
    char            *stringtable;
    struct nlist_64 *el;

    if (ppc)
        NXSwapInt(magic);
    i = 0;
    stringtable = ptr + swap_endian(sym->stroff, magic);
    el = ptr + swap_endian(sym->symoff, magic);
    good_sym = nb_good_sym_64(swap_endian(sym->nsyms, magic), stringtable, el, magic);
    itab = find_order_64(swap_endian(sym->nsyms, magic), stringtable, el, magic);
    while (i < good_sym)
    {    
        if (!(swap_endian(el[itab[i]].n_type, magic) & N_STAB))
        {
            print_value(swap_endian(el[itab[i]].n_value, magic), 16, !ppc ? swap_endian(el[itab[i]].n_type, magic) : el[itab[i]].n_type);
            // printf("%s\t", stringtable + swap_endian(el[itab[i]].n_un.n_strx, magic));
            if (!ppc)
                print_symbol(swap_endian(el[itab[i]].n_type, magic), swap_endian(el[itab[i]].n_sect, magic), swap_endian(el[itab[i]].n_desc, magic));
            else
                print_symbol(el[itab[i]].n_type, el[itab[i]].n_sect, el[itab[i]].n_desc);
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
        NXSwapInt(magic);
    i = 0;
    stringtable = ptr + swap_endian(sym->stroff, magic);
    el = ptr + swap_endian(sym->symoff, magic);
    good_sym = nb_good_sym_32(swap_endian(sym->nsyms, magic), stringtable, el, magic);
    itab = find_order_32(swap_endian(sym->nsyms, magic), stringtable, el, magic);
    while (i < good_sym)
    {    
        if (!(swap_endian(el[itab[i]].n_type, magic) & N_STAB))
        {
            print_value(swap_endian(el[itab[i]].n_value, magic), 8, !ppc ? swap_endian(el[itab[i]].n_type, magic) : el[itab[i]].n_type);
            // printf("%s\t", stringtable + swap_endian(el[itab[i]].n_un.n_strx, magic));
            if (!ppc)
                print_symbol(swap_endian(el[itab[i]].n_type, magic), swap_endian(el[itab[i]].n_sect, magic), swap_endian(el[itab[i]].n_desc, magic));
            else
                print_symbol(el[itab[i]].n_type, el[itab[i]].n_sect, el[itab[i]].n_desc);
            write(1, stringtable + swap_endian(el[itab[i]].n_un.n_strx, magic), ft_strlen(stringtable + swap_endian(el[itab[i]].n_un.n_strx, magic)));
            write(1, "\n", 1);
        }
        i++;
    }
}

int    swap_endian(int value, uint32_t magic)
{
    if (magic == MH_CIGAM || magic == MH_CIGAM_64)
        return (NXSwapInt(value));
    return (value);
}

void    handle_64(void *ptr, uint8_t ppc)
{

    // write(1, "handle 64\n", 10);
    // printf("handle 64\n");

    int                     ncmds;
    int                     i;
    struct mach_header_64   *header;
    struct load_command     *lc;
    struct symtab_command   *sym;

    header = (struct mach_header_64 *)ptr;
    ncmds = header->ncmds;
    lc = ptr + sizeof(struct mach_header_64);
    i = 0;
    while (i < swap_endian(ncmds, header->magic))
    {
        if (swap_endian(lc->cmd, header->magic) == LC_SYMTAB)
        {
            sym = (struct symtab_command *)lc;
            print_output_64(sym, ptr, header->magic, ppc);
            break ;
        }
        lc = (void *)lc + lc->cmdsize;
        i++;
    }
}

void    handle_32(void *ptr, uint8_t ppc)
{
    // write(1, "handle 32\n", 10);
    // printf("handle 32\n");

    int                     ncmds;
    int                     i;
    struct mach_header      *header;
    struct load_command     *lc;
    struct symtab_command   *sym;

    header = (struct mach_header *)ptr;
    ncmds = header->ncmds;
    lc = ptr + sizeof(struct mach_header);
    i = 0;
    while (i < swap_endian(ncmds, header->magic))
    {
        if (swap_endian(lc->cmd, header->magic) == LC_SYMTAB)
        {
            sym = (struct symtab_command *)lc;
            print_output_32(sym, ptr, header->magic, ppc);
            break ;
        }
        lc = (void *)lc + swap_endian(lc->cmdsize, header->magic);
        i++;
    }
}

void    print_for_arch(cpu_type_t cputype)
{
    write(1, " (for architecture ", 19);
    if (cputype & CPU_TYPE_X86 == CPU_TYPE_X86 && (cputype & CPU_ARCH_ABI64) != CPU_ARCH_ABI64)
        write(1, "i386", 4);
    else if ((cputype & CPU_ARCH_ABI64) == CPU_ARCH_ABI64)
        write(1, g_cpu_type_tab[cputype - CPU_ARCH_ABI64 - 1], ft_strlen(g_cpu_type_tab[cputype - CPU_ARCH_ABI64 - 1]));
    else
        write(1, g_cpu_type_tab[cputype - 1], ft_strlen(g_cpu_type_tab[cputype - 1]));
    if ((cputype & CPU_ARCH_ABI64) == CPU_ARCH_ABI64)
        write(1, "_64", 3);
    write(1, "):", 2);

}

void    handle_fat_32(void *ptr, char *av)
{
    // write(1, "\n", 14);

    int                 i;
    struct fat_header   *header;
    struct fat_arch     *arch;

    i = 0;
    header = (struct fat_header *)ptr;
    arch = ptr + sizeof(struct fat_header);
    while (i < NXSwapInt(header->nfat_arch))
    {
        write(1, "\n", 1);
        write(1, av, ft_strlen(av));
        if (NXSwapInt(header->nfat_arch) > 1)
            print_for_arch((NXSwapInt(arch->cputype)));
        write(1, "\n", 1);
        if ((NXSwapInt(arch->cputype) & CPU_ARCH_ABI64) == CPU_ARCH_ABI64)
            handle_64(ptr + NXSwapInt(arch->offset), (NXSwapInt(arch->cputype) & CPU_TYPE_POWERPC) == CPU_TYPE_POWERPC);
        else
            handle_32(ptr + NXSwapInt(arch->offset), (NXSwapInt(arch->cputype) & CPU_TYPE_POWERPC) == CPU_TYPE_POWERPC);
        i++;
        arch++;
    }
}

void    handle_fat_64(void *ptr, char *av)
{
    // write(1, "handle fat 64\n", 14);
    // printf("handle fat 64\n");

    int                 i;
    struct fat_header   *header;
    struct fat_arch_64     *arch;

    i = 0;
    header = (struct fat_header *)ptr;
    arch = ptr + sizeof(struct fat_header);
    while (i < NXSwapInt(header->nfat_arch))
    {
        write(1, "\n", 1);
        write(1, av, ft_strlen(av));
        if (NXSwapInt(header->nfat_arch) > 1)
            print_for_arch((NXSwapInt(arch->cputype)));
        write(1, "\n", 1);
        if ((NXSwapInt(arch->cputype) & CPU_ARCH_ABI64) == CPU_ARCH_ABI64)
            handle_64(ptr + NXSwapInt(arch->offset), (NXSwapInt(arch->cputype) & CPU_TYPE_POWERPC) == CPU_TYPE_POWERPC);
        else
            handle_32(ptr + NXSwapInt(arch->offset), (NXSwapInt(arch->cputype) & CPU_TYPE_POWERPC) == CPU_TYPE_POWERPC);
        i++;
        arch++;
    }
}

void    print_path(char *av)
{
    write(1, "\n", 1);
    write(1, av, ft_strlen(av));
    write(1, ":\n", 2);
}

void    nm(void *ptr, char *av, uint8_t mult)
{
    uint32_t    magic_number;
    cpu_type_t  cputype;

    magic_number = *(uint32_t *)ptr;
    cputype = ptr + sizeof(uint32_t);
    
    if (mult > 2 && magic_number != FAT_MAGIC && magic_number != FAT_MAGIC_64 && magic_number != FAT_CIGAM && magic_number != FAT_CIGAM_64)
        print_path(av);
    if (magic_number == MH_MAGIC_64 || magic_number == MH_CIGAM_64)
        handle_64(ptr, (NXSwapInt(cputype) & CPU_TYPE_POWERPC) == CPU_TYPE_POWERPC);
    if (magic_number == MH_MAGIC || magic_number == MH_CIGAM)
        handle_32(ptr, (NXSwapInt(cputype) & CPU_TYPE_POWERPC) == CPU_TYPE_POWERPC);
    if (magic_number == FAT_MAGIC || magic_number == FAT_CIGAM)
        handle_fat_32(ptr, av);
    if (magic_number == FAT_MAGIC_64 || magic_number == FAT_CIGAM_64)
        handle_fat_64(ptr, av);
}

int main(int ac, char **av)
{
    int         fd;
    void        *ptr;
    struct stat buf;
    int         n;

    // if (ac < 2)
    // {
    //     fprintf(stderr, "Please give me an arg\n");
    //     return (EXIT_FAILURE);
    // }
    
    n = 1;
    if (ac < 2)
        n--;
    while (n < ac)
    {
        if ((fd = open(av[n], O_RDONLY)) < 0)
        {
            perror("open");
            n++;
            continue;
            // return (EXIT_FAILURE);
        }
        if (fstat(fd, &buf) < 0)
        {
            perror("fstat");
            return (EXIT_FAILURE);
        }
        if ((ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
        {
            perror("mmap");
            return (EXIT_FAILURE);
        }
        nm(ptr, av[n], ac);
        if (munmap(ptr, buf.st_size) < 0)
        {
            perror("munmap");
            return (EXIT_FAILURE);
        }
        if (close(fd) < 0)
        {
            perror("close");
            return (EXIT_FAILURE);
        }
        n++;
    }
    return(EXIT_SUCCESS);
}
