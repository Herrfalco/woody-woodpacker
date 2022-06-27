/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unlzw.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 14:18:39 by fcadet            #+#    #+#             */
/*   Updated: 2022/06/27 02:46:04 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lzw.h"
#include "../includes.h"
#include "../utils/utils_asm.h"
#include "asm/unlzw_asm.h"

static uint64_t	unlzw_chunk(int64_t dst, int64_t src, rw_buff_t *r_buff,
		rw_buff_t *w_buff) {
	t_dico		dico = { 0 };
	uint16_t	value = 0, last_value = 0, init = 0, first;

	dico.bits = 9;
	for (; !value_reader(src, &value, dico.bits, r_buff);
			last_value = value, value = 0) {
		if (!init) {
			entry_writer(dst, value, &dico, w_buff);
			init = 1;
		} else {
			if (value == INCR_CODE) {
				++dico.bits;
				value = last_value;
			} else if (value == RESET_CODE || value == STOP_CODE) {
				file_writer(dst, 0, ONLY_FLUSH, w_buff);
				return (value);
			} else if (value == dico.size + DICO_START) {
				new_entry(last_value, find_first_pattern(last_value, &dico), &dico);
				entry_writer(dst, value, &dico, w_buff);
			} else {
				first = entry_writer(dst, value, &dico, w_buff);
				new_entry(last_value, first, &dico);
			}
		}
	}
	quit_2_fd_asm(src, dst, "can't read from source file");
	return (0);
}

void		unlzw(int64_t dst, int64_t src) {
	rw_buff_t	r_buff = { 0 }, w_buff = { 0 };
	int64_t		file_sz;

	if ((file_sz = get_fd_size_asm(src)) < 0)
		quit_2_fd_asm(dst, src, "can't get file size");
	if (file_sz < 1)
		return;
	while (unlzw_chunk(dst, src, &r_buff, &w_buff) != STOP_CODE);
}
