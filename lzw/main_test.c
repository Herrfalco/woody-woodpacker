#include "../includes.h"
#include "lzw.h"
/*
static int		str_cmp(char *s1, char *s2) {
	while (*s1 || *s2) {
		if (*s1 != *s2)
			return (1);
		s1++;
		s2++;
	}
	return (0);
	
}
*/
int		main(int ac, char **av) {
	int	fd = 0;
	int	new_fd = 0;

	if (ac != 2)
		quit("2 args needed.");
	if ((fd = open(av[1], O_RDONLY)) == -1)
		quit("Can not open entry file.");
	new_fd = lzw(fd);
	unlzw(new_fd);
	return (0);
}
