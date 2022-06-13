#include "../includes.h"
#include "lzw.h"
#include "../data_rw/data_rw.h"

static size_t	unlzw_chunk(int fd, int new_fd, int64_t *file_sz) {
	t_dico		dico = { 0 };
	uint16_t	value, last_value, first;

	if (value_reader(fd, &last_value, 12) < 0)
		quit_2_fd(fd, new_fd, "can't read file");
	file_writer(new_fd, last_value, NO_FLUSH);
	for (; *file_sz && value_reader(fd, &value, 12) > 0
			&& dico.size < DICO_SIZE;
			last_value = value, *file_sz = sat_sub(*file_sz, 12)) {
		if (value > 255) {
			if (value > dico.size + 255)
				not_in_dico(last_value, &dico);
			first = entry_writer(new_fd, value, &dico);
			if (dico.size < DICO_SIZE)
				new_entry(last_value, first, &dico);
		} else {
			file_writer(new_fd, value, NO_FLUSH);
			new_entry(last_value, value, &dico);
		}
	}
	return (dico.size);
}

void		unlzw(int fd) {
	int64_t		file_sz;
	int			new_fd = 0;

	file_sz = get_fd_size(fd) * 8;
	if ((new_fd = open("uncompressed_file", O_WRONLY | O_TRUNC | O_CREAT, 0777)) == -1)
		quit_fd(fd, "Can not create ma bite file.");
	while (file_sz && unlzw_chunk(fd, new_fd, &file_sz) >= DICO_SIZE);
	file_writer(new_fd, 0, ONLY_FLUSH);
}
