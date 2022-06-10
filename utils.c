/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <fcadet@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 11:54:11 by herrfalco         #+#    #+#             */
/*   Updated: 2022/06/10 15:35:20 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.h"

void		quit_2_fd(int fd1, int fd2, char *error) {
	if (fd1 >= 0)
		close(fd1);
	if (fd2 >= 0)
		close(fd2);
	if (error)
		fprintf(stderr, "Error: %s\n", error);
	exit(ERR_STAT);
}

void		quit_fd(int fd, char *error) {
	quit_2_fd(fd, -1, error);
}

void		quit(char *error) {
	quit_fd(-1, error);
}

uint64_t		sat_sub(uint64_t lhs, uint64_t rhs) {
	return (lhs < rhs ? 0 : lhs - rhs);
}

int		seek_ret(int f1, int f2, int ret) {
	int		seek_v = 0;

	if (f1 > -1)
		seek_v |= lseek(f1, 0, SEEK_SET);
	if (f2 > -1)
		seek_v |= lseek(f2, 0, SEEK_SET);
	return (seek_v ? -1 : ret);
}

int64_t			get_fd_size(int fd) {
	ssize_t	size;

	if ((size = lseek(fd, 0, SEEK_END)) < 0 || lseek(fd, 0, SEEK_SET) < 0)
		return (-1);
	return (size);
}
