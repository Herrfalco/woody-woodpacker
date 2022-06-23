/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <fcadet@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 11:54:11 by herrfalco         #+#    #+#             */
/*   Updated: 2022/06/23 15:45:14 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

int64_t			get_fd_size(int fd) {
	ssize_t	size;

	if ((size = lseek(fd, 0, SEEK_END)) < 0 || lseek(fd, 0, SEEK_SET) < 0)
		return (-1);
	return (size);
}
