/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dico.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 14:18:21 by fcadet            #+#    #+#             */
/*   Updated: 2022/06/27 14:14:07 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lzw.h"
#include "../includes.h"

int				get_bits_nb(uint64_t dico_size) {
	if (dico_size < 253)
		return (9);
	else if (dico_size < 765)
		return (10);
	else if (dico_size < 1789)
		return (11);
	else
		return (12);
}

void			new_entry(uint16_t last_value, uint16_t value, t_dico *dico) {
	dico->entry[dico->size][0] = last_value;
	dico->entry[dico->size][1] = value;
	++dico->size;
}

int64_t			check_dico(uint16_t last_value, uint16_t value, t_dico *dico) {
	uint64_t		i;

	for (i = 0; i < dico->size; ++i)
		if (last_value == dico->entry[i][0] && value == dico->entry[i][1])
			return (i);
	return (-1);
}

int64_t			entry_writer(int fd, uint16_t value, t_dico *dico, rw_buff_t *buff) {
	int64_t		ret;

	if (value >= DICO_START) {
		ret = entry_writer(fd, dico->entry[value - DICO_START][0], dico, buff);
		if (file_writer(fd, dico->entry[value - DICO_START][1], NO_FLUSH, buff))
			return (-1);
		return (ret);
	} else {
		if (file_writer(fd, value, NO_FLUSH, buff))
			return (-1);
		return (value);
	}
}

uint16_t		find_first_pattern(uint16_t value, t_dico *dico) {
	return (value >= DICO_START
			? find_first_pattern(dico->entry[value - DICO_START][0], dico) : value);
}
