/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_asm.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 22:07:07 by herrfalco         #+#    #+#             */
/*   Updated: 2022/06/23 17:00:37 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_ASM_H
#define UTILS_ASM_H

#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_SIZE		1024	// (must update buff_sz in aes_fd_dec.asm)

uint64_t	str_len_asm(char *str);

void		quit_4_fd_asm(uint8_t fd1, uint8_t fd2, uint8_t fd3, uint8_t fd4, char *error);
void		quit_3_fd_asm(uint8_t fd1, uint8_t fd2, uint8_t fd3, char *error);
void		quit_2_fd_asm(uint8_t fd1, uint8_t fd2, char *error);
void		quit_fd_asm(uint8_t fd, char *error);
void		quit_asm(char *error);

uint64_t	sat_sub_asm(uint64_t lhs, uint64_t rhs);
uint8_t		seek_asm(uint8_t f1, uint8_t f2);

int64_t		get_fd_size_asm(int fd);

#endif // UTILS_ASM_H
