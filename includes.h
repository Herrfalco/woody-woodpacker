/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   includes.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 22:07:07 by herrfalco         #+#    #+#             */
/*   Updated: 2022/06/09 12:22:48 by herrfalco        ###   ########.fr       */
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

typedef enum	bool_e {
	FALSE,
	TRUE,
}				bool_t;

void		quit_2_fd(int fd1, int fd2, char *error);
void		quit_fd(int fd, char *error);
void		quit(char *error);

#endif // INCLUDES_H
