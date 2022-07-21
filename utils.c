/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/20 16:35:16 by fcadet            #+#    #+#             */
/*   Updated: 2022/07/21 12:25:51 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.h"

void	error_fd_unmap(int fd, void *mem, size_t size, char *msg) {
	munmap(mem, size);
	if (fd >= 0)
		close(fd);
	if (mem)
		munmap(mem, size);
	if (msg)
		fprintf(stderr, "Error: %s\n", msg);
	exit(42);
}

void	error_unmap(void *mem, size_t size, char *msg) {
	error_fd_unmap(-1, mem, size, msg);
}

void	error_fd(int fd, char *msg) {
	error_fd_unmap(fd, NULL, 0, msg);
}

void	error(char *msg) {
	error_fd_unmap(-1, NULL, 0, msg);
}

uint64_t		round_up(uint64_t val, uint64_t mod) {
	return (val % mod ? (val / mod) * mod + mod : val);
}

int		str_n_cmp(uint8_t *s1, uint8_t *s2, int n) {
	for (; *s1 && *s1 == *s2; ++s1, ++s2)
		if (!--n)
			break;
	return (*s1 - *s2);
}

int64_t			get_fd_size(int fd) {
	int64_t		size;

	if ((size = lseek(fd, 0, SEEK_END)) < 0 || lseek(fd, 0, SEEK_SET) != 0)
		return (-1);
	return (size);
}

int		write_pad(int fd, uint64_t size) {
	static uint8_t		buff[BUFF_SZ] = { 0 };
	uint64_t			write_sz = 0;
	int64_t				w_ret;

	for (; size; size -= write_sz) {
		write_sz = size < BUFF_SZ ? size : BUFF_SZ;
		if ((w_ret = write(fd, buff, write_sz)) < 0
				|| (uint64_t)w_ret != write_sz)
			return (-1);
	}
	return (0);
}
