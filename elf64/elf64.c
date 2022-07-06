/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf64.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 22:56:40 by herrfalco         #+#    #+#             */
/*   Updated: 2022/07/06 13:32:57 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <elf.h>
#include <unistd.h>
#include "../utils/utils_asm.h"

#define PRE_CODE		"\x55" \
						"\x48\x89\xe5" \
						"\x48\x83\xe4\xf0" \
						"\x48\x83\xec\x10" \
						"\x48\xb8\x2e\x2e\x2e\x2e\x57" \
						"\x4f\x4f\x44" \
						"\x48\x89\x04\x24" \
						"\x48\xb8\x59\x2e\x2e\x2e\x2e" \
						"\x0a\x00\x00" \
						"\x48\x89\x44\x24\x08" \
						"\xbf\x01\x00\x00\x00" \
						"\x48\x89\xe6" \
						"\xba\x0e\x00\x00\x00" \
						"\xb8\x01\x00\x00\x00" \
						"\x0f\x05" \
						"\x48\x89\xec" \
						"\x5d" \
						"\x48\xb8"
#define PRE_CODE_SZ		67
#define POST_CODE		"\xff\xe0"
#define POST_CODE_SZ	2

int		file_cpy(int src) {
	uint8_t			buff[BUFF_SIZE] = { 0 };
	int				dst = open("woody", O_RDWR | O_TRUNC | O_CREAT, 0777);
	uint64_t		read_ret;

	if (dst < 0)
		return (-1);
	while ((read_ret = read(src, buff, BUFF_SIZE)) > 0) {
		if (write(dst, buff, read_ret) != read_ret) {
			close(dst);
			return (-1);
		}
	}
	if (read_ret < 0 || lseek(dst, 0, SEEK_SET) < 0) {
		close(dst);
		return (-1);
	}
	return (dst);
}

int		str_cmp(uint8_t *s1, uint8_t *s2) {
	for (; *s1 && *s1 == *s2; ++s1, ++s2);
	return (*s1 - *s2);
}

int		main(int argc, char **argv) {
	int			src, dst;
	uint64_t	i, saved_entry;
	uint8_t		str_tab[BUFF_SIZE];

	Elf64_Ehdr  hdr;
	Elf64_Shdr	s_hdr, s_stab;
	Elf64_Phdr	p_hdr;

	if (argc != 2)
		quit_asm("need 1 argument");	
	if ((src = open(argv[1], O_RDONLY)) < 0)
		quit_asm("can't open source file");	

	if ((dst = file_cpy(src)) < 0)
		quit_fd_asm(src, "can't copy srouce file");
	close(src);
	
	if (read(dst, &hdr, sizeof(hdr)) != hdr.e_ehsize)
		quit_fd_asm(dst, "can't read from source file");

	saved_entry = hdr.e_entry;

	if (hdr.e_shstrndx == SHN_XINDEX)
		printf("chiant\n");
	switch (hdr.e_shstrndx) {
		case SHN_UNDEF:
			printf("no\n");
			break;
		case SHN_XINDEX:
			printf("chiant\n");
			break;
		default:
			printf("0x%x\n", hdr.e_shstrndx);
	}

	if (lseek(dst, hdr.e_shoff + hdr.e_shstrndx * hdr.e_shentsize, SEEK_SET) < 0)
		quit_fd_asm(dst, "can't seek into destination file");
	if (read(dst, &s_stab, hdr.e_shentsize) != hdr.e_shentsize)
		quit_fd_asm(dst, "can't read from source file");
	if (s_stab.sh_type == SHT_STRTAB)
		printf("str tab ok\n");
	else
		printf("not an str tab\n");
	if (s_stab.sh_size > BUFF_SIZE)
		quit_fd_asm(dst, "buffer is not big enough");
	if (lseek(dst, s_stab.sh_offset, SEEK_SET) < 0)
		quit_fd_asm(dst, "can't seek into destination file");
	if (read(dst, str_tab, s_stab.sh_size) != s_stab.sh_size)
		quit_fd_asm(dst, "can't read from source file");
	if (lseek(dst, hdr.e_shoff, SEEK_SET) < 0)
		quit_fd_asm(dst, "can't seek into destination file");
	for (i = 0; i < hdr.e_shnum; ++i) {
		if (read(dst, &s_hdr, hdr.e_shentsize) != hdr.e_shentsize)
			quit_fd_asm(dst, "can't read from source file");
		if (!str_cmp(str_tab + s_hdr.sh_name, ".data"))
			break;
	}
	if (i == hdr.e_shnum)
		quit_fd_asm(dst, "can't find data section");
	if (lseek(dst, hdr.e_phoff, SEEK_SET) < 0)
		quit_fd_asm(dst, "can't seek into destination file");
	for (i = 0; i < hdr.e_phnum; ++i) {
		if (read(dst, &p_hdr, hdr.e_phentsize) != hdr.e_phentsize)
			quit_fd_asm(dst, "can't read from source file");
		if (s_hdr.sh_addr >= p_hdr.p_vaddr && s_hdr.sh_addr < p_hdr.p_vaddr + p_hdr.p_memsz)
			break;
	}
	printf("%ld\n", i);
	return (0);
}
