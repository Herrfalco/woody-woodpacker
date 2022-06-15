/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aes.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 13:56:24 by herrfalco         #+#    #+#             */
/*   Updated: 2022/06/15 15:38:24 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"
#include "aes.h"
#include "aes_steps.h"
#include "aes_utils.h"
#include "aes_tables.h"
#include "asm/aes_asm.h"

int				rand_key(uint8_t *buff, size_t size) {
	int			fd = open("/dev/urandom", O_RDONLY);
	size_t		i;

	if (fd < 0 || read(fd, buff, size) < (ssize_t)size) {
		close(fd);
		return (-1);
	}
	for (i = 0; i < size; ++i) {
		buff[i] = buff[i] % 62;
		if (buff[i] < 10)
			buff[i] += '0';
		else if (buff[i] < 36)
			buff[i] += 'A' - 10;
		else
			buff[i] += 'a' - 36;
	}
	close(fd);
	return (0);
}

static void			sub_word(uint8_t *word, method_t type) {
	static const uint8_t	s_box[] = S_BOX;
	const uint8_t			*box = s_box + type * 256;
	uint8_t					i;

	for (i = 0; i < 4; ++i)
		word[i] = box[i];
}

void				round_keys(uint8_t *key, uint32_t *rkeys) {
	static const int32_t	rcon[] = {
		0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000,
		0x20000000, 0x40000000, 0x80000000, 0x1b000000, 0x36000000 };
	uint8_t					i;
	uint8_t					n = KEY_SIZE / 32;
	uint32_t				tmp;

	for (i = 0; i < 4 * get_rnb(); ++i) {
		if (i < n)
			rkeys[i] = ((uint32_t *)key)[i];
		else if (!(i % n)) {
			tmp = rkeys[i - 1];
			rot_word(&tmp, 1, ENCODE);
			sub_word((uint8_t *)&tmp, ENCODE);
			rkeys[i] = rkeys[i - n] ^ tmp ^ rcon[i / n];
		} else if (n > 6 && i % n == 4) {
			tmp = rkeys[i - 1];
			sub_word((uint8_t *)&tmp, ENCODE);
			rkeys[i] = rkeys[i - n] ^ tmp;
		} else
			rkeys[i] = rkeys[i - n] ^ rkeys[i - 1];
	}
}

static void			encode_block(uint8_t *block, uint32_t *rkeys) {
	uint8_t		round;
	uint8_t		r_nb = get_rnb();

	for (round = 0; round < r_nb; ++round) {
		if (round) {
			sub_bytes_asm(block, ENCODE);
			shift_rows(block, ENCODE);
			if (round + 1 < r_nb)
				mix_columns(block, ENCODE);
		}
		add_rkeys(block, rkeys, round);
	}
}

void				aes_data(uint8_t *data, uint64_t size, uint32_t *r_keys, method_t type) {
	uint64_t	i;

	if (type == ENCODE)
		for (i = 0; i < size; i += 16)
			encode_block(data + i, r_keys);
	else
		for (i = 0; i < size; i += 16)
			decode_block_asm(data + i, r_keys);
}
