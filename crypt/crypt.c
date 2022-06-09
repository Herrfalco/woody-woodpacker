/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   crypt.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 13:56:24 by herrfalco         #+#    #+#             */
/*   Updated: 2022/06/08 12:56:05 by herrfalco        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"
#include "crypt.h"
#include "crypt_steps.h"
#include "crypt_utils.h"

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
	uint8_t		rk_i = 0;
	uint8_t		round;
	uint8_t		r_nb = get_rnb();

	for (round = 0; round < r_nb; ++round) {
		if (round) {
			sub_bytes(block, ENCODE);
			shift_rows(block, ENCODE);
			if (round + 1 < r_nb)
				mix_columns(block, ENCODE);
		}
		add_rkeys(block, rkeys, &rk_i);
	}
}

static void			decode_block(uint8_t *block, uint32_t *rkeys) {
	uint8_t		rk_i;
	int8_t		round;
	uint8_t		r_nb = get_rnb();

	for (round = r_nb - 1; round >= 0; --round) {
		rk_i = round * 4;
		add_rkeys(block, rkeys, &rk_i);
		if (round) {
			if (round + 1 < r_nb)
				mix_columns(block, DECODE);
			shift_rows(block, DECODE);
			sub_bytes(block, DECODE);
		}
	}
}

int					convert_data(uint8_t *data, uint64_t size, uint32_t *r_keys, method_t type) {
	uint64_t	i;

	if (size % 16) {
		fprintf(stderr, "Error: encode_data(data, size) -> data size must be divisible by 16\n");
		return (-1);
	}
	if (type == ENCODE)
		for (i = 0; i < size; i += 16)
			encode_block(data + i, r_keys);
	else
		for (i = 0; i < size; i += 16)
			decode_block(data + i, r_keys);
	return (0);
}
