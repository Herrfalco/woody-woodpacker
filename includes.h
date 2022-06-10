/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   includes.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 22:07:07 by herrfalco         #+#    #+#             */
/*   Updated: 2022/06/10 15:35:44 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDES_H
#define INCLUDES_H

#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERR_STAT		42
#define BUFF_SIZE		1024

typedef enum	bool_e {
	FALSE,
	TRUE,
}				bool_t;

void		quit_2_fd(int fd1, int fd2, char *error);
void		quit_fd(int fd, char *error);
void		quit(char *error);
uint64_t	sat_sub(uint64_t lhs, uint64_t rhs);
int			seek_ret(int f1, int f2, int ret);
int64_t		get_fd_size(int fd);


#endif // INCLUDES_H
