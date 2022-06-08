#include "../includes.h"
#include "lzw.h"
#include "../data_rw/data_rw.h"

static void			exit_error(char *str, int fd, int new_fd) {
	write(2, str, strlen(str));
	if (fd)
		close(fd);
	if (new_fd)
		close(new_fd);
	exit(1);
}

void				print_dico(t_dico *dico) {
	size_t		i;

	for (i = 0; i < dico->size; ++i)
		printf("%ld -> %d, %d\n", 256 + i, dico->entry[i][0], dico->entry[i][1]);
}

static void			new_entry(uint16_t last_byte, uint16_t byte, t_dico *dico) {
	size_t	i;

	for (i = 0; i < dico->size; ++i)
		if (dico->entry[i][0] == last_byte && dico->entry[i][1] == byte)
			return ;
	dico->entry[dico->size][0] = last_byte;
	dico->entry[dico->size][1] = byte;
	++dico->size;
}

static int			check_dico(uint16_t last_value, uint16_t value, t_dico *dico) {
	size_t	i;

	for (i = 0; i < dico->size; ++i)
		if (last_value == dico->entry[i][0] && value == dico->entry[i][1])
			return (i + 1);
	return (0);
}

static uint16_t		entry_writer(int fd, uint16_t value, t_dico *dico) {
	uint16_t	ret;

	if (dico->entry[value - 256][0] > 255)
		ret = entry_writer(fd, dico->entry[value - 256][0], dico);
	else {
		file_writer(fd, dico->entry[value - 256][0], NO_FLUSH);
		ret = dico->entry[value - 256][0];
	}
	file_writer(fd, dico->entry[value - 256][1], NO_FLUSH);
	return (ret);
}

static uint16_t		find_first_pattern(uint16_t value, t_dico *dico) {
	if (dico->entry[value - 256][0] > 255)
		return (find_first_pattern(dico->entry[value - 256][0], dico));
	else 
		return (dico->entry[value - 256][0]);
}

static void			not_in_dico(uint16_t last_value, t_dico *dico) {
	if (last_value <= 255)
		new_entry(last_value, last_value, dico);
	else
		new_entry(last_value, find_first_pattern(last_value, dico), dico);
}

static size_t		lzw_chunk(int fd, int new_fd) {
	t_dico		dico = { 0 };
	uint16_t	byte, last_byte;
	ssize_t		i;

	last_byte = 0;
	if (!file_reader(fd, (uint8_t *)&last_byte))
		return (dico.size);
	while (file_reader(fd, (uint8_t *)&byte) && dico.size < DICO_SIZE) {
		if ((i = check_dico(last_byte, byte, &dico)) > 0)
			last_byte = 255 + i;
		else {
			value_writer(new_fd, last_byte, 12, NO_FLUSH);
			new_entry(last_byte, byte, &dico);
			last_byte = byte;
		}
	}
	value_writer(new_fd, last_byte, 12, NO_FLUSH);
	return (dico.size);
}

static void			lzw(int fd, int new_fd) {
	while (lzw_chunk(fd, new_fd) == DICO_SIZE - 4);
	value_writer(new_fd, 0, 12, ONLY_FLUSH);
}

static size_t	unlzw_chunk(int fd, int new_fd) {
	t_dico		dico = { 0 };
	uint16_t	value, last_value, first;

	if (!value_reader(fd, &last_value, 12))
		return (dico.size);
	file_writer(new_fd, last_value, NO_FLUSH);
	for (; value_reader(fd, &value, 12) && dico.size < DICO_SIZE; last_value = value) {
		if (value > 255) {
			if (value > dico.size + 255)
				not_in_dico(last_value, &dico);
			first = entry_writer(new_fd, value, &dico);
			new_entry(last_value, first, &dico);
		} else {
			file_writer(new_fd, value, NO_FLUSH);
			new_entry(last_value, value, &dico);
		}
	}
	return (dico.size);
}

static void		unlzw(int fd, int new_fd) {
	while (unlzw_chunk(fd, new_fd) >= DICO_SIZE);
	file_writer(new_fd, 0, ONLY_FLUSH);
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
	} else {
		if ((new_fd = open(strcat(av[2], ".unlzw"), O_WRONLY | O_TRUNC | O_CREAT, 0644)) == -1)
			exit_error("Error: Couldn't create new file.", fd, new_fd);
		unlzw(fd, new_fd);
	}
	close(fd);
	close(new_fd);
	return (0);
}
