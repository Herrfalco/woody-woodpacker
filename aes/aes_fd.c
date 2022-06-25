/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aes_fd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 23:37:21 by herrfalco         #+#    #+#             */
/*   Updated: 2022/06/25 16:50:01 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "aes.h"
#include "aes_asm.h"
#include "../utils/utils_asm.h"

void			aes_fd_enc(int fd_dst, int fd_src, uint8_t *key) {
	uint8_t		buff[BUFF_SIZE] = { 0 };
	uint32_t	r_keys[RKEY_MAX_NB] = { 0 };
	ssize_t		read_ret = 0, write_ret = 0;
	int64_t		src_sz = 0;
	uint64_t	data_sz;
	size_t		i;

	round_keys_asm(key, r_keys);
	if ((src_sz = get_fd_size_asm(fd_src)) < 0)
		quit_2_fd_asm(fd_src, fd_dst, "can't seek into source file");
	if ((write_ret = write(fd_dst, &src_sz, 8)) < 8)
		quit_2_fd_asm(fd_src, fd_dst, "can't write to destination file");
	for (; src_sz && (read_ret = read(fd_src, buff, BUFF_SIZE)) > 0;
			src_sz = sat_sub_asm(src_sz, BUFF_SIZE)) {
		for (i = src_sz; i < BUFF_SIZE; ++i)
			buff[i] = 0;
		data_sz = src_sz < BUFF_SIZE ? round_up_asm(src_sz, 16) : BUFF_SIZE;
		aes_data_enc(buff, data_sz, r_keys);
		if ((write_ret = write(fd_dst, buff, data_sz)) < 0 || (size_t)write_ret != data_sz)
			quit_2_fd_asm(fd_src, fd_dst, "can't write to destination file");
	}
	if (read_ret < 0)
		quit_2_fd_asm(fd_src, fd_dst, "can't read from source file");
}
