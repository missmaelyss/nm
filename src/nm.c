#include <nm_otool.h>

char* g_cpu_type_tab[18] = {"vax", "", "", "", "", "mc680x0", "x86", "", "", "mc98000", "hppa", "arm", "mc88000", "sparc", "i860", "", "", "ppc"};

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

void    nm(void *ptr, char *av, uint8_t mult)
{
    uint32_t    magic_number;
    cpu_type_t  cputype;

    magic_number = *(uint32_t *)ptr;
    cputype = (cpu_type_t)(ptr + sizeof(uint32_t));
    
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
