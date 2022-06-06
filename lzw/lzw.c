#include "../includes.h"
#include "lzw.h"
#include "../data_rw/data_rw.h"

static void		exit_error(char *str, int fd, int lzw_fd) {
	write(2, str, strlen(str));
	if (fd)
		close(fd);
	if (lzw_fd)
		close(lzw_fd);
	exit(1);
}

static void		new_entry(uint16_t last_byte, uint8_t byte, t_dico *dico) {
	dico->entry[dico->size][0] = last_byte;
	dico->entry[dico->size][1] = byte;
	dico->size++;
}

static int		check_dico(uint16_t last_byte, uint8_t byte, t_dico dico) {
	for (size_t i = 0; i < dico.size; i++)
		if (last_byte == dico.entry[i][0] && byte == dico.entry[i][1])
			return (i + 1);
	return (0);
}

static void		lzw(int fd, int lzw_fd) {
	uint8_t		byte;
	uint16_t	last_byte;
	t_dico		dico = { 0 };
	int			i = -1;

	while (file_reader(fd, &byte) > 0) {
		if (i < 0) {
			last_byte = byte;
			i++;
			continue;
		}
		if ((i = check_dico(last_byte, byte, dico)) > 0) {
			last_byte = 256 + i;
			continue ;
		}
		value_writer(lzw_fd, last_byte, 12, FALSE);
		new_entry(last_byte, byte, &dico);
		last_byte = byte;
	}
	value_writer(lzw_fd, last_byte, 12, TRUE);
}

int				main(int ac, char **av) {
	int	fd;
	int	lzw_fd;

	if (ac != 2)
		exit_error("USAGE: ./[exe] [file]", fd, lzw_fd);
	if ((fd = open(av[1], O_RDONLY)) == -1)
		exit_error("Error: Couldn't open file.", fd, lzw_fd);
	if ((lzw_fd = open(strcat(av[1], ".lzw"), O_WRONLY | O_TRUNC | O_CREAT, 0644)) == -1) 
		exit_error("Error: Couldn't create new file.", fd, lzw_fd);
	lzw(fd, lzw_fd);
	close(fd);
	close(lzw_fd);
	return (0);
}
