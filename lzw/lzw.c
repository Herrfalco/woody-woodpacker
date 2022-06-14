#include "../includes.h"
#include "lzw.h"
#include "../data_rw/data_rw.h"

static size_t		lzw_chunk(int fd, int new_fd, int64_t *file_sz) {
	t_dico		dico = { 0 };
	uint16_t	byte, last_byte;
	ssize_t		i;

	last_byte = 0;
	if (file_reader(fd, (uint8_t *)&last_byte) < 0)
		quit_2_fd(fd, new_fd, "can't read file");
	for (; *file_sz && file_reader(fd, (uint8_t *)&byte) > 0
			&& dico.size < DICO_SIZE;
			--*file_sz) {
		if ((i = check_dico(last_byte, byte, &dico)) > 0)
			last_byte = 257 + i;
		else {
			value_writer(new_fd, last_byte, 12, NO_FLUSH);
			new_entry(last_byte, byte, &dico);
			last_byte = byte;
		}
	}
	value_writer(new_fd, last_byte, 12, NO_FLUSH);
	if (dico.size == DICO_SIZE)
		value_writer(new_fd, 256, 12, NO_FLUSH);
	return (dico.size);
}

int			lzw(int fd) {
	int64_t	file_sz;
	int	new_fd = 0;
	
	if ((new_fd = syscall(319, "tmp.lzw", 0)) == -1)
		quit_fd(fd, "Can not create memfd.");
	if ((file_sz = get_fd_size(fd)) < 0)
		quit_fd(fd, "can't get file size");
	lseek(fd, 0, SEEK_SET);
	while (file_sz && lzw_chunk(fd, new_fd, &file_sz) >= DICO_SIZE);
	value_writer(new_fd, 257, 12, NO_FLUSH);
	value_writer(new_fd, 0, 12, ONLY_FLUSH);
	close(fd);
	return (new_fd);
}

