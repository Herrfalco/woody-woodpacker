/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lzw.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 14:17:16 by fcadet            #+#    #+#             */
/*   Updated: 2022/06/26 13:09:16 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lzw.h"
#include "../includes.h"
#include "../utils/utils_asm.h"

static uint64_t		lzw_chunk(int dst, int src, int64_t *file_sz, uint8_t *nb_bits,
		rw_buff_t *r_buff, rw_buff_t *w_buff) {
	t_dico		dico = { 0 };
	uint16_t	byte = 0, last_byte = 0;
	int64_t		i;

	dico.bits = 9;
	if (*file_sz && file_reader(src, (uint8_t *)&last_byte, r_buff))
		quit_2_fd_asm(src, dst, "can't read from source file");
	--*file_sz;
	for (; *file_sz && !file_reader(src, (uint8_t *)&byte, r_buff)
			&& dico.size < DICO_SIZE; --*file_sz) {
		if ((i = check_dico(last_byte, byte, &dico)) >= 0)
			last_byte = DICO_START + i;
		else {
			value_writer(dst, last_byte, dico.bits, NO_FLUSH, w_buff);
			new_entry(last_byte, byte, &dico);
			if (get_bits_nb(dico.size) != dico.bits) {
				value_writer(dst, INCR_CODE, dico.bits, NO_FLUSH, w_buff);
				++dico.bits;
			}
			last_byte = byte;
		}
	}
	value_writer(dst, last_byte, dico.bits, NO_FLUSH, w_buff);
	if (dico.size == DICO_SIZE) {
		value_writer(dst, RESET_CODE, dico.bits, NO_FLUSH, w_buff);
		value_writer(dst, byte, 9, NO_FLUSH, w_buff);
	}
	*nb_bits = dico.bits;
	return (dico.size);
}

void		lzw(int dst, int src) {
	rw_buff_t	r_buff = { 0 }, w_buff = { 0 };
	int64_t		file_sz;
	uint8_t 	nb_bits = 0;
	
	if ((file_sz = get_fd_size_asm(src)) < 0)
		quit_2_fd_asm(dst, src, "can't get file size");
	while (file_sz && lzw_chunk(dst, src, &file_sz, &nb_bits, &r_buff, &w_buff)
			>= DICO_SIZE);
	value_writer(dst, STOP_CODE, nb_bits, FLUSH, &w_buff);
}
