/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_rw.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 12:46:15 by herrfalco         #+#    #+#             */
/*   Updated: 2022/06/06 16:56:37 by herrfalco        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"
#include "data_rw.h"

int		file_writer(int fd, uint8_t byte, bool_t flush) {
	static uint8_t		buff[BUFF_SIZE];
	static ssize_t		buff_len = 0;
	
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

int		value_writer(int fd, uint16_t value, size_t size, bool_t last_value) {
	static uint32_t		buff = 0;
	static uint8_t		buff_len = 0;
	static uint16_t		mask = 0;

	if (!mask)
		mask = ((uint16_t)~mask) >> (16 - size);
	buff |= ((uint32_t)(value & mask)) << (32 - size - buff_len);
	buff_len += size;
	while (buff_len >= 8 || (last_value && buff_len > 0)) {
		if (file_writer(fd, buff >> 24, last_value && buff_len <= 8) < 0)
			return (-1);
		buff <<= 8;
		buff_len = buff_len < 8 ? 0 : buff_len - 8;
	}
	return (buff_len);
}

int		value_reader(int fd, uint16_t *value, size_t size) {
	static uint32_t		buff = 0;
	static size_t		buff_size = 0;
	static uint16_t		mask = 0;
	int					read_ret;
	uint8_t				byte;

	if (!mask)
		mask = ((uint16_t)~mask) >> (16 - size);
	while (buff_size <= 24 && (read_ret = file_reader(fd, &byte)) > 0) {
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
