#include <nm_otool.h>

uint16_t    swap_endian_short(uint16_t value, uint32_t magic)
{
    if (magic == MH_CIGAM || magic == MH_CIGAM_64)
        return (OSSwapConstInt16(value));
    return (value);
}

uint32_t    swap_endian(uint32_t value, uint32_t magic)
{
    if (magic == MH_CIGAM || magic == MH_CIGAM_64)
        return (OSSwapConstInt32(value));
    return (value);
}

uint64_t    swap_endian_64(uint64_t value, uint32_t magic)
{
    if (magic == MH_CIGAM || magic == MH_CIGAM_64)
        return (OSSwapConstInt64(value));
    return (value);
}

int     nb_good_sym_64(int nsyms, struct nlist_64 *el, uint32_t magic)
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

int     nb_good_sym_32(int nsyms, struct nlist *el, uint32_t magic)
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