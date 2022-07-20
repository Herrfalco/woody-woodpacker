#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <elf.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BUFF_SZ		1024

static void		clean_exit(DIR *dir, int fd,  char *msg) {
	if (dir)
		closedir(dir);
	if (fd)
		close(fd);
	if (msg) {
		fprintf(stderr, "Error: %s\n", msg);
		exit(42);
	}
	else
		exit(0);
}

int		main(int ac, char **av) {
	DIR				*d;
	struct dirent	*dir;
	uint64_t		i;
	Elf64_Ehdr		hdr;
	int				src;
	char			file_path[BUFF_SZ];

	if (ac != 3)
		clean_exit(NULL, 0, "./file_by_type [DIR] [TYPE(DYN / EXEC)]");
	if (strcmp("DYN", av[2]) && strcmp("EXEC", av[2]))
		clean_exit(NULL, 0, "./file_by_type [DIR] [TYPE(DYN / EXEC)]");
	if (!(d = opendir(av[1])))
		clean_exit(NULL, 0, "can't open source directory");
	for (i = 0; (dir = readdir(d)) != NULL; ++i) {
		if (dir->d_type == DT_REG) {
			sprintf(file_path, "%s/%s", av[1], dir->d_name);
			if ((src = open(file_path, O_RDONLY)) < 0)
				fprintf(stderr, "File \"%s\": can't be opened\n", dir->d_name);
			if (read(src, &hdr, sizeof(Elf64_Ehdr)) == -1)
				clean_exit(d, src, "can't read source file");
			if (hdr.e_type == ET_EXEC && !strcmp("EXEC", av[2]))
				printf("%s\n", file_path);
			else if (hdr.e_type == ET_DYN && !strcmp("DYN", av[2]))
				printf("%s\n", file_path);
			close(src);
		}
	}
	clean_exit(d, 0, NULL);
	return (0);
}
