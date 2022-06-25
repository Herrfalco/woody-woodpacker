/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aes.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 23:12:22 by herrfalco         #+#    #+#             */
/*   Updated: 2022/06/25 13:10:37 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AES_H
#define AES_H

#include "../includes.h"

#define KEY_SIZE		256		// 128 | 192 | 256 (must update key_sz in aes_round_keys.asm and aes_fd_dec.asm)
#define RKEY_MAX_NB		60		// (must update rkey_arr_sz in aes_fd_dec.asm)

typedef enum		method_e {
	ENCODE,
	DECODE,
}					method_t;

int		rand_key(uint8_t *buff, size_t size);
void	round_keys(uint8_t *key, uint32_t *rkeys);

void	aes_data_enc(uint8_t *data, uint64_t size, uint32_t *r_keys);
void	aes_fd_enc(int fd_dst, int fd_src, uint8_t *key);

#endif // AES_H
