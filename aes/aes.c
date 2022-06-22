/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aes.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 13:56:24 by herrfalco         #+#    #+#             */
/*   Updated: 2022/06/22 22:45:27 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"
#include "aes.h"
#include "aes_utils.h"
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

static void			encode_block(uint8_t *block, uint32_t *rkeys) {
	uint8_t		round;
	uint8_t		r_nb = get_rnb_asm(KEY_SIZE);

	for (round = 0; round < r_nb; ++round) {
		if (round) {
			sub_bytes_asm(block, ENCODE);
			shift_rows_asm(block, ENCODE);
			if (round + 1 < r_nb)
				mix_columns_asm(block, ENCODE);
		}
		add_rkeys_asm(block, rkeys, round);
	}
}

void				aes_data_enc(uint8_t *data, uint64_t size, uint32_t *r_keys) {
	uint64_t	i;

	for (i = 0; i < size; i += 16)
		encode_block(data + i, r_keys);
}
