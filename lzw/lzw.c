#include "../includes.h"
#include "lzw.h"
#include "../data_rw/data_rw.h"

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

int			lzw(int fd) {
	int	new_fd = 0;
	
	if ((new_fd = syscall(319, "tmp.lzw", 0)) == -1)
		quit_fd(fd, "Can not create memfd.");
	while (lzw_chunk(fd, new_fd) == DICO_SIZE);
	value_writer(new_fd, 0, 12, ONLY_FLUSH);
	close(fd);
	return (new_fd);
}

