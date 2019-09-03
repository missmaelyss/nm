#include <nm_otool.h>

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

int     ft_strlen(char *str)
{
    int n;

    n = 0;
    while (str[n] != 0)
        n++;
    return (n);
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
