/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_rw.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 22:04:23 by herrfalco         #+#    #+#             */
/*   Updated: 2022/06/07 14:59:12 by herrfalco        ###   ########.fr       */
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

int		file_reader(int fd, uint8_t *byte);
int		file_writer(int fd, uint8_t byte, flush_t flush);
int		value_writer(int fd, uint16_t value, size_t size, flush_t flush);
int		value_reader(int fd, uint16_t *value, size_t size);

#endif // BYTE_RW_H
