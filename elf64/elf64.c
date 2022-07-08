/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf64.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 22:56:40 by herrfalco         #+#    #+#             */
/*   Updated: 2022/07/08 13:47:42 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <elf.h>
#include <unistd.h>
#include "../utils/utils_asm.h"

#define PRE_CODE		"\x55\x48\x89\xe5\x48\x83\xe4\xf0" \
						"\x48\x83\xec\x10\x48\xbb\x2e\x2e" \
						"\x2e\x2e\x57\x4f\x4f\x44\x48\x89" \
						"\x1c\x24\x48\xbb\x59\x2e\x2e\x2e" \
						"\x2e\x0a\x00\x00\x48\x89\x5c\x24" \
						"\x08\xbf\x01\x00\x00\x00\x48\x89" \
						"\xe6\xba\x0e\x00\x00\x00\xb8\x01" \
						"\x00\x00\x00\x0f\x05\x48\xbb"
#define PRE_CODE_SZ		63
#define IN_CODE			"\x48\x29\xd8\x48\xbb"
#define IN_CODE_SZ		5
#define POST_CODE		"\x48\x01\xd8\xff\xd0\x48\x89\xec" \
						"\x5d"
#define POST_CODE_SZ	9

#define CODE_SZ			PRE_CODE_SZ + 8 + IN_CODE_SZ + 8 + POST_CODE_SZ
#define PAGE_SZ			4096

int		file_cpy(int dst, int src, int64_t size) {
	uint8_t			buff[BUFF_SIZE] = { 0 };
	int64_t			read_ret;

	while (size) {
		if ((read_ret = read(src, buff, size < 0 || size > BUFF_SIZE ? BUFF_SIZE : size)) < 1
				|| write(dst, buff, read_ret) != read_ret) {
			if (size < 0 && !read_ret)
				return (0);
			return (-1);
		}
		if (size > 0)
			size = sat_sub_asm(size, BUFF_SIZE);
	}
	return (0);
}

int		file_zero(int dst, uint64_t size) {
	uint8_t			buff[BUFF_SIZE] = { 0 };
	uint64_t		write_sz;

	for (; size; size = sat_sub_asm(size, BUFF_SIZE)) { 
		write_sz = size > BUFF_SIZE ? BUFF_SIZE : size;
		if (write(dst, buff, write_sz) < 0)
			return (-1);
	}
	return (0);
}

int		str_cmp(uint8_t *s1, uint8_t *s2) {
	for (; *s1 && *s1 == *s2; ++s1, ++s2);
	return (*s1 - *s2);
}

int		main(int argc, char **argv) {
	int			src, dst;
	uint64_t	i, s_data_i, p_data_i, saved_entry, bss_sz, bss_off, load_size;
	int64_t		src_sz;
	uint8_t		str_tab[BUFF_SIZE];

	Elf64_Ehdr  hdr;
	Elf64_Shdr	s_data, s_hdr, s_stab;
	Elf64_Phdr	p_data, p_hdr;

	if (argc != 2)
		quit_asm("need 1 argument");	
	if ((src = open(argv[1], O_RDONLY)) < 0)
		quit_asm("can't open source file");	

	if ((src_sz = get_fd_size_asm(src)) < 0)
		quit_fd_asm(src, "can't get source file size");
	if (read(src, &hdr, sizeof(hdr)) != hdr.e_ehsize)
		quit_fd_asm(src, "can't read from source file");

	saved_entry = hdr.e_entry;

	switch (hdr.e_shstrndx) {
		case SHN_UNDEF:
			printf("no\n");
			return (2);
		case SHN_XINDEX:
			printf("chiant\n");
			return (1);
		default:
			printf("0x%x\n", hdr.e_shstrndx);
	}

	if (lseek(src, hdr.e_shoff + hdr.e_shstrndx * hdr.e_shentsize, SEEK_SET) < 0)
		quit_fd_asm(src, "can't seek into destination file");
	if (read(src, &s_stab, hdr.e_shentsize) != hdr.e_shentsize)
		quit_fd_asm(src, "can't read from source file");
	if (s_stab.sh_type == SHT_STRTAB)
		printf("str tab ok\n");
	else
		printf("not an str tab\n");
	if (s_stab.sh_size > BUFF_SIZE)
		quit_fd_asm(src, "buffer is not big enough");

	if (lseek(src, s_stab.sh_offset, SEEK_SET) < 0)
		quit_fd_asm(src, "can't seek into destination file");
	if (read(src, str_tab, s_stab.sh_size) != (int64_t)s_stab.sh_size)
		quit_fd_asm(src, "can't read from source file");
	if (lseek(src, hdr.e_shoff, SEEK_SET) < 0)
		quit_fd_asm(src, "can't seek into destination file");
	for (s_data_i = 0; s_data_i < hdr.e_shnum; ++s_data_i) {
		if (read(src, &s_data, hdr.e_shentsize) != hdr.e_shentsize)
			quit_fd_asm(src, "can't read from source file");
		if (!str_cmp(str_tab + s_data.sh_name, (uint8_t *)".data"))
			break;
	}
	if (s_data_i == hdr.e_shnum)
		quit_fd_asm(src, "can't find data section");
	if (lseek(src, hdr.e_phoff, SEEK_SET) < 0)
		quit_fd_asm(src, "can't seek into destination file");
	for (p_data_i = 0; p_data_i < hdr.e_phnum; ++p_data_i) {
		if (read(src, &p_data, hdr.e_phentsize) != hdr.e_phentsize)
			quit_fd_asm(src, "can't read from source file");
		if (s_data.sh_addr >= p_data.p_vaddr
				&& s_data.sh_addr < p_data.p_vaddr + p_data.p_memsz)
			break;
	}

	hdr.e_entry = p_data.p_vaddr + p_data.p_memsz;
	bss_sz = p_data.p_memsz - p_data.p_filesz;
	bss_off = p_data.p_offset + p_data.p_filesz;
	load_size = round_up_asm(bss_sz + CODE_SZ, PAGE_SZ);

	p_data.p_filesz += load_size;
	p_data.p_memsz += load_size - bss_sz;
	p_data.p_flags = PF_X | PF_W | PF_R;

	if ((dst = open("woody", O_WRONLY | O_CREAT | O_TRUNC, 0777)) < 0)
		quit_fd_asm(src, "can't open destination file");	
	if (write(dst, &hdr, sizeof(hdr)) != sizeof(hdr))
		quit_2_fd_asm(dst, src, "can't write to destination file");
	if (lseek(src, sizeof(hdr), SEEK_SET) < 0)
		quit_2_fd_asm(dst, src, "can't seek into source file");
	if (file_cpy(dst, src, bss_off - sizeof(hdr)) < 0)
		quit_2_fd_asm(dst, src, "can't copy from source file");

	if (file_zero(dst, bss_sz) < 0)
		quit_2_fd_asm(dst, src, "can't fill destination file with zeros");
	if (write(dst, PRE_CODE, PRE_CODE_SZ) != PRE_CODE_SZ
			|| write(dst, &hdr.e_entry, 8) != 8
			|| write(dst, IN_CODE, IN_CODE_SZ) != IN_CODE_SZ
			|| write(dst, &saved_entry, 8) != 8
			|| write(dst, POST_CODE, POST_CODE_SZ) != POST_CODE_SZ)
		quit_2_fd_asm(dst, src, "can't write to destination file");
	if (file_zero(dst, load_size - bss_sz - (CODE_SZ)) < 0)
		quit_2_fd_asm(dst, src, "can't fill destination file with zeros");
	if (file_cpy(dst, src, -1) < 0)
		quit_2_fd_asm(dst, src, "can't copy to destination file");

	if (lseek(dst, hdr.e_phoff, SEEK_SET) < 0)
		quit_2_fd_asm(src, dst, "can't seek into destination file");
	if (lseek(src, hdr.e_phoff, SEEK_SET) < 0)
		quit_2_fd_asm(src, dst, "can't seek into destination file");
	for (i = 0; i < hdr.e_phnum; ++i) {
		if (read(src, &p_hdr, hdr.e_phentsize) != hdr.e_phentsize)
			quit_2_fd_asm(src, dst, "can't read from destination file");
		if (i == p_data_i)
			p_hdr = p_data;
		if (p_hdr.p_offset > p_data.p_offset)
			p_hdr.p_offset += load_size;
		if (write(dst, &p_hdr, sizeof(p_hdr)) != sizeof(p_hdr))
			quit_2_fd_asm(src, dst, "can't write into destination file");
	}

	if (lseek(dst, hdr.e_shoff, SEEK_SET) < 0)
		quit_2_fd_asm(src, dst, "can't seek into destination file");
	if (lseek(src, hdr.e_shoff, SEEK_SET) < 0)
		quit_2_fd_asm(src, dst, "can't seek into destination file");
	for (i = 0; i < hdr.e_shnum; ++i) {
		if (read(src, &s_hdr, hdr.e_shentsize) != hdr.e_shentsize)
			quit_2_fd_asm(src, dst, "can't read from destination file");
		if (i == s_data_i)
			s_hdr = s_data;
		if (s_hdr.sh_offset > s_data.sh_offset)
			s_hdr.sh_offset += load_size;
		if (write(dst, &s_hdr, sizeof(s_hdr)) != sizeof(s_hdr))
			quit_2_fd_asm(src, dst, "can't write into destination file");
	}
	return (0);
}
