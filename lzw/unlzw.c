#include "../includes.h"
#include "lzw.h"
#include "../data_rw/data_rw.h"
#include "asm/unlzw_asm.h"

static uint64_t	unlzw_chunk(int fd, int new_fd, int *reset) {
	t_dico		dico;
	uint16_t	value, last_value, first;

	init_dico(&dico);
	if (value_reader(fd, &last_value, 9) < 0)
		quit_2_fd(fd, new_fd, "can't read file");
	file_writer(new_fd, last_value, NO_FLUSH);
	if (*reset) {
		*reset = 0;
		return (DICO_SIZE);
	}
	for (; value_reader(fd, &value, dico.bits) > 0; last_value = value) {
		if (value == RESET_CODE) {
			*reset = 1;
			return (DICO_SIZE);
		}
		else if (value == INCR_CODE) {
			value = last_value;
			++dico.bits;
		}
		else if (value == STOP_CODE)
			return (0);
		else if (value >= DICO_START) {
			if (value >= dico.size + DICO_START)
				not_in_dico(last_value, &dico);
			first = entry_writer(new_fd, value, &dico);
			new_entry(last_value, first, &dico);
		}
		else {
			file_writer(new_fd, value, NO_FLUSH);
			new_entry(last_value, value, &dico);
		}
	}
	return (dico.size);
}

void		unlzw(int fd) {
	int			new_fd = 0;
	int			reset = 0;

	if ((new_fd = open("uncompressed_file", O_WRONLY | O_TRUNC | O_CREAT, 0777)) == -1)
		quit_fd(fd, "Can not create uncompressed file.");
	lseek(fd, 0, SEEK_SET);
	if (get_fd_size(fd) > 0)
		while (unlzw_chunk(fd, new_fd, &reset) >= DICO_SIZE);
	file_writer(new_fd, 0, ONLY_FLUSH);
	close(fd);
	close(new_fd);
}
