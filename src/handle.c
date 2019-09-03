#include <nm_otool.h>

void    handle_64(void *ptr, uint8_t ppc)
{
    uint32_t                ncmds;
    uint32_t                i;
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
    uint32_t                ncmds;
    uint32_t                i;
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

void    handle_fat_64(void *ptr, char *av)
{
    uint32_t            i;
    struct fat_header   *header;
    struct fat_arch_64     *arch;

    i = 0;
    header = (struct fat_header *)ptr;
    arch = ptr + sizeof(struct fat_header);
    while (i < OSSwapConstInt32(header->nfat_arch))
    {
        write(1, "\n", 1);
        write(1, av, ft_strlen(av));
        if (OSSwapConstInt32(header->nfat_arch) > 1)
            print_for_arch((OSSwapConstInt32(arch->cputype)));
        write(1, "\n", 1);
        if ((OSSwapConstInt32(arch->cputype) & CPU_ARCH_ABI64) == CPU_ARCH_ABI64)
            handle_64(ptr + OSSwapConstInt32(arch->offset), (OSSwapConstInt32(arch->cputype) & CPU_TYPE_POWERPC) == CPU_TYPE_POWERPC);
        else
            handle_32(ptr + OSSwapConstInt32(arch->offset), (OSSwapConstInt32(arch->cputype) & CPU_TYPE_POWERPC) == CPU_TYPE_POWERPC);
        i++;
        arch++;
    }
}

void    handle_fat_32(void *ptr, char *av)
{
    uint32_t            i;
    struct fat_header   *header;
    struct fat_arch     *arch;

    i = 0;
    header = (struct fat_header *)ptr;
    arch = ptr + sizeof(struct fat_header);
    while (i < OSSwapConstInt32(header->nfat_arch))
    {
        write(1, "\n", 1);
        write(1, av, ft_strlen(av));
        if (OSSwapConstInt32(header->nfat_arch) > 1)
            print_for_arch((OSSwapConstInt32(arch->cputype)));
        write(1, "\n", 1);
        if ((OSSwapConstInt32(arch->cputype) & CPU_ARCH_ABI64) == CPU_ARCH_ABI64)
            handle_64(ptr + OSSwapConstInt32(arch->offset), (OSSwapConstInt32(arch->cputype) & CPU_TYPE_POWERPC) == CPU_TYPE_POWERPC);
        else
            handle_32(ptr + OSSwapConstInt32(arch->offset), (OSSwapConstInt32(arch->cputype) & CPU_TYPE_POWERPC) == CPU_TYPE_POWERPC);
        i++;
        arch++;
    }
}