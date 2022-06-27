/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lzw.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 14:17:16 by fcadet            #+#    #+#             */
/*   Updated: 2022/06/27 02:42:31 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lzw.h"
#include "../includes.h"
#include "../utils/utils_asm.h"
#include "asm/asm.h"

static void		lzw_chunk(int64_t dst, int64_t src, int64_t *file_size,
		rw_buff_t *r_buff, rw_buff_t *w_buff) {
	t_dico		dico = { 0 };
	uint16_t	value = 0, last_value = 0, init = 0;
	int64_t		idx;

	dico.bits = 9;
	for (; *file_size && dico.size < DICO_SIZE
			&& !(file_reader(src, (uint8_t *)&value, r_buff));
			--*file_size, last_value = value, value = 0) {
		if (!init)
			init = 1;
		else if ((idx = check_dico(last_value, value, &dico)) >= 0)
			value = DICO_START + idx;
		else {
			if (value_writer(dst, last_value, dico.bits, NO_FLUSH, w_buff))
				quit_2_fd_asm(src, dst, "can't write to destination file");
			new_entry_asm(last_value, value, &dico);
			if (get_bits_nb(dico.size) != dico.bits) {
				if (value_writer(dst, INCR_CODE, dico.bits, NO_FLUSH, w_buff))
					quit_2_fd_asm(src, dst, "can't write to destination file");
				++dico.bits;
			}
		}
	}
	if (value_writer(dst, last_value, dico.bits, NO_FLUSH, w_buff))
		quit_2_fd_asm(src, dst, "can't write to destination file");
	if (dico.size == DICO_SIZE) {
		if (value_writer(dst, RESET_CODE, dico.bits, NO_FLUSH, w_buff))
			quit_2_fd_asm(src, dst, "can't write to destination file");
	}
	else if (!*file_size) {
		if (value_writer(dst, STOP_CODE, dico.bits, FLUSH, w_buff))
			quit_2_fd_asm(src, dst, "can't write to destination file");
	}
	else
		quit_2_fd_asm(src, dst, "can't read from source file");
}

void		lzw(int64_t dst, int64_t src) {
	rw_buff_t	r_buff = { 0 }, w_buff = { 0 };
	int64_t		file_sz;
	
	if ((file_sz = get_fd_size_asm(src)) < 0)
		quit_2_fd_asm(dst, src, "can't get file size");
	while (file_sz)
		lzw_chunk(dst, src, &file_sz, &r_buff, &w_buff);
}
