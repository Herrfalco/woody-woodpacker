/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aes_steps.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/05 15:40:47 by herrfalco         #+#    #+#             */
/*   Updated: 2022/06/16 13:32:01 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"
#include "aes.h"
#include "aes_steps.h"
#include "aes_utils.h"
#include "aes_tables.h"

//////////////////// MIX_COLUMNS ////////////////////////

void				mix_columns(uint8_t *block, method_t type) {
	static const uint8_t	mult_mat[] = MULT_MAT;
	static const int8_t		mix_mat[] = {
		0, 1, 6, 6, 6, 0, 1, 6, 6, 6, 0, 1, 1, 6, 6, 0,
		5, 3, 4, 2, 2, 5, 3, 4, 4, 2, 5, 3, 3, 4, 2, 5 };
	const int8_t			*mix = mix_mat + type * 16;
	uint8_t					tmp[16] = { 0 };
	uint8_t					x, y, i;

	for (x = 0; x < 4; ++x) {
		for (y = 0; y < 16; y += 4) {
			for (i = 0; i < 4; ++i) {
				tmp[y + x] ^= mix[y + i] == 6
					? block[i * 4 + x] : mult_mat[mix[y + i] * 256 + block[i * 4 + x]];
			}
		}
	}
	for (i = 0; i < 2; ++i)
		((uint64_t *)block)[i] = ((uint64_t *)tmp)[i];
}
