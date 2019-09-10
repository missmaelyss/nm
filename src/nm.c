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

void    fill_section_32(struct s_file_ptr *ptr, uint8_t ppc)
{
    uint32_t                ncmds;
    uint32_t                i;
    struct mach_header      *header;
    struct load_command     *lc;
    int                     i_tab;

    header = (struct mach_header *)(ptr->ptr);
    ncmds = header->ncmds;
    lc = (ptr->ptr) + sizeof(struct mach_header);
    i = 0;
    i_tab = 0;
    while (i < swap_endian(ncmds, header->magic))
    {
        if ((void *)lc > ptr->max_ptr)
        {
            printf("Error\n");
            return ;
        }
        if (swap_endian(lc->cmd, header->magic) == LC_SEGMENT)
        {
        
            struct segment_command   *seg;
            struct section           *sect;
            uint32_t                    n;

            seg = (void *)lc;
            n = 0;
            sect = (void *)seg + sizeof(struct segment_command);
            if ((void *)sect > ptr->max_ptr)
            {
                printf("Error\n");
                return ;
            } 
            while (n < swap_endian(seg->nsects, header->magic))
            {
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
}


void    fill_section_64(struct s_file_ptr *ptr, uint8_t ppc)
{
    uint32_t                ncmds;
    uint32_t                i;
    struct mach_header_64   *header;
    struct load_command     *lc;
    int                     i_tab;

    header = (struct mach_header_64 *)(ptr->ptr);
    ncmds = header->ncmds;
    lc = (ptr->ptr) + sizeof(struct mach_header_64);
    i = 0;
    i_tab = 0;
    while (i < swap_endian(ncmds, header->magic))
    {
        if ((void *)lc > ptr->max_ptr)
        {
            printf("Error\n");
            return ;
        }
        if (swap_endian(lc->cmd, header->magic) == LC_SEGMENT_64)
        {
        
            struct segment_command_64   *seg;
            struct section_64           *sect;
            uint32_t                    n;

            seg = (void *)lc;
            n = 0;
            sect = (void *)seg + sizeof(struct segment_command_64);
            
            while (n < seg->nsects)
            {
                if ((void *)sect > ptr->max_ptr)
                {
                    printf("Error\n");
                    return ;
                }   
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
}

void    nm(struct s_file_ptr *ptr, char *av, uint8_t mult)
{
    uint32_t    magic_number;
    cpu_type_t  cputype;
    uint32_t    filetype;

    magic_number = *(uint32_t *)(ptr->ptr);
    cputype = (cpu_type_t)((ptr->ptr) + sizeof(uint32_t));
    filetype = *(uint32_t *)((ptr->ptr) + sizeof(uint32_t) + sizeof(cpu_type_t) + sizeof(cpu_subtype_t));

    if (OSSwapConstInt64(*(uint64_t *)(ptr->ptr)) == 0x213C617263683E0A || *(uint64_t *)(ptr->ptr) == 0x213C617263683E0A)
    {
        handle_lyb(ptr, av);
        return ;
    }
    filetype = ((magic_number == MH_CIGAM || magic_number == MH_CIGAM_64) ? OSSwapConstInt32(filetype) : filetype);
    if (!(filetype == 0x0 || filetype == 0x1 || filetype ==  0x2 || filetype == 0x6 || filetype == 0x3000080 || filetype == 0x3000000))
        return (ft_error(av, "The file was not recognized as a valid object file\n"));
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

void    ft_error(char *av, char *error)
{
    write(2, av, ft_strlen(av));
    write(2, ": ", 2);
    write(2, error, ft_strlen(error));
    write(2, "\n", 1);
}

int main(int ac, char **av)
{
    int                 fd;
    int                 n;
    struct stat         buf;
    struct s_file_ptr   ptr;

    n = 0;
    if (ac < 2)
        n--;
    while (++n < ac)
    {
        if ((fd = open(av[n], O_RDONLY)) < 0)
        {
            ft_error(av[n], "Permission denied.");
            continue;
        }
        if (fstat(fd, &buf) < 0)
        {
            ft_error(av[n], "Permission denied.");
            close(fd);
            continue;
        }
        if ((ptr.ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
        {
            ft_error(av[n], "Is a directory.");
            close(fd);
            continue;
        }
        ptr.max_ptr = ptr.ptr + buf.st_size;
        nm(&ptr, av[n], ac);
        if (munmap(ptr.ptr, buf.st_size) < 0)
        {
            close(fd);
            continue;
        }
        close(fd);
    }
    return (EXIT_SUCCESS);
}
