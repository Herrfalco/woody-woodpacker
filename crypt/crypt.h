/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   crypt.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 23:12:22 by herrfalco         #+#    #+#             */
/*   Updated: 2022/06/04 23:45:02 by herrfalco        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CRYPT_H
#define CRYPT_H

#include "../includes.h"
#include "crypt_tables.h"

#define KEY_SIZE		256		// 128 | 192 | 256
#define RKEY_ARR_SIZE	60
#define BUFF_SIZE		1024

typedef enum		method_e {
	ENCODE = 0,
	DECODE = 1,
}					method_t;

typedef enum		endian_e {
	NO,
	BIG,
	LITTLE,
}					endian_t;

int					rand_key(uint8_t *buff, size_t size);
void				round_keys(uint8_t *key, uint32_t *rkeys);
int					convert_data(uint8_t *data, uint64_t size, uint32_t *r_keys, method_t type);

#endif // CRYPT_H
