/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf64.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 22:56:40 by herrfalco         #+#    #+#             */
/*   Updated: 2022/06/07 12:00:55 by herrfalco        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "elf64.h"

void	str_n_cpy(uint8_t *dst, uint8_t *src, size_t size) {
	while (size) {
		if (size >= 8) {
			*((uint64_t *)dst) = *((uint64_t *)src);
			dst += 8;
			src += 8;
			size -= 8;
		} else if (size >= 4) {
			*((uint32_t *)dst) = *((uint32_t *)src);
			dst += 4;
			src += 4;
			size -= 4;
		} else if (size >= 2) {
			*((uint16_t *)dst) = *((uint16_t *)src);
			dst += 2;
			src += 2;
			size -= 2;
		} else {
			*(dst++) = *(src++);
			size -= 1;
		}
	}
	*dst = '\0';
}

int		main(void) {
	uint8_t			magic[] = MAGIC;
	elf64_hdr_t		file_hdr = { 0 };
	seg_hdr_t		seg_hdr = { 0 };
	uint8_t			code[0x1000];

	str_n_cpy(file_hdr.ident.magic, magic, MAGIC_SZ);
	file_hdr.ident.bit_class = OS_64;
	file_hdr.ident.data_enc = LSB;
	file_hdr.ident.version = VERSION;
	file_hdr.ident.os = SYS_V;
	file_hdr.type = EXEC;
	file_hdr.machine = AMD64;
	file_hdr.version = VERSION;
	file_hdr.seg_hoff = sizeof(elf64_hdr_t);
	file_hdr.ehsize = sizeof(elf64_hdr_t);
	file_hdr.seg_hentsize = 0x1000;
	file_hdr.seg_hnum = 1;

	seg_hdr.type = LOAD;
	seg_hdr.flags = X;
	seg_hdr.offset = sizeof(elf64_hdr_t) + sizeof(seg_hdr_t);
	seg_hdr.vaddr = MAX_ADDRESS - 0x1000;
	seg_hdr.filesz = 0x1000;
	seg_hdr.memsz = 0x1000;
	seg_hdr.align = 2;
}
