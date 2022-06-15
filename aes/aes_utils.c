/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aes_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/05 15:41:37 by herrfalco         #+#    #+#             */
/*   Updated: 2022/06/15 13:37:48 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"
#include "aes.h"
#include "aes_utils.h"
#include "aes_tables.h"

uint64_t		round_up(uint64_t val, uint64_t mod) {
	return (val % mod ? (val / mod) * mod + mod : val);
}

endian_t		get_endian(void) {
	uint16_t bytes = 1;

	return (*((uint8_t*)&bytes) == 1 ? LITTLE : BIG);
}

uint8_t			get_rnb(void) {
	switch (KEY_SIZE) {
		case 128: 
			return (11);
		case 192:
			return (13);
		default:
			return (15);
	}
}

void			rot_word(uint32_t *word, uint8_t shift, method_t type) {
	static endian_t		endian = NO;

	if (endian == NO)
		endian = get_endian();
	if ((endian == LITTLE && type == ENCODE)
			|| (endian == BIG && type == DECODE))
		*word = (*word >> shift * 8) | (*word << (4 - shift) * 8);
	else
		*word = (*word << shift * 8) | (*word >> (4 - shift) * 8);
}
