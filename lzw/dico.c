#include "../includes.h"
#include "lzw.h"
#include "../data_rw/data_rw.h"

void			new_entry(uint16_t last_byte, uint16_t byte, t_dico *dico) {
	size_t	i;

	for (i = 0; i < dico->size; ++i)
		if (dico->entry[i][0] == last_byte && dico->entry[i][1] == byte)
			return ;
	dico->entry[dico->size][0] = last_byte;
	dico->entry[dico->size][1] = byte;
	++dico->size;
}

int				check_dico(uint16_t last_value, uint16_t value, t_dico *dico) {
	size_t	i;

	for (i = 0; i < dico->size; ++i)
		if (last_value == dico->entry[i][0] && value == dico->entry[i][1])
			return (i + 1);
	return (0);
}

uint16_t		entry_writer(int fd, uint16_t value, t_dico *dico) {
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

void			not_in_dico(uint16_t last_value, t_dico *dico) {
	if (last_value <= 255)
		new_entry(last_value, last_value, dico);
	else
		new_entry(last_value, find_first_pattern(last_value, dico), dico);
}


