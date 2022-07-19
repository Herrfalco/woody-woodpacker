/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf64.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 22:56:40 by herrfalco         #+#    #+#             */
/*   Updated: 2022/07/19 15:09:38 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <elf.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>

#define PAGE_SZ			0x1000
#define PAGE_MSK		0xfffffffffffff000
#define BUFF_SZ			1024

extern uint8_t		sc;
extern uint8_t		sc_end;
extern uint8_t		sc_data;
extern uint8_t		sc_data_end;
extern uint64_t		sc_entry;
extern uint64_t		sc_old_entry;

void	quit_fd_unmap(int fd, void *mem, size_t size, char *msg) {
	munmap(mem, size);
	if (fd >= 0)
		close(fd);
	if (msg) {
		fprintf(stderr, "Error: %s\n", msg);
		exit(42);
	}
	quit_fd_asm(fd, msg);
}

void	quit_unmap(void *mem, size_t size, char *msg) {
	quit_fd_unmap(-1, mem, size, msg);
}

int		write_pad(int fd, uint64_t size) {
	static uint8_t		buff[BUFF_SZ] = { 0 };
	uint64_t			write_sz = 0;
	int64_t				w_ret;

	for (; size; size -= write_sz) {
		write_sz = size < BUFF_SZ ? size : BUFF_SZ;
		if ((w_ret = write(fd, buff, write_sz)) < 0
				|| (uint64_t)w_ret != write_sz)
			return (-1);
	}
	return (0);
}

int		main(int argc, char **argv) {
	int			src, dst;
	uint64_t	i, m_sz, code_sz, data_sz, load_sz, m_space, f_space;
	int64_t		w_ret;
	uint8_t		*m_src;
	Elf64_Ehdr	*e_hdr;
	Elf64_Phdr	*p_hdr, *p_txt = NULL, *p_nxt;

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

	code_sz = &sc_end - &sc;
	data_sz = &sc_data_end - &sc_data;
	load_sz = data_sz + code_sz;

//	printf("load size: %ld\n", load_sz);

	for (i = 0, p_hdr = (Elf64_Phdr *)(m_src + e_hdr->e_phoff);
			!p_txt && i < e_hdr->e_phnum; ++i, ++p_hdr) {
		if (p_hdr->p_type == PT_LOAD && (p_hdr->p_flags & PF_X))
			p_txt = p_hdr;
	}

	if (!p_txt || i == e_hdr->e_phnum)
		quit_unmap(m_src, m_sz, "can't find needed segments");
	p_nxt = p_hdr;
	printf("next offset: 0x%lx\n", p_nxt->p_offset);
	printf("next vaddr: 0x%lx\n", p_nxt->p_vaddr);

	m_space = p_nxt->p_vaddr - (p_txt->p_vaddr + p_txt->p_memsz);
	f_space = p_nxt->p_offset - (p_txt->p_offset + p_txt->p_filesz);

	if (m_space < load_sz)
		quit_unmap(m_src, m_sz, "no space found for injection");

	if (f_space < load_sz)
		quit_unmap(m_src, m_sz, "need to add padding");

	syscall(10, (uint64_t)&sc_data & PAGE_MSK,
			round_up_asm(data_sz, PAGE_SZ), PROT_READ | PROT_WRITE | PROT_EXEC);
	sc_entry = p_txt->p_vaddr + p_txt->p_memsz;
	printf("new entry: 0x%lx\n", sc_entry);
	sc_old_entry = e_hdr->e_entry;
	e_hdr->e_entry = sc_entry;

	p_txt->p_filesz += load_sz;
	p_txt->p_memsz += load_sz;

	if ((dst = open("woody", O_WRONLY | O_CREAT | O_TRUNC, 0777)) < 0)
		quit_unmap(m_src, m_sz, "can't open destination file");	
	if ((w_ret = write(dst, m_src, e_hdr->e_entry)) < 0
			|| (uint64_t)w_ret != e_hdr->e_entry
			|| (w_ret = write(dst, &sc, load_sz)) < 0
			|| (uint64_t)w_ret != load_sz)
		quit_fd_unmap(dst, m_src, m_sz, "can't write to destination file");
	if (write_pad(dst, f_space - load_sz))
		quit_fd_unmap(dst, m_src, m_sz, "can't write padding to destination file");
	if ((w_ret = write(dst, m_src + e_hdr->e_entry + f_space, m_sz - (e_hdr->e_entry + f_space))) < 0
			|| (uint64_t)w_ret != m_sz - (e_hdr->e_entry + f_space))
		quit_fd_unmap(dst, m_src, m_sz, "can't write to destination file");

	munmap(m_src, m_sz);
	close(dst);
	return (0);
}
