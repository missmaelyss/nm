#include <nm_otool.h>

char*   g_cpu_type_tab[18] = {"vax", "", "", "", "", "mc680x0", "x86", "", "", "mc98000", "hppa", "arm", "mc88000", "sparc", "i860", "", "", "ppc"};
char   g_section_symbol[30] = "";

void    print_for_arch(cpu_type_t cputype)
{
    write(1, " (for architecture ", 19);
    if ((cputype & CPU_TYPE_X86) == CPU_TYPE_X86 && (cputype & CPU_ARCH_ABI64) != CPU_ARCH_ABI64)
        write(1, "i386", 4);
    else if ((cputype & CPU_ARCH_ABI64) == CPU_ARCH_ABI64)
        write(1, g_cpu_type_tab[cputype - CPU_ARCH_ABI64 - 1], ft_strlen(g_cpu_type_tab[cputype - CPU_ARCH_ABI64 - 1]));
    else
        write(1, g_cpu_type_tab[cputype - 1], ft_strlen(g_cpu_type_tab[cputype - 1]));
    if ((cputype & CPU_ARCH_ABI64) == CPU_ARCH_ABI64)
        write(1, "_64", 3);
    write(1, "):", 2);

}

void    print_path(char *av)
{
    write(1, "\n", 1);
    write(1, av, ft_strlen(av));
    write(1, ":\n", 2);
}

void    fill_section_32(void *ptr, uint8_t ppc)
{
    uint32_t                ncmds;
    uint32_t                i;
    struct mach_header      *header;
    struct load_command     *lc;
    int                     i_tab;

    header = (struct mach_header *)ptr;
    ncmds = header->ncmds;
    lc = ptr + sizeof(struct mach_header);
    i = 0;
    i_tab = 0;
    while (i < swap_endian(ncmds, header->magic))
    {
        if (swap_endian(lc->cmd, header->magic) == LC_SEGMENT)
        {
        
            struct segment_command   *seg;
            struct section           *sect;
            uint32_t                    n;

            seg = (void *)lc;
            n = 0;
            // printf("#%d: LC_SEGMENT-%s\n", i, seg->segname);
            sect = (void *)seg + sizeof(struct segment_command);
            
            while (n < swap_endian(seg->nsects, header->magic))
            {
                // printf("\t#%d: %s\n", n, sect->sectname);
                if (ft_strcmp(sect->sectname,"__text") == 0)
                {
                    g_section_symbol[i_tab] = 't';
                    i_tab++;
                }
                else if (ft_strcmp(sect->sectname,"__data") == 0)
                {
                    g_section_symbol[i_tab] = 'd';
                    i_tab++;
                }
                else if (ft_strcmp(sect->sectname,"__bss") == 0)
                {
                    g_section_symbol[i_tab] = 'b';
                    i_tab++;
                }
                // else if (ft_strcmp(sect->sectname,"__common") == 0)
                // {
                //     g_section_symbol[i_tab] = 'c';
                //     i_tab++;
                // }
                else
                {
                    g_section_symbol[i_tab] = 's';
                    i_tab++;
                }
                n++;
                sect++;
            }
        }
        lc = (void *)lc + swap_endian(lc->cmdsize, header->magic);
        i++;
    }

    // int n;
    // n = 0;
    // while (n < ft_strlen(g_section_symbol))
    // {
    //     printf("%d: %c\n", n, g_section_symbol[n]);
    //     n++;
    // }
}


void    fill_section_64(void *ptr, uint8_t ppc)
{
    uint32_t                ncmds;
    uint32_t                i;
    struct mach_header_64   *header;
    struct load_command     *lc;
    int                     i_tab;

    header = (struct mach_header_64 *)ptr;
    ncmds = header->ncmds;
    lc = ptr + sizeof(struct mach_header_64);
    i = 0;
    i_tab = 0;
    while (i < swap_endian(ncmds, header->magic))
    {
        if (swap_endian(lc->cmd, header->magic) == LC_SEGMENT_64)
        {
        
            struct segment_command_64   *seg;
            struct section_64           *sect;
            uint32_t                    n;

            seg = (void *)lc;
            n = 0;
            // printf("#%d: LC_SEGMENT-%s\n", i, seg->segname);
            sect = (void *)seg + sizeof(struct segment_command_64);
            
            while (n < seg->nsects)
            {
                // printf("\t#%d: %s\n", n, sect->sectname);
                if (ft_strcmp(sect->sectname,"__text") == 0)
                {
                    g_section_symbol[i_tab] = 't';
                    i_tab++;
                }
                else if (ft_strcmp(sect->sectname,"__data") == 0)
                {
                    g_section_symbol[i_tab] = 'd';
                    i_tab++;
                }
                else if (ft_strcmp(sect->sectname,"__bss") == 0)
                {
                    g_section_symbol[i_tab] = 'b';
                    i_tab++;
                }
                // else if (ft_strcmp(sect->sectname,"__common") == 0)
                // {
                //     g_section_symbol[i_tab] = 'c';
                //     i_tab++;
                // }
                else
                {
                    g_section_symbol[i_tab] = 's';
                    i_tab++;
                }
                n++;
                sect++;
            }
        }
        lc = (void *)lc + swap_endian(lc->cmdsize, header->magic);
        i++;
    }

    // int n;
    // n = 0;
    // while (n < 30)
    // {
    //     printf("%d: %c\n", n, g_section_symbol[n]);
    //     n++;
    // }
}

void    nm(void *ptr, char *av, uint8_t mult)
{
    uint32_t    magic_number;
    cpu_type_t  cputype;
    uint32_t    filetype;

    magic_number = *(uint32_t *)ptr;
    cputype = (cpu_type_t)(ptr + sizeof(uint32_t));
    filetype = *(uint32_t *)(ptr + sizeof(uint32_t) + sizeof(cpu_type_t) + sizeof(cpu_subtype_t));

    // printf("Debut de nm, ptr: %x\n", ptr);
    if (OSSwapConstInt64(*(uint64_t *)ptr) == 0x213C617263683E0A || *(uint64_t *)ptr == 0x213C617263683E0A)
    {
        handle_lyb(ptr, av);
        return ;
    }
    filetype = ((magic_number == MH_CIGAM || magic_number == MH_CIGAM_64) ? OSSwapConstInt32(filetype) : filetype);
    if (!(filetype == 0x0 || filetype == 0x1 || filetype ==  0x2 || filetype == 0x6 || filetype == 0x3000080 || filetype == 0x3000000))
        return ;
    if (mult > 2 && magic_number != FAT_MAGIC && magic_number != FAT_MAGIC_64 && magic_number != FAT_CIGAM && magic_number != FAT_CIGAM_64)
        print_path(av);
    if (magic_number == MH_MAGIC_64 || magic_number == MH_CIGAM_64)
        handle_64(ptr, (OSSwapConstInt32(cputype) & CPU_TYPE_POWERPC) == CPU_TYPE_POWERPC);
    if (magic_number == MH_MAGIC || magic_number == MH_CIGAM)
        handle_32(ptr, (OSSwapConstInt32(cputype) & CPU_TYPE_POWERPC) == CPU_TYPE_POWERPC);
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

    n = 0;
    if (ac < 2)
        n--;
    while (++n < ac)
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
            if (close(fd) < 0)
            {
                perror("close");
                return (EXIT_FAILURE);
            }
            continue;
            // return (EXIT_FAILURE);
        }
        if ((ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
        {
            perror("mmap");
            if (close(fd) < 0)
            {
                perror("close");
                return (EXIT_FAILURE);
            }
            continue;
            // return (EXIT_FAILURE);
        }
        nm(ptr, av[n], ac);
        if (munmap(ptr, buf.st_size) < 0)
        {
            perror("munmap");
            if (close(fd) < 0)
            {
                perror("close");
                return (EXIT_FAILURE);
            }
            continue;
            // return (EXIT_FAILURE);
        }
        if (close(fd) < 0)
        {
            perror("close");
            // continue;
            return (EXIT_FAILURE);
        }
        // n++;
    }
    return(EXIT_SUCCESS);
}
