/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 15:25:00 by fcadet            #+#    #+#             */
/*   Updated: 2022/06/23 16:54:27 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include "utils_asm.h"

// A Supprimer
int				close_ret(int f1, int f2, int f3, int ret) {
	if (f1 > -1)
		close(f1);
	if (f2 > -1)
		close(f2);
	if (f3 > -1)
		close(f3);
	return (ret);
}

int				rand_v_file(int *file, uint64_t size) {
	int			src = open("/dev/urandom", O_RDONLY);
	uint8_t		buff[BUFF_SIZE];
	ssize_t		read_ret, write_size;

	if (src < 0)
		return (-1);
	if ((*file = syscall(319, "v_file", 0)) < 0)
		return (close_ret(src, -1, -1, -1));
	for (; size && (read_ret = read(src, buff, BUFF_SIZE)) > 0;
			size = sat_sub_asm(size, read_ret)) {
		write_size = size > (size_t)read_ret ? (size_t)read_ret : size;
		if (write(*file, buff, write_size) != write_size)
			return (close_ret(src, *file, -1, -1));
	}
	if (read_ret < 0)
		return (close_ret(src, *file, -1, -1));
	close(src);
	return (seek_asm(*file, 0) ? -1 : 0);
}

static int		bin_n_cmp(uint8_t *b1, uint8_t *b2, uint64_t size) {
	for (; size && *b1 == *b2; --size, ++b1, ++b2);
	return (size);
}

int				diff_v_files(int f1, int f2) {
	uint8_t		buff_1[BUFF_SIZE];
	uint8_t		buff_2[BUFF_SIZE];
	ssize_t		rd_ret_1, rd_ret_2;
	uint64_t	size_1, size_2;

	size_1 = get_fd_size(f1);
	size_2 = get_fd_size(f2);
	if (size_1 != size_2)
		return (1);
	for (; size_1 && (rd_ret_1 = read(f1, buff_1, BUFF_SIZE)) > 0
				&& (rd_ret_2 = read(f2, buff_2, BUFF_SIZE)) > 0;
				size_1 = sat_sub_asm(size_1, rd_ret_1)) {
		rd_ret_1 = rd_ret_1 < BUFF_SIZE ? rd_ret_1 : BUFF_SIZE;
		rd_ret_2 = rd_ret_2 < BUFF_SIZE ? rd_ret_2 : BUFF_SIZE;
		if (rd_ret_1 != rd_ret_2 || bin_n_cmp(buff_1, buff_2, rd_ret_1))
			return (seek_asm(f1, f2) ? -1 : 1);
	}
	if (rd_ret_1 < 0 || rd_ret_2 < 0) {
		seek_asm(f1, f2);
		return (-1);
	}
	return (seek_asm(f1, f2) ? -1 : 0);
}