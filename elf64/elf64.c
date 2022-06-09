/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf64.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 22:56:40 by herrfalco         #+#    #+#             */
/*   Updated: 2022/06/09 10:50:22 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "elf64.h"

#define CODE		"\xb8\x01\x00\x00\x00" \
					"\xbf\x01\x00\x00\x00" \
					"\xbe\x00\x10\x40\x00" \
					"\xba\x0e\x00\x00\x00" \
					"\x0f\x05" \
					"\xb8\x3c\x00\x00\x00" \
					"\x48\x31\xff" \
					"\x0f\x05"

#define CODE_SZ		32

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

void	write_seg(int fd, uint32_t flags, uint64_t offset, uint64_t addr, uint64_t size) {
	seg_hdr_t		seg_hdr = { 0 };

	seg_hdr.type = LOAD;
	seg_hdr.flags = flags,
	seg_hdr.offset = offset,
	seg_hdr.vaddr = addr,
	seg_hdr.paddr = addr,
	seg_hdr.filesz = size,
	seg_hdr.memsz = size,
	seg_hdr.align = PAGE_SZ;
	write(fd, &seg_hdr, sizeof(seg_hdr));
}

int		main(void) {
	uint8_t			magic[] = MAGIC;
	elf64_hdr_t		file_hdr = { 0 };
	uint8_t			*code = (uint8_t *)CODE;
	int				fd = open("test.exe", O_WRONLY | O_CREAT | O_TRUNC, 0777);
	uint8_t			fill[0x1000] = { 0 };

	if (fd < 0)
		fprintf(stderr, "Error: Can't open file test.exe\n");
	str_n_cpy(file_hdr.ident.magic, magic, MAGIC_SZ);
	file_hdr.ident.bit_class = OS_64;
	file_hdr.ident.data_enc = LSB;
	file_hdr.ident.version = VERSION;
	file_hdr.ident.os = SYS_V;
	file_hdr.type = EXEC;
	file_hdr.machine = AMD64;
	file_hdr.version = VERSION;
	file_hdr.entry = BASE_ADDR;
	file_hdr.seg_hoff = sizeof(elf64_hdr_t);
	file_hdr.ehsize = sizeof(elf64_hdr_t);
	file_hdr.seg_hentsize = sizeof(seg_hdr_t);
	file_hdr.seg_hnum = 2;
	write(fd, &file_hdr, sizeof(file_hdr));

	write_seg(fd, X, 0x1000, BASE_ADDR, CODE_SZ);
	write_seg(fd, R, 0x2000, BASE_ADDR + 0x1000, 14);

	write(fd, code, CODE_SZ);
	write(fd, fill, 0x1000 - CODE_SZ);
	write(fd, "Helloworld...\n", 14);
	return (0);
}
