/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dico.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 14:18:21 by fcadet            #+#    #+#             */
/*   Updated: 2022/06/24 14:18:36 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lzw.h"
#include "../includes.h"
#include "../data_rw/data_rw.h"

void			init_dico(t_dico *dico) {
	dico->size = 0;
	dico->bits = 9;
}

int				get_bits_nb(uint64_t dico_size) {
	if (dico_size < 258)
		return (9);
	else if (dico_size < 765)
		return (10);
	else if (dico_size < 1789)
		return (11);
	else
		return (12);
}

void			new_entry(uint16_t last_byte, uint16_t byte, t_dico *dico) {
	dico->entry[dico->size][0] = last_byte;
	dico->entry[dico->size][1] = byte;
	++dico->size;
}

int				check_dico(uint16_t last_value, uint16_t value, t_dico *dico) {
	uint64_t	i;

	for (i = 0; i < dico->size; ++i)
		if (last_value == dico->entry[i][0] && value == dico->entry[i][1])
			return (i + 1);
	return (0);
}

uint16_t		entry_writer(int fd, uint16_t value, t_dico *dico) {
	uint16_t	ret;

	if (dico->entry[value - DICO_START][0] >= DICO_START)
		ret = entry_writer(fd, dico->entry[value - DICO_START][0], dico);
	else {
		file_writer(fd, dico->entry[value - DICO_START][0], NO_FLUSH);
		ret = dico->entry[value - DICO_START][0];
	}
	file_writer(fd, dico->entry[value - DICO_START][1], NO_FLUSH);
	return (ret);
}

static uint16_t		find_first_pattern(uint16_t value, t_dico *dico) {
	if (dico->entry[value - DICO_START][0] >= DICO_START)
		return (find_first_pattern(dico->entry[value - DICO_START][0], dico));
	else 
		return (dico->entry[value - DICO_START][0]);
}

void			not_in_dico(uint16_t last_value, t_dico *dico) {
	if (last_value <= MAX_BYTE)
		new_entry(last_value, last_value, dico);
	else
		new_entry(last_value, find_first_pattern(last_value, dico), dico);
}
