/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unlzw.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 14:18:39 by fcadet            #+#    #+#             */
/*   Updated: 2022/06/25 17:52:26 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lzw.h"
#include "../includes.h"
#include "../utils/utils_asm.h"
#include "asm/unlzw_asm.h"

static uint64_t	unlzw_chunk(int dst, int src, int *reset, rw_buff_t *r_buff, rw_buff_t *w_buff) {
	t_dico		dico = {
		.size = 0,
		.bits = 9,
	};
	uint16_t	value, last_value, first;

	if (value_reader(src, &last_value, 9, r_buff) < 0)
		quit_2_fd_asm(src, dst, "can't read file");
	if (last_value == STOP_CODE)
		return (0);
	file_writer(dst, last_value, NO_FLUSH, w_buff);
	if (*reset) {
		*reset = 0;
		return (DICO_SIZE);
	}
	for (; value_reader(src, &value, dico.bits, r_buff) > 0;
			last_value = value) {
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
			if (value >= dico.size + DICO_START) {
				not_in_dico(last_value, &dico);
				entry_writer(dst, value, &dico, w_buff);
			}
			else {
				first = entry_writer(dst, value, &dico, w_buff);
				new_entry(last_value, first, &dico);
			}
		}
		else {
			file_writer(dst, value, NO_FLUSH, w_buff);
			new_entry(last_value, value, &dico);
		}
	}
	return (dico.size);
}

void		unlzw(int dst, int src) {
	rw_buff_t	r_buff = { 0 }, w_buff = { 0 };
	int			reset = 0;

	if (get_fd_size_asm(src) > 0)
		while (unlzw_chunk(dst, src, &reset, &r_buff, &w_buff) >= DICO_SIZE);
	file_writer(dst, 0, ONLY_FLUSH, &w_buff);
}
