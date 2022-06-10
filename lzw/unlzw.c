#include "../includes.h"
#include "lzw.h"
#include "../data_rw/data_rw.h"

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

void		unlzw(int fd) {
	int new_fd = 0;

	lseek(fd, 0, SEEK_SET);
	if ((new_fd = open("uncompressed_file", O_WRONLY | O_TRUNC | O_CREAT, 0777)) == -1)
		quit_fd(fd, "Can not create compressed file.");
	while (unlzw_chunk(fd, new_fd) >= DICO_SIZE);
	file_writer(new_fd, 0, ONLY_FLUSH);
}
