/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aes_fd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 23:37:21 by herrfalco         #+#    #+#             */
/*   Updated: 2022/06/20 18:00:24 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"
#include "aes_utils.h"
#include "asm/aes_asm.h"
#include "aes.h"

static void			encode_fd(int fd_dst, int fd_src, uint32_t *r_keys) {
	uint8_t		buff[BUFF_SIZE] = { 0 };
	ssize_t		read_ret, write_ret;
	int64_t		src_sz;
	uint64_t	data_sz;
	size_t		i;

	if ((src_sz = get_fd_size(fd_src)) < 0)
		quit_2_fd(fd_src, fd_dst, "can't seek into source file");
	if ((write_ret = write(fd_dst, &src_sz, 8)) < 8)
		quit_2_fd(fd_src, fd_dst, "can't write to destination file");
	for (; src_sz && (read_ret = read(fd_src, buff, BUFF_SIZE)) > 0;
			src_sz = sat_sub(src_sz, BUFF_SIZE)) {
		for (i = src_sz; i < BUFF_SIZE; ++i)
			buff[i] = 0;
		data_sz = src_sz < BUFF_SIZE ? round_up(src_sz, 16) : BUFF_SIZE;
		aes_data_enc(buff, data_sz, r_keys);
		if ((write_ret = write(fd_dst, buff, data_sz)) < 0 || (size_t)write_ret < data_sz)
			quit_2_fd(fd_src, fd_dst, "can't write to destination file");
	}
	if (read_ret < 0)
		quit_2_fd(fd_src, fd_dst, "can't read from source file");
}

static void			decode_fd(int fd_dst, int fd_src, uint32_t *r_keys) {
	uint8_t		buff[BUFF_SIZE];
	ssize_t		read_ret, write_ret;
	uint64_t	src_sz, write_sz;

	if ((read_ret = read(fd_src, &src_sz, 8)) < 8)
		quit_2_fd(fd_src, fd_dst, "can't read from source file");
	for (; src_sz && (read_ret = read(fd_src, buff, BUFF_SIZE)) > 0;
			src_sz = sat_sub(src_sz, BUFF_SIZE)) {
		write_sz = src_sz > BUFF_SIZE ? BUFF_SIZE : src_sz;
		aes_data_dec_asm(buff, round_up(write_sz, 16), r_keys, get_rnb_asm(KEY_SIZE));
		if ((write_ret = write(fd_dst, buff, write_sz)) < 0 || (size_t)write_ret < write_sz)
			quit_2_fd(fd_src, fd_dst, "can't write to destination file");
	}
	if (read_ret < 0)
		quit_2_fd(fd_src, fd_dst, "can't read from source file");
}

void				aes_fd(int fd_dst, int fd_src, uint8_t *key, method_t type) {
	uint32_t		r_keys[RKEY_ARR_SIZE] = { 0 };

	round_keys(key, r_keys);
	if (type == ENCODE)
		encode_fd(fd_dst, fd_src, r_keys);
	else
		decode_fd(fd_dst, fd_src, r_keys);
	if (seek_ret(fd_dst, fd_src, 0))
		quit_2_fd(fd_src, fd_dst, "can't seek into files");
}
