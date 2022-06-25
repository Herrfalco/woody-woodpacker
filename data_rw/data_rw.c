/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_rw.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 12:46:15 by herrfalco         #+#    #+#             */
/*   Updated: 2022/06/25 15:06:10 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"
#include "data_rw.h"

int		file_writer(int fd, uint8_t byte, flush_t flush) {
	static uint8_t		buff[BUFF_SIZE];
	static ssize_t		buff_len = 0;
	
	if (flush != ONLY_FLUSH)
		buff[buff_len++] = byte;
	if (flush || buff_len == BUFF_SIZE) {
		if (write(fd, buff, buff_len) != buff_len)
			return (-1);
		buff_len = 0;
	}
	return (buff_len);
}

int		file_reader(int fd, uint8_t *byte, b_buff_t *buff) {
	if (buff->idx == buff->size) {
		buff->idx = 0;
		if ((buff->size = read(fd, buff, BUFF_SIZE)) < 1)
			return (buff->size);
	}
	*byte = buff->data[buff->idx];
	return (buff->size - buff->idx++);
}

int		value_writer(int fd, uint16_t value, size_t size, flush_t flush) {
	static uint32_t		buff = 0;
	static uint8_t		buff_len = 0;
	uint16_t			mask = ((uint16_t)~0) >> (16 - size);

	buff |= ((uint32_t)(value & mask)) << (32 - size - buff_len);
	buff_len += size;
	while (buff_len >= 8 || (flush && buff_len > 0)) {
		if (file_writer(fd, buff >> 24, buff_len <= 8 ? flush : NO_FLUSH) < 0)
			return (-1);
		buff <<= 8;
		buff_len = buff_len < 8 ? 0 : buff_len - 8;
	}
	return (buff_len);
}

int		value_reader(int fd, uint16_t *value, size_t size, dw_buff_t *vr_buff, b_buff_t *fr_buff) {
	int					read_ret;
	uint8_t				byte;
	uint16_t			mask = ((uint16_t)~0) >> (16 - size);

	while (vr_buff->size <= 24 && (read_ret = file_reader(fd, &byte, fr_buff)) > 0) {
		vr_buff->data <<= 8;
		vr_buff->data |= byte;
		vr_buff->size += 8;
	}
	if (read_ret < 0)
		return (read_ret);
	if (vr_buff->size >= size) {
		vr_buff->size -= size;
		*value = (vr_buff->data >> vr_buff->size) & mask;
		return (1);
	} else
		return (0);
}
