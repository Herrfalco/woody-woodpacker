/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_rw.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 12:46:15 by herrfalco         #+#    #+#             */
/*   Updated: 2022/06/28 16:19:15 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lzw.h"
#include "../includes.h"
#include "../utils/utils_asm.h"
#include "asm/asm.h"

/*
int64_t		file_reader(int64_t fd, uint8_t *byte, rw_buff_t *buff) {
	int64_t		read_ret;

	if (buff->idx == buff->b_size) {
		buff->idx = 0;
		if ((read_ret = read(fd, buff->bytes, BUFF_SIZE)) < 1)
			return (-1);
		buff->b_size = (uint64_t)read_ret;
	}
	*byte = buff->bytes[buff->idx];
	++buff->idx;
	return (0);
}
*/

int64_t		value_writer(int64_t fd, uint16_t value, uint64_t size, flush_t flush, rw_buff_t *buff) {
	uint16_t			mask = ((uint16_t)~0) >> (16 - size);

	buff->dword |= ((uint32_t)(value & mask)) << (32 - size - buff->dw_size);
	buff->dw_size += size;
	while (buff->dw_size >= 8 || (flush && buff->dw_size > 0)) {
		if (file_writer_asm(fd, buff->dword >> 24, buff->dw_size <= 8 ? flush : NO_FLUSH, buff))
			return (-1);
		buff->dword <<= 8;
		buff->dw_size = sat_sub_asm(buff->dw_size, 8);
	}
	return (0);
}
