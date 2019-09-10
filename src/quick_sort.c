#include <nm_otool.h>

void swap_tab(int *tableau, int a, int b)
{
    int temp = tableau[a];
    tableau[a] = tableau[b];
    tableau[b] = temp;
}

void quickSort_64(int *tableau, int debut, int fin, char *str, struct nlist_64 *el, uint32_t magic)
{
    int gauche = debut;
    int droite = fin;
    const char *pivot = (debut >= fin) ? NULL : ft_strdup(str + swap_endian(el[tableau[debut]].n_un.n_strx, magic));
    const uint32_t   pivot_value = (debut >= fin) ? 0 : swap_endian(el[tableau[debut]].n_value, magic);

    if (debut >= fin)
        return;
    while(1)
    {
        while(ft_strcmp(str + swap_endian(el[tableau[droite]].n_un.n_strx, magic), pivot) > 0 || 
            (ft_strcmp(str + swap_endian(el[tableau[droite]].n_un.n_strx, magic), pivot) == 0 && 
                swap_endian(el[tableau[droite]].n_value, magic) > pivot_value))
            droite--;
        while(ft_strcmp(str + swap_endian(el[tableau[gauche]].n_un.n_strx, magic), pivot) < 0 ||
            (ft_strcmp(str + swap_endian(el[tableau[gauche]].n_un.n_strx, magic), pivot) == 0 && 
                swap_endian(el[tableau[gauche]].n_value, magic) < pivot_value))
            gauche++;

        if (ft_strcmp(str + swap_endian(el[tableau[gauche]].n_un.n_strx, magic), str + swap_endian(el[tableau[droite]].n_un.n_strx, magic)) > 0 ||
            (ft_strcmp(str + swap_endian(el[tableau[gauche]].n_un.n_strx, magic), str + swap_endian(el[tableau[droite]].n_un.n_strx, magic)) == 0 && 
                swap_endian(el[tableau[gauche]].n_value, magic) > swap_endian(el[tableau[droite]].n_value, magic)))
            swap_tab(tableau, gauche, droite);
        else break;
    }
    if (pivot)
        free(pivot);
    quickSort_64(tableau, debut, droite, str, el, magic);
    quickSort_64(tableau, droite+1, fin, str, el, magic);
}

void quickSort_32(int *tableau, int debut, int fin, char *str, struct nlist *el, uint32_t magic)
{
    int gauche = debut;
    int droite = fin;
    const char *pivot = (debut >= fin) ? NULL : ft_strdup(str + swap_endian(el[tableau[debut]].n_un.n_strx, magic));
    const uint32_t   pivot_value = (debut >= fin) ? 0 : swap_endian(el[tableau[debut]].n_value, magic);

    if (debut >= fin)
        return;
    while(1)
    {
        while(ft_strcmp(str + swap_endian(el[tableau[droite]].n_un.n_strx, magic), pivot) > 0 || 
            (ft_strcmp(str + swap_endian(el[tableau[droite]].n_un.n_strx, magic), pivot) == 0 && 
                swap_endian(el[tableau[droite]].n_value, magic) > pivot_value))
            droite--;
        while(ft_strcmp(str + swap_endian(el[tableau[gauche]].n_un.n_strx, magic), pivot) < 0 ||
            (ft_strcmp(str + swap_endian(el[tableau[gauche]].n_un.n_strx, magic), pivot) == 0 && 
                swap_endian(el[tableau[gauche]].n_value, magic) < pivot_value))
            gauche++;
        if (ft_strcmp(str + swap_endian(el[tableau[gauche]].n_un.n_strx, magic), str + swap_endian(el[tableau[droite]].n_un.n_strx, magic)) > 0 ||
            (ft_strcmp(str + swap_endian(el[tableau[gauche]].n_un.n_strx, magic), str + swap_endian(el[tableau[droite]].n_un.n_strx, magic)) == 0 && 
                swap_endian(el[tableau[gauche]].n_value, magic) > swap_endian(el[tableau[droite]].n_value, magic)))
            swap_tab(tableau, gauche, droite);
        else break;
    }
    if (pivot)
        free(pivot);
    quickSort_32(tableau, debut, droite, str, el, magic);
    quickSort_32(tableau, droite+1, fin, str, el, magic);
}

int    *find_order_64(int nsyms, char *str, struct nlist_64 *el, uint32_t magic)
{
    int     *itab;
    int     i;
    int     good_sym;

    good_sym = nb_good_sym_64(nsyms, el, magic);
    
    if (!(itab = (int *)malloc(good_sym * sizeof(int))))
        exit (-1);
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
    quickSort_64(itab, 0, good_sym - 1, str, el, magic);
    return (itab);
}

int    *find_order_32(int nsyms, char *str, struct nlist *el, uint32_t magic)
{
    int     *itab;
    int     i;
    int     good_sym;

    good_sym = nb_good_sym_32(nsyms, el, magic);
    if (!(itab = (int *)malloc(good_sym * sizeof(int))))
        exit (-1);
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