/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aes_steps.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/05 15:40:47 by herrfalco         #+#    #+#             */
/*   Updated: 2022/06/09 13:04:06 by herrfalco        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"
#include "aes.h"
#include "aes_steps.h"
#include "aes_utils.h"
#include "aes_tables.h"

//////////////////// SUB_BYTES ////////////////////////

void				sub_bytes(uint8_t *block, method_t type) {
	uint8_t			i;

	for (i = 0; i < 4; ++i)
		sub_word(block + i * 4, type);
}

//////////////////// SHIFT_ROWS ////////////////////////

void				shift_rows(uint8_t *block, method_t type) {
	uint8_t		i;
	uint32_t	*lines = (uint32_t *)block;

	for (i = 0; i < 4; ++i)
		rot_word(lines + i, i, type);
}

//////////////////// MIX_COLUMNS ////////////////////////

void				mix_columns(uint8_t *block, method_t type) {
	static const uint8_t	mult_mat[] = MULT_MAT;
	static const int8_t		mix_mat[] = {
		0, 1, -1, -1, -1, 0, 1, -1, -1, -1, 0, 1, 1, -1, -1,
		0, 5, 3, 4, 2, 2, 5, 3, 4, 4, 2, 5, 3, 3, 4, 2, 5 };
	const int8_t			*mix = mix_mat + type * 16;
	uint8_t					tmp[16] = { 0 };
	uint8_t					x, y, i;

	for (x = 0; x < 4; ++x) {
		for (y = 0; y < 4; ++y) {
			for (i = 0; i < 4; ++i) {
				tmp[y * 4 + x] ^= mix[y * 4 + i] < 0
					? block[i * 4 + x] : mult_mat[mix[y * 4 + i] * 256 + block[i * 4 + x]];
			}
		}
	}
	for (i = 0; i < 2; ++i)
		((uint64_t *)block)[i] = ((uint64_t *)tmp)[i];
}

//////////////////// ADD_ROUND_KEYS ////////////////////////

void			add_rkeys(uint8_t *block, uint32_t *rkeys, uint8_t *rk_i) {
	uint8_t				i;

	for (i = 0; i < 4; ++i, ++(*rk_i))
		((uint32_t *)block)[i] ^= rkeys[*rk_i];
}
