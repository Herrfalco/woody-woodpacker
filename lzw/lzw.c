#include "../includes.h"
#include "lzw.h"
#include "../data_rw/data_rw.h"

static size_t		max_bit(uint64_t value) {
	size_t		i;

	for (i = 0; value; ++i)
		value >>= 1;
	return (i);
}

static size_t		lzw_chunk(int fd, int new_fd, int64_t *file_sz) {
	t_dico		dico;
	uint16_t	byte, last_byte;
	ssize_t		i;

	last_byte = 0;
	init_dico(&dico);
	if (file_reader(fd, (uint8_t *)&last_byte) < 0)
		quit_2_fd(fd, new_fd, "can't read file");
	for (; *file_sz && file_reader(fd, (uint8_t *)&byte) > 0
			&& dico.size < DICO_SIZE;
			--*file_sz) {
		if ((i = check_dico(last_byte, byte, &dico)) > 0)
			last_byte = 258 + i;
		else {
			if (max_bit(last_byte) > (size_t)dico.bits) {
				printf("%d, %d\n", last_byte, dico.bits);
				quit("wrong bit number");
			}
			value_writer(new_fd, last_byte, dico.bits, NO_FLUSH);
			new_entry(last_byte, byte, &dico);
			if (get_bits_nb(dico.size) != dico.bits) {
				if (max_bit(INCR_CODE) > dico.bits)
					quit("wrong bit number");
				value_writer(new_fd, INCR_CODE, dico.bits, NO_FLUSH);
				dico.bits = get_bits_nb(dico.size);
			}
			last_byte = byte;
		}
	}
	if (max_bit(last_byte) > dico.bits)
		quit("wrong bit number");
	value_writer(new_fd, last_byte, dico.bits, NO_FLUSH);
	if (dico.size == DICO_SIZE) {
		if (max_bit(RESET_CODE) > dico.bits)
			quit("wrong bit number");
		value_writer(new_fd, RESET_CODE, dico.bits, NO_FLUSH);
		if (max_bit(byte) > 9)
			quit("wrong bit number");
		value_writer(new_fd, byte, 9, NO_FLUSH);
	}
	return (dico.size);
}

int			lzw(int fd) {
	int64_t	file_sz;
	int		new_fd = 0;
	size_t	dico_sz;
	
	if ((new_fd = syscall(319, "tmp.lzw", 0)) == -1)
		quit_fd(fd, "Can not create memfd.");
	if ((file_sz = get_fd_size(fd)) < 0)
		quit_fd(fd, "can't get file size");
	lseek(fd, 0, SEEK_SET);
	while (file_sz && (dico_sz = lzw_chunk(fd, new_fd, &file_sz) >= DICO_SIZE));
	/*
	if (max_bit(STOP_CODE) > dico_sz)
		quit("wrong bit number");
		*/
//	value_writer(new_fd, STOP_CODE, dico_sz, NO_FLUSH);
	value_writer(new_fd, STOP_CODE, get_bits_nb(dico_sz), NO_FLUSH);
	/*
	if (max_bit(0) > dico_sz)
		quit("wrong bit number");
		*/
//	value_writer(new_fd, 0, dico_sz, ONLY_FLUSH);
	value_writer(new_fd, 0, get_bits_nb(dico_sz), ONLY_FLUSH);
	close(fd);
	return (new_fd);
}

