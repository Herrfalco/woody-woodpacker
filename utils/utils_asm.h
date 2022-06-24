/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_asm.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 22:07:07 by herrfalco         #+#    #+#             */
/*   Updated: 2022/06/24 14:13:47 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_ASM_H
#define UTILS_ASM_H

uint64_t	str_len_asm(char *str);
uint64_t	sat_sub_asm(uint64_t lhs, uint64_t rhs);

void		quit_4_fd_asm(int64_t fd1, int64_t fd2, int64_t fd3, int64_t fd4, char *error);
void		quit_3_fd_asm(int64_t fd1, int64_t fd2, int64_t fd3, char *error);
void		quit_2_fd_asm(int64_t fd1, int64_t fd2, char *error);
void		quit_fd_asm(int64_t fd, char *error);
void		quit_asm(char *error);

int64_t		seek_4_fd_asm(int64_t fd1, int64_t fd2, int64_t fd3, int64_t fd4);
int64_t		seek_3_fd_asm(int64_t fd1, int64_t fd2, int64_t fd3);
int64_t		seek_2_fd_asm(int64_t fd1, int64_t fd2);
int64_t		seek_fd_asm(int64_t f1);

int64_t		get_fd_size_asm(int64_t fd);

#endif // UTILS_ASM_H
