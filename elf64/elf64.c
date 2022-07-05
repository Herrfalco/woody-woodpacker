/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf64.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 22:56:40 by herrfalco         #+#    #+#             */
/*   Updated: 2022/07/04 15:57:37 by fcadet           ###   ########.fr       */
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

int		main(int argc, char **argv) {
	int			src, dst;
	uint64_t	i;

	Elf64_Ehdr  hdr;
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

	printf("0x%016lx\n", hdr.e_entry);

	for (i = 0; i < hdr.e_phnum; ++i) {
		if (read(dst, &p_hdr, hdr.e_phentsize) != hdr.e_phentsize)
			quit_fd_asm(dst, "can't read from source file");
		if (p_hdr.p_type == PT_NOTE)
			break;
	}

	if (i == hdr.e_phnum)
		quit_fd_asm(dst, "no NOTE segment in this file");

	p_hdr.p_type = PT_LOAD;

	if (lseek(dst, sizeof(p_hdr) * -1, SEEK_CUR) < 0)
		quit_fd_asm(dst, "can't seek into destination file");

	if (write(dst, &p_hdr, sizeof(p_hdr)) < 0)
		quit_fd_asm(dst, "can't write into destination file");

	return (0);
}
