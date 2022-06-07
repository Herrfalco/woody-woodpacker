#include "../includes.h"
#include "lzw.h"
#include "../data_rw/data_rw.h"

static void		exit_error(char *str, int fd, int new_fd) {
	write(2, str, strlen(str));
	if (fd)
		close(fd);
	if (new_fd)
		close(new_fd);
	exit(1);
}

static void		new_entry(uint16_t last_byte, uint8_t byte, t_dico *dico) {
	dico->entry[dico->size][0] = last_byte;
	dico->entry[dico->size][1] = byte;
	dico->size++;
}

static int		check_dico(uint16_t last_value, uint16_t value, t_dico dico) {
	for (size_t i = 0; i < dico.size; i++)
		if (last_value == dico.entry[i][0] && value == dico.entry[i][1])
			return (i + 1);
	return (0);
}

static uint16_t		entry_writer(int fd, uint16_t value, t_dico dico, bool_t last_value) {
	uint16_t	ret;

	if (dico.entry[value - 257][0] > 256)
		entry_writer(fd, dico.entry[value - 257][0], dico, FALSE);
	else {
		value_writer(fd, dico.entry[value - 257][0], 8, FALSE);
		ret = dico.entry[value - 257][0];
	}
	value_writer(fd, dico.entry[value - 257][1], 8, last_value);
	return (ret);
}

static void		lzw(int fd, int new_fd) {
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
		if ((i = check_dico(last_byte, (uint16_t)byte, dico)) > 0) {
			last_byte = 256 + i;
			continue ;
		}
		value_writer(new_fd, last_byte, 12, FALSE);
		new_entry(last_byte, byte, &dico);
		last_byte = byte;
	}
	value_writer(new_fd, last_byte, 12, TRUE);
}

static void		unlzw(int fd, int new_fd) {
	uint16_t	value, last_value, first;
	t_dico		dico = { 0 };
	size_t		i = 0;

	while (value_reader(fd, &value, 12) > 0) {
		if (dico.size == 0) {
			dico.entry[0][i] = value;
			value_writer(new_fd, value, 8, FALSE);
			i == 0 ? i++ : dico.size++;
			last_value = value;
			continue ;
		}
		if (value > 256) {
			first = entry_writer(new_fd, value, dico, FALSE);
			new_entry(last_value, first, &dico);
			last_value = value;
		}
		else {
			value_writer(new_fd, value, 8, FALSE);
			new_entry(last_value, value, &dico);
			last_value = value;
		}
	}
	value_writer(new_fd, (uint16_t)'\0', 8, TRUE);
}

int				main(int ac, char **av) {
	int	fd = 0;
	int	new_fd = 0;

	if (ac != 3)
		exit_error("USAGE: ./[exe] [-c/-u] [file]", fd, new_fd);
	if ((fd = open(av[2], O_RDONLY)) == -1)
		exit_error("Error: Couldn't open file.", fd, new_fd);
	if (!strcmp(av[1], "-c")) {
		if ((new_fd = open(strcat(av[2], ".lzw"), O_WRONLY | O_TRUNC | O_CREAT, 0644)) == -1) 
			exit_error("Error: Couldn't create new file.", fd, new_fd);
		lzw(fd, new_fd);
	}
	else {
		if ((new_fd = open(strcat(av[2], ".unlzw"), O_WRONLY | O_TRUNC | O_CREAT, 0644)) == -1)
			exit_error("Error: Couldn't create new file.", fd, new_fd);
		unlzw(fd, new_fd);
	}
	close(fd);
	close(new_fd);
	return (0);
}
