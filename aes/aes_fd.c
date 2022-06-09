/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aes_fd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 23:37:21 by herrfalco         #+#    #+#             */
/*   Updated: 2022/06/09 13:07:44 by herrfalco        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"
#include "aes.h"

static void			encode_fd(int fd_dst, int fd_src, uint32_t *r_keys) {
	uint8_t		buff[BUFF_SIZE];
	ssize_t		read_ret, write_ret;
	uint64_t	src_sz, data_sz;
	size_t		i;

	for (src_sz = 0, data_sz = 0; (read_ret = read(fd_src, buff, BUFF_SIZE)) > 0;) {
		src_sz += read_ret;
		if (read_ret < BUFF_SIZE)
			for (i = read_ret; i < BUFF_SIZE; ++i)
				buff[i] = 0;
		data_sz = read_ret % 16 ? (read_ret / 16) * 16 + 16 : read_ret;
		aes_data(buff, data_sz, r_keys, ENCODE);
		if ((write_ret = write(fd_dst, buff, data_sz)) < 0 || (size_t)write_ret < data_sz)
			quit_2_fd(fd_src, fd_dst, "can't write to destination file");
	}
	if (read_ret < 0)
		quit_2_fd(fd_src, fd_dst, "can't read from source file");
	if ((write_ret = write(fd_dst, &src_sz, 8)) < 8)
		quit_2_fd(fd_src, fd_dst, "can't write to destination file");
}

static void			decode_fd(int fd_dst, int fd_src, uint32_t *r_keys) {
	uint8_t		buff[BUFF_SIZE];
	ssize_t		read_ret, write_ret;
	uint64_t	src_sz, data_sz, write_sz;

	if ((data_sz = lseek(fd_src, -8, SEEK_END)) < 0)
		quit_2_fd(fd_src, fd_dst, "can't seek inside source file");
	if ((read_ret = read(fd_src, &src_sz, 8)) < 8)
		quit_2_fd(fd_src, fd_dst, "can't read from source file");
	if ((data_sz = lseek(fd_src, 0, SEEK_SET)) < 0)
		quit_2_fd(fd_src, fd_dst, "can't seek inside source file");
	for (; (read_ret = read(fd_src, buff, BUFF_SIZE)) > 0;
		data_sz -= read_ret, src_sz -= read_ret) {
		aes_data(buff, data_sz > (size_t)read_ret ? read_ret : data_sz, r_keys, DECODE);
		write_sz = src_sz > (size_t)read_ret ? read_ret : src_sz;
		if ((write_ret = write(fd_dst, buff, write_sz)) < 0 || (size_t)write_ret < write_sz)
			quit_2_fd(fd_src, fd_dst, "can't write to destination file");
	}
	if (read_ret < 0)
		quit_2_fd(fd_src, fd_dst, "can't read from source file");
}

void				aes_fd(int fd_dst, int fd_src, method_t type) {
	uint8_t			key[KEY_SIZE] = { 0 };
	uint32_t		r_keys[RKEY_ARR_SIZE] = { 0 };

	if (rand_key(key, KEY_SIZE) < 0)
		quit_2_fd(fd_dst, fd_src, "can't create random key");
	round_keys(key, r_keys);
	if (type == ENCODE)
		encode_fd(fd_dst, fd_src, r_keys);
	else
		decode_fd(fd_dst, fd_src, r_keys);
}
