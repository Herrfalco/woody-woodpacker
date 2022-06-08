/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_rw.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 12:46:15 by herrfalco         #+#    #+#             */
/*   Updated: 2022/06/07 15:15:49 by herrfalco        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"
#include "data_rw.h"

int		data_writer(uint8_t **data, size_t *pos, uint8_t byte, flush_t flush) {
	static uint8_t		buff[BUFF_SIZE];
	static ssize_t		buff_len = 0;
	ssize_t				i = 0

	if (flush != ONLY_FLUSH)
		buff[buff_len++] = byte;
	if (flush || buff_len == BUFF_SIZE) {
		for (; i < buff_len; ++i)
			*data[*pos += i] = buff[i];
		buff_len = 0;
	}
	return (buff_len);
}

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

int		file_reader(int fd, uint8_t *byte) {
	static uint8_t		buff[BUFF_SIZE];
	static ssize_t		buff_size = 0;
	static ssize_t		buff_idx = 0;

	if (buff_idx == buff_size) {
		buff_idx = 0;
		if ((buff_size = read(fd, buff, BUFF_SIZE)) < 1)
			return (buff_size);
	}
	*byte = buff[buff_idx];
	return (buff_size - (buff_idx++));
}

int		data_reader(uint8_t **data, size_t *pos, uint8_t *byte) {
	static uint8_t		buff[BUFF_SIZE];
	static ssize_t		buff_size = 0;
	static ssize_t		buff_idx = 0;
	ssize_t				i = 0;

	if (buff_idx == buff_size) {
		buff_idx = 0;
		for (; i < BUFF_SIZE; ++i) {
			buff[i] = *data[pos += i];
			buff_size += i;
			if (buff[i] == '\0')
				return (buff_size);
		}
	}
	*byte = buff[buff_idx];
	return (buff_size - (buff_idx++));
}

int		value_writer(uint8_t **data, size_t *pos, uint16_t value, size_t size, flush_t flush) {
	static uint32_t		buff = 0;
	static uint8_t		buff_len = 0;
	static uint16_t		mask = 0;

	if (!mask)
		mask = ((uint16_t)~mask) >> (16 - size);
	buff |= ((uint32_t)(value & mask)) << (32 - size - buff_len);
	buff_len += size;
	while (buff_len >= 8 || (flush && buff_len > 0)) {
		if (data_writer(data, pos, buff >> 24, buff_len <= 8 ? flush : NO_FLUSH) < 0)
			return (-1);
		buff <<= 8;
		buff_len = buff_len < 8 ? 0 : buff_len - 8;
	}
	return (buff_len);
}

int		value_reader(uint8_t **data, size_t *pos, uint16_t *value, size_t size) {
	static uint32_t		buff = 0;
	static size_t		buff_size = 0;
	static uint16_t		mask = 0;
	int					read_ret;
	uint8_t				byte;

	if (!mask)
		mask = ((uint16_t)~mask) >> (16 - size);
	while (buff_size <= 24 && (read_ret = data_reader(data, pos, &byte)) > 0) {
		buff <<= 8;
		buff |= byte;
		buff_size += 8;
	}
	if (read_ret < 0)
		return (read_ret);
	if (buff_size >= size) {
		buff_size -= size;
		*value = (buff >> buff_size) & mask;
		return (1);
	} else {
		buff = 0;
		buff_size = 0;
		return (0);
	}
}
