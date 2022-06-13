/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf64.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 22:56:40 by herrfalco         #+#    #+#             */
/*   Updated: 2022/06/13 14:25:05 by fcadet           ###   ########.fr       */
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
	for (; size; ++dst, ++src, --size)
		*dst = *src;
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

void	write_hdr(int fd, uint64_t entry, uint16_t seg_nb) {
	static uint8_t			magic[] = MAGIC;
	elf64_hdr_t				file_hdr = { 0 };

	lseek(fd, 0, SEEK_SET);
	str_n_cpy(file_hdr.ident.magic, magic, MAGIC_SZ);
	file_hdr.ident.bit_class = OS_64;
	file_hdr.ident.data_enc = LSB;
	file_hdr.ident.version = VERSION;
	file_hdr.ident.os = SYS_V;
	file_hdr.type = EXEC;
	file_hdr.machine = AMD64;
	file_hdr.version = VERSION;
	file_hdr.entry = entry;
	file_hdr.seg_hoff = sizeof(elf64_hdr_t);
	file_hdr.ehsize = sizeof(elf64_hdr_t);
	file_hdr.seg_hentsize = sizeof(seg_hdr_t);
	file_hdr.seg_hnum = seg_nb;
	write(fd, &file_hdr, sizeof(file_hdr));
}

uint16_t	cpy_segs(int input, int output) {
	uint16_t	seg_nb, i;
	uint64_t	seg_tab, filesz;
	uint8_t		buff[BUFF_SIZE];
	seg_hdr_t	seg_hdr;
	seg_hdr_t	last_offset = { 0 };
	seg_hdr_t	last_vaddr = { 0 };

	lseek(input, SEG_NB, SEEK_SET);
	read(input, &seg_nb, sizeof(uint16_t));
	lseek(input, SEG_TAB, SEEK_SET);
	read(input, &seg_tab, sizeof(uint64_t));
	lseek(output, sizeof(elf64_hdr_t), SEEK_SET);
	for (i = 0; i < seg_nb; ++i) {
		lseek(input, seg_tab + i * sizeof(seg_hdr_t), SEEK_SET);
		read(input, &seg_hdr, sizeof(seg_hdr_t));	
		if (seg_hdr.offset > last_offset.offset)
			last_offset = seg_hdr;
		if (seg_hdr.vaddr > last_offset.vaddr)
			last_vaddr = seg_hdr;
		filesz = seg_hdr.filesz;
		write(output, &seg_hdr, sizeof(seg_hdr_t));
		lseek(input, seg_hdr.offset, SEEK_SET);
		lseek(output, seg_hdr.offset, SEEK_SET);
		for (; filesz; sat_sub(filesz, BUFF_SIZE)) {
			read(input, buff, filesz > BUFF_SIZE ? BUFF_SIZE : filesz);
			write(output
		}
	}
	return (seg_nb);
}

int		main(void) {
	uint8_t			*code = (uint8_t *)CODE;
	int				fd = open("test.exe", O_WRONLY | O_CREAT | O_TRUNC, 0777);
	uint8_t			fill[0x1000] = { 0 };

	if (fd < 0)
		fprintf(stderr, "Error: Can't open file test.exe\n");

	write_seg(fd, R | W | X, 0x1000, BASE_ADDR, CODE_SZ);
	write_seg(fd, R | W | X, 0x2000, BASE_ADDR + 0x1000, 14);

	write(fd, fill, 0x1000 - (sizeof(elf64_hdr_t) + 2 * sizeof(seg_hdr_t)));
	write(fd, code, CODE_SZ);
	write(fd, fill, 0x1000 - CODE_SZ);
	write(fd, "Helloworld...\n", 14);
	return (0);
}
