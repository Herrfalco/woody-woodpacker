/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lzw.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/28 14:34:29 by fcadet            #+#    #+#             */
/*   Updated: 2022/06/28 15:04:57 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMPR_H
# define COMPR_H

# define DICO_SIZE	3837
# define BUFF_SIZE	1024
# define RESET_CODE	256
# define STOP_CODE	257
# define INCR_CODE	258
# define DICO_START	259

# include "../includes.h"

typedef enum	flush_e {
	NO_FLUSH,
	FLUSH,
	ONLY_FLUSH,
}				flush_t;

struct			rw_buff_s {
	uint8_t		bytes[BUFF_SIZE];	// +0
	uint64_t	b_size;				// +1024
	uint64_t	idx;				// +1032
	uint32_t	dword;				// +1040
	uint64_t	dw_size;			// +1044
	uint8_t		error;				// +1052
}				__attribute__((packed));

typedef struct	rw_buff_s		rw_buff_t;

struct			s_dico {
	uint64_t	size;
	uint16_t	entry[DICO_SIZE][2];
	uint8_t		bits;
}				__attribute__((packed));

typedef struct	s_dico		t_dico;

int64_t		value_writer(int64_t fd, uint16_t value, uint64_t size, flush_t flush, rw_buff_t *buff);

int			get_bits_nb(uint64_t dico_size);
int64_t		check_dico(uint16_t last_value, uint16_t value, t_dico *dico);

void		lzw(int64_t dst, int64_t src);

#endif
