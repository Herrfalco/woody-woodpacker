/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_rw.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 22:04:23 by herrfalco         #+#    #+#             */
/*   Updated: 2022/06/25 15:07:47 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BYTE_RW_H
#define BYTE_RW_H

#define BUFF_SIZE		1024

typedef enum	flush_e {
	NO_FLUSH,
	FLUSH,
	ONLY_FLUSH,
}				flush_t;

struct			b_buff_s {
	uint8_t		data[BUFF_SIZE];
	uint64_t	size;
	uint64_t	idx;
}				__attribute__((packed));

typedef struct	b_buff_s		b_buff_t;

struct			dw_buff_s {
	uint32_t	data;
	uint64_t	size;
}				__attribute__((packed));

typedef struct	dw_buff_s		dw_buff_t;

int		file_reader(int fd, uint8_t *byte, b_buff_t *buff);
int		file_writer(int fd, uint8_t byte, flush_t flush);
int		value_writer(int fd, uint16_t value, size_t size, flush_t flush);
int		value_reader(int fd, uint16_t *value, size_t size, dw_buff_t *vr_buff, b_buff_t *fr_buff);

#endif // BYTE_RW_H
