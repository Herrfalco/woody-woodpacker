/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aes_utils.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/05 15:48:44 by herrfalco         #+#    #+#             */
/*   Updated: 2022/06/15 13:43:34 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AES_UTILS_H
#define AES_UTILS_H

#include "aes.h"

typedef enum	endian_e {
	NO,
	BIG,
	LITTLE,
}				endian_t;

int				seek_ret(int f1, int f2, int ret);
int64_t			get_fd_size(int fd);
uint64_t		sat_sub(uint64_t lhs, uint64_t rhs);
uint64_t		round_up(uint64_t val, uint64_t mod);
endian_t		get_endian(void);
uint8_t			get_rnb(void);
void			rot_word(uint32_t *word, uint8_t shift, method_t type);

#endif // AES_UTILS_H
