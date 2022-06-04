/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_rw.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 22:04:23 by herrfalco         #+#    #+#             */
/*   Updated: 2022/06/04 23:31:43 by herrfalco        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BYTE_RW_H
#define BYTE_RW_H

#include "../includes.h"

#define BUFF_SIZE		1024

int		value_writer(int fd, uint16_t value, size_t size, bool_t last_value);
int		value_reader(int fd, uint16_t *value, size_t size);

#endif // BYTE_RW_H
