/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_rw.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 12:46:15 by herrfalco         #+#    #+#             */
/*   Updated: 2022/06/25 17:44:32 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lzw.h"
#include "../includes.h"
#include "../utils/utils_asm.h"

int		file_writer(int fd, uint8_t byte, flush_t flush, rw_buff_t *buff) {
	int64_t		write_ret;

	if (flush != ONLY_FLUSH)
		buff->bytes[buff->b_size++] = byte;
	if (flush || buff->b_size == BUFF_SIZE) {
		if ((write_ret = write(fd, buff->bytes, buff->b_size)) < 0
				|| (uint64_t)write_ret != buff->b_size)
			return (-1);
		buff->b_size = 0;
	}
	return (buff->b_size);
}

int		file_reader(int fd, uint8_t *byte, rw_buff_t *buff) {
	if (buff->idx == buff->b_size) {
		buff->idx = 0;
		if ((buff->b_size = read(fd, buff->bytes, BUFF_SIZE)) < 1)
			return (buff->b_size);
	}
	*byte = buff->bytes[buff->idx];
	return (buff->b_size - buff->idx++);
}

int		value_writer(int fd, uint16_t value, size_t size, flush_t flush, rw_buff_t *buff) {
	uint16_t			mask = ((uint16_t)~0) >> (16 - size);

	buff->dword |= ((uint32_t)(value & mask)) << (32 - size - buff->dw_size);
	buff->dw_size += size;
	while (buff->dw_size >= 8 || (flush && buff->dw_size > 0)) {
		if (file_writer(fd, buff->dword >> 24, buff->dw_size <= 8 ? flush : NO_FLUSH, buff) < 0)
			return (-1);
		buff->dword <<= 8;
		buff->dw_size = sat_sub_asm(buff->dw_size, 8);
	}
	return (buff->dw_size);
}

int		value_reader(int fd, uint16_t *value, size_t size, rw_buff_t *buff) {
	int					read_ret;
	uint8_t				byte;
	uint16_t			mask = ((uint16_t)~0) >> (16 - size);

	while (buff->dw_size <= 24 && (read_ret = file_reader(fd, &byte, buff)) > 0) {
		buff->dword <<= 8;
		buff->dword |= byte;
		buff->dw_size += 8;
	}
	if (read_ret < 0)
		return (read_ret);
	if (buff->dw_size >= size) {
		buff->dw_size -= size;
		*value = (buff->dword >> buff->dw_size) & mask;
		return (1);
	} else
		return (0);
}
