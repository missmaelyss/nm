#include <nm_otool.h>

void    handle_lyb(struct s_file_ptr *ptr, char *av)
{
    struct  ar_hdr *header;

    header = (struct ar_hdr *)(ptr->ptr + 0x8);
    while (atoi(header->ar_size))
    {
        if ((void *)header > ptr->max_ptr)
        {
            printf("Error\n");
            return ;
        }       
        if (ft_strcmp(header->ar_fmag, "`\n__.SYMDEF SORTED") != 0 && ft_strcmp(header->ar_fmag, "`\n__.SYMDEF"))
        {
            write(1, "\n", 1);
            write(1, av, ft_strlen(av));
            write(1, "(", 1);
            write(1, &((header->ar_fmag)[2]), ft_strlen(&((header->ar_fmag)[2])));
            write(1, "):\n", 3);
            nm(&(struct s_file_ptr){(void *)(header->ar_fmag) + 2 + (ft_strlen(&((header->ar_fmag)[2])) / 8 + (ft_strlen(&((header->ar_fmag)[2])) % 8 > 0 ? 1 : 0)) * 0x8 + 0x4, ptr->max_ptr}, av, 1);
        }
        ptr->ptr = (void *)(header->ar_fmag) + 2 + atoi(header->ar_size);
        if (OSSwapConstInt64(*(uint64_t *)(ptr->ptr)) == 0x213C617263683E0A)
            break ;
        header = (struct ar_hdr *)(ptr->ptr);
    }
}

void    handle_64(struct s_file_ptr *ptr, uint8_t ppc)
{
    uint32_t                ncmds;
    uint32_t                i;
    struct mach_header_64   *header;
    struct load_command     *lc;
    struct symtab_command   *sym;
    
    fill_section_64(ptr, ppc);
    header = (struct mach_header_64 *)(ptr->ptr);
    ncmds = header->ncmds;
    lc = ptr->ptr + sizeof(struct mach_header_64);
    i = 0;
    while (i < swap_endian(ncmds, header->magic))
    {
        if ((void *)lc > ptr->max_ptr)
        {
            printf("Error\n");
            return ;
        } 
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

void    handle_32(struct s_file_ptr *ptr, uint8_t ppc)
{
    uint32_t                ncmds;
    uint32_t                i;
    struct mach_header      *header;
    struct load_command     *lc;
    struct symtab_command   *sym;

    fill_section_32(ptr, ppc);
    header = (struct mach_header *)(ptr->ptr);
    ncmds = header->ncmds;
    lc = ptr->ptr + sizeof(struct mach_header);
    i = 0;
    while (i < swap_endian(ncmds, header->magic))
    {
        if ((void *)lc > ptr->max_ptr)
        {
            printf("Error\n");
            return ;
        } 
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

void    handle_fat_64(struct s_file_ptr *ptr, char *av)
{
    uint32_t            i;
    struct fat_header   *header;
    struct fat_arch_64     *arch;

    i = 0;
    header = (struct fat_header *)(ptr->ptr);
    arch = ptr->ptr + sizeof(struct fat_header);
    while (i < OSSwapConstInt32(header->nfat_arch))
    {
        if ((void *)arch > ptr->max_ptr)
        {
            printf("Error\n");
            return ;
        } 
        write(1, "\n", 1);
        write(1, av, ft_strlen(av));
        if (OSSwapConstInt32(header->nfat_arch) > 1)
            print_for_arch((OSSwapConstInt64(arch->cputype)));
        write(1, "\n", 1);
        if ((OSSwapConstInt64(arch->cputype) & CPU_ARCH_ABI64) == CPU_ARCH_ABI64)
            handle_64(ptr + OSSwapConstInt64(arch->offset), (OSSwapConstInt64(arch->cputype) & CPU_TYPE_POWERPC) == CPU_TYPE_POWERPC);
        else
            handle_32(ptr + OSSwapConstInt64(arch->offset), (OSSwapConstInt64(arch->cputype) & CPU_TYPE_POWERPC) == CPU_TYPE_POWERPC);
        i++;
        arch++;
    }
}

void    handle_fat_32(struct s_file_ptr *ptr, char *av)
{
    uint32_t            i;
    struct fat_header   *header;
    struct fat_arch     *arch;

    i = 0;
    header = (struct fat_header *)(ptr->ptr);
    arch = ptr->ptr + sizeof(struct fat_header);
    while (i < OSSwapConstInt32(header->nfat_arch))
    {
        if ((void *)arch > ptr->max_ptr)
        {
            printf("Error\n");
            return ;
        }
        write(1, "\n", 1);
        write(1, av, ft_strlen(av));
        if (OSSwapConstInt32(header->nfat_arch) > 1)
            print_for_arch((OSSwapConstInt32(arch->cputype)));
        write(1, "\n", 1);
        
        if ((OSSwapConstInt32(arch->cputype) & CPU_ARCH_ABI64) == CPU_ARCH_ABI64)
        {
            if (OSSwapConstInt64(*(uint64_t *)(ptr->ptr + OSSwapConstInt32(arch->offset))) == 0x213C617263683E0A || *(uint64_t *)(ptr->ptr + OSSwapConstInt32(arch->offset)) == 0x213C617263683E0A)
                handle_lyb(&(struct s_file_ptr){ptr->ptr + OSSwapConstInt32(arch->offset), ptr->max_ptr}, av);
            else
                handle_64(&(struct s_file_ptr){ptr->ptr + OSSwapConstInt32(arch->offset), ptr->max_ptr}, (OSSwapConstInt32(arch->cputype) & CPU_TYPE_POWERPC) == CPU_TYPE_POWERPC);
        }
        else
        {
            if (OSSwapConstInt64(*(uint64_t *)(ptr->ptr + OSSwapConstInt32(arch->offset))) == 0x213C617263683E0A || *(uint64_t *)(ptr->ptr + OSSwapConstInt32(arch->offset)) == 0x213C617263683E0A)
                handle_lyb(&(struct s_file_ptr){ptr->ptr + OSSwapConstInt32(arch->offset), ptr->max_ptr}, av);
            else
                handle_32(&(struct s_file_ptr){ptr->ptr + OSSwapConstInt32(arch->offset), ptr->max_ptr}, (OSSwapConstInt32(arch->cputype) & CPU_TYPE_POWERPC) == CPU_TYPE_POWERPC);
        }
        i++;
        arch++;
    }
}