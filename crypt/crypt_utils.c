/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   crypt_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/05 15:41:37 by herrfalco         #+#    #+#             */
/*   Updated: 2022/06/05 16:03:40 by herrfalco        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"
#include "crypt.h"
#include "crypt_tables.h"

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

void			sub_word(uint8_t *word, method_t type) {
	static const uint8_t	s_box[] = S_BOX;
	const uint8_t			*box = s_box + type * 256;
	uint8_t					i;

	for (i = 0; i < 4; ++i)
		word[i] = box[(word[i] >> 4) * 16 | (word[i] & 0xf)];
}
