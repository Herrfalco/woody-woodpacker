/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unlzw.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 14:18:39 by fcadet            #+#    #+#             */
/*   Updated: 2022/06/27 23:33:35 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lzw.h"
#include "../includes.h"
#include "../utils/utils_asm.h"
#include "asm/asm.h"

/*
uint64_t	unlzw_chunk(int64_t dst, int64_t src, rw_buff_t *r_buff,
		rw_buff_t *w_buff) {
	t_dico		dico = { 0 };
	uint16_t	value = 0, last_value = 0, init = 0;
	int64_t		first;

	dico.bits = 9;
	for (; !value_reader(src, &value, dico.bits, r_buff);
			last_value = value, value = 0) {
		if (!init) {
			if (entry_writer_asm(dst, value, &dico, w_buff) < 0)
				quit_2_fd_asm(src, dst, "can't write to destination file");
			init = 1;
		} else {
			if (value == INCR_CODE) {
				++dico.bits;
				value = last_value;
			} else if (value == RESET_CODE || value == STOP_CODE) {
				if (file_writer(dst, 0, ONLY_FLUSH, w_buff))
					quit_2_fd_asm(src, dst, "can't write to destination file");
				return (value);
			} else if (value == dico.size + DICO_START) {
				new_entry_asm(last_value, find_first_pattern_asm(last_value, &dico), &dico);
				if (entry_writer_asm(dst, value, &dico, w_buff) < 0)
					quit_2_fd_asm(src, dst, "can't write to destination file");
			} else if (value > dico.size + DICO_START) {
				quit_2_fd_asm(src, dst, "corrupted file");
			} else {
				if ((first = entry_writer_asm(dst, value, &dico, w_buff)) < 0)
					quit_2_fd_asm(src, dst, "can't write to destination file");
				new_entry_asm(last_value, first, &dico);
			}
		}
	}
	quit_2_fd_asm(src, dst, "can't read from source file");
	return (0);
}
*/

/*
void		unlzw(int64_t dst, int64_t src) {
	rw_buff_t	r_buff = { 0 }, w_buff = { 0 };
	int64_t		file_sz;

	if ((file_sz = get_fd_size_asm(src)) < 0)
		quit_2_fd_asm(dst, src, "can't get file size");
	if (file_sz < 1)
		return;
	while (unlzw_chunk(dst, src, &r_buff, &w_buff) != STOP_CODE);
}
*/
