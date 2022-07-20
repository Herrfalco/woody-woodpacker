/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   includes.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/20 16:36:11 by fcadet            #+#    #+#             */
/*   Updated: 2022/07/20 19:02:43 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDES_H
#define INCLUDES_H

#include <elf.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define PAGE_SZ			0x1000
#define PAGE_MSK		0xfffffffffffff000
#define BLOCK_MSK		0xfffffffffffffff0
#define BUFF_SZ			1024
#define KEY_SZ			128

typedef struct		s_hdrs {
	Elf64_Ehdr	*elf;
	Elf64_Phdr	*p_txt;
	Elf64_Phdr	*p_nxt;
	Elf64_Shdr	*s_txt;
}					t_hdrs;

typedef struct		s_sizes {
	uint64_t	mem;
	uint64_t	code;
	uint64_t	data;
	uint64_t	load;
	uint64_t	f_pad;
	uint64_t	m_pad;
}					t_sizes;

extern uint8_t		sc;
extern uint8_t		sc_end;
extern uint8_t		sc_data;
extern uint8_t		sc_data_end;
extern uint64_t		sc_entry;
extern uint64_t		sc_text_addr;
extern uint64_t		sc_text_sz;
extern uint64_t		sc_old_entry;
extern uint8_t		sc_key;

extern void			aes_encoding(uint8_t *data, uint8_t *key);

void				error_fd_unmap(int fd, void *mem, size_t size, char *msg);
void				error_unmap(void *mem, size_t size, char *msg);
void				error_fd(int fd, char *msg);
void				error(char *msg);

uint64_t			round_up(uint64_t val, uint64_t mod);
int					str_cmp(uint8_t *s1, uint8_t *s2);

int					write_pad(int fd, uint64_t size);
int64_t				get_fd_size(int fd);

#endif //INCLUDES_H
