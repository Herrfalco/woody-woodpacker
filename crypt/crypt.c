/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   crypt.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 13:56:24 by herrfalco         #+#    #+#             */
/*   Updated: 2022/06/04 23:44:40 by herrfalco        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "crypt.h"

static endian_t		get_endian(void) {
	uint16_t bytes = 1;

	return (*((uint8_t*)&bytes) == 1 ? LITTLE : BIG);
}

static void			sub_word(uint8_t *word, method_t type) {
	static const uint8_t	s_box[] = S_BOX;
	const uint8_t			*box = s_box + type * 256;
	uint8_t					i;

	for (i = 0; i < 4; ++i)
		word[i] = box[(word[i] >> 4) * 16 | (word[i] & 0xf)];
}

static void			sub_bytes(uint8_t *block, method_t type) {
	uint8_t			i;

	for (i = 0; i < 4; ++i)
		sub_word(block + i * 4, type);
}

static void			rot_word(uint32_t *word, uint8_t shift, method_t type) {
	static endian_t		endian = NO;

	if (endian == NO)
		endian = get_endian();
	if ((endian == LITTLE && type == ENCODE)
			|| (endian == BIG && type == DECODE))
		*word = (*word >> shift * 8) | (*word << (4 - shift) * 8);
	else
		*word = (*word << shift * 8) | (*word >> (4 - shift) * 8);
}

static void			shift_rows(uint8_t *block, method_t type) {
	uint8_t		i;
	uint32_t	*lines = (uint32_t *)block;

	for (i = 0; i < 4; ++i)
		rot_word(lines + i, i, type);
}

static void			mix_columns(uint8_t *block, method_t type) {
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

/*
void		print_block(char *title, uint8_t *block) {
	uint8_t		x, y;

	printf("%s:\n", title);
	for (y = 0; y < 4; ++y) {
		for (x = 0; x < 4; ++x) {
			printf("%02x ", block[y * 4 + x]);
		}
		printf("\n");
	}
	printf("\n");
}
*/

int					rand_key(uint8_t *buff, size_t size) {
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

static uint8_t		get_rnb(void) {
	switch (KEY_SIZE) {
		case 128: 
			return (11);
		case 192:
			return (13);
		default:
			return (15);
	}
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

static void			add_rkeys(uint8_t *block, uint32_t *rkeys, uint8_t *rk_i) {
	uint8_t				i;

	for (i = 0; i < 4; ++i, ++(*rk_i))
		((uint32_t *)block)[i] ^= rkeys[*rk_i];
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

 int				convert_data(uint8_t *data, uint64_t size, uint32_t *r_keys, method_t type) {
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
