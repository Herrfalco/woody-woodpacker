/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <fcadet@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 11:54:11 by herrfalco         #+#    #+#             */
/*   Updated: 2022/06/09 12:22:25 by herrfalco        ###   ########.fr       */
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
