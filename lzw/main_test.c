#include "../includes.h"
#include "../test_utils.h"
#include "lzw.h"

int		main(int ac, char **av) {
	int	fd = 0;
	int	new_fd = 0;

	if (ac != 2)
		quit("1 arg needed to set fd size");
	rand_v_file(&fd, atoi(av[1]));
	new_fd = lzw(fd);
	printf("%ld\n", get_fd_size(new_fd));
	unlzw(new_fd);
	if ((new_fd = open("uncompressed_file", O_RDONLY)) == -1)
		quit("can't open uncompressed file");
	if (diff_v_files(fd, new_fd))
		quit_2_fd(fd, new_fd, "files are differents");
	else
		quit_2_fd(fd, new_fd, "files are equals");
	return (0);
}
