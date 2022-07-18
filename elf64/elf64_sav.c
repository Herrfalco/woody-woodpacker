/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf64_sav.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 22:56:40 by herrfalco         #+#    #+#             */
/*   Updated: 2022/07/17 15:13:32 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <elf.h>
#include <unistd.h>
#include "../utils/utils_asm.h"

#define PAGE_SZ			0x1000

extern uint8_t		sh_code;
extern uint8_t		sh_code_end;
extern uint8_t		sh_code_data;
extern uint8_t		sh_code_data_end;

void	quit_fd_unmap(int fd, void *mem, size_t size, char *msg) {
	munmap(mem, size);
	quit_fd_asm(fd, msg);
}

void	quit_unmap(void *mem, size_t size, char *msg) {
	quit_fd_unmap(-1, mem, size, msg);
}

int		write_pad(int fd, uint64_t size) {
	static uint8_t		buff[BUFF_SIZE] = { 0 };
	uint64_t			write_sz = 0;
	int64_t				w_ret;

	for (; size; size -= write_sz) {
		write_sz = size < BUFF_SIZE ? size : BUFF_SIZE;
		if ((w_ret = write(fd, buff, write_sz)) < 0
				|| (uint64_t)w_ret != write_sz)
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
	uint64_t	i, m_sz, code_sz, data_sz, load_sz, saved_txt_sz, old_txt_sz;
	int64_t		w_ret;
	uint8_t		*m_src;
	Elf64_Ehdr	*e_hdr;
	Elf64_Phdr	*p_hdr, *p_txt = NULL;
	Elf64_Shdr	*s_hdr;

	if (argc != 2)
		quit_asm("need 1 argument");	
	if ((src = open(argv[1], O_RDONLY)) < 0)
		quit_asm("can't open source file");	

	if ((int64_t)(m_sz = get_fd_size_asm(src)) < 0)
		quit_fd_asm(src, "can't get source file size");
	if ((m_src = mmap(NULL, m_sz, PROT_READ | PROT_WRITE, MAP_PRIVATE, src, 0)) == MAP_FAILED)
		quit_fd_asm(src, "can't map source file into memory");
	close(src);
	e_hdr = (Elf64_Ehdr *)m_src;

	code_sz = &sh_code_end - &sh_code;
	data_sz = &sh_code_data_end - &sh_code_data;
	load_sz = round_up_asm(data_sz + code_sz, PAGE_SZ);

	for (i = 0, p_hdr = (Elf64_Phdr *)(m_src + e_hdr->e_phoff);
			!p_txt && i < e_hdr->e_phnum; ++i, ++p_hdr) {
		if (p_hdr->p_type == PT_LOAD && (p_hdr->p_flags & PF_X)) {
			p_txt = p_hdr;
			saved_txt_sz = p_txt->p_filesz;
			p_txt->p_filesz += load_sz;
			p_txt->p_memsz += load_sz;
		}
	}

	if (p_hdr->p_type != PT_LOAD)
		quit_unmap(m_src, m_sz, "unsupported ELF binary");
	old_txt_sz = p_hdr->p_offset - p_txt->p_offset;

	for (i = 0, p_hdr = (Elf64_Phdr *)(m_src + e_hdr->e_phoff);
			i < e_hdr->e_phnum; ++i, ++p_hdr) {
		if (p_hdr->p_offset >= p_txt->p_offset + saved_txt_sz) {
			p_hdr->p_vaddr += load_sz;
			p_hdr->p_paddr += load_sz;
			p_hdr->p_offset += load_sz;
		}
	}

	for (i = 0, s_hdr = (Elf64_Shdr *)(m_src + e_hdr->e_shoff);
			i < e_hdr->e_shnum; ++i, ++s_hdr) {
		if (s_hdr->sh_offset >= p_txt->p_offset + saved_txt_sz) {
			s_hdr->sh_addr += load_sz;
			s_hdr->sh_offset += load_sz;
		}
	}

	e_hdr->e_shoff += load_sz;

	if ((dst = open("woody", O_WRONLY | O_CREAT | O_TRUNC, 0777)) < 0)
		quit_unmap(m_src, m_sz, "can't open destination file");	
	if ((w_ret = write(dst, m_src, p_txt->p_offset + saved_txt_sz)) < 0
			|| (uint64_t)w_ret != p_txt->p_offset + saved_txt_sz)
		quit_fd_unmap(dst, m_src, m_sz, "can't write to destination file");
	if (write_pad(dst, load_sz + (old_txt_sz - saved_txt_sz)))
		quit_fd_unmap(dst, m_src, m_sz, "can't write padding to destination file");
	if ((w_ret = write(dst, m_src + p_txt->p_offset + old_txt_sz,
					m_sz - (p_txt->p_offset + old_txt_sz))) < 0
			|| (uint64_t)w_ret != m_sz - (p_txt->p_offset + old_txt_sz))
		quit_fd_unmap(dst, m_src, m_sz, "can't write to destination file");

	munmap(m_src, m_sz);
	close(dst);
	return (0);
}
