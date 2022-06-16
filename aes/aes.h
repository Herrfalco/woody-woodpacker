/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aes.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 23:12:22 by herrfalco         #+#    #+#             */
/*   Updated: 2022/06/16 12:19:42 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AES_H
#define AES_H

#define KEY_SIZE		256		// 128 | 192 | 256
#define RKEY_ARR_SIZE	60

typedef enum		method_e {
	ENCODE = 0,
	DECODE = 1,
}					method_t;

int		rand_key(uint8_t *buff, size_t size);
void	round_keys(uint8_t *key, uint32_t *rkeys);

void	aes_data(uint8_t *data, uint64_t size, uint32_t *r_keys, method_t type);
void	aes_fd(int fd_dst, int fd_src, uint8_t *key, method_t type);

#endif // AES_H
