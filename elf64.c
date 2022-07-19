/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf64.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 22:56:40 by herrfalco         #+#    #+#             */
/*   Updated: 2022/07/19 15:33:37 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <elf.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define PAGE_SZ			0x1000
#define PAGE_MSK		0xfffffffffffff000
#define BUFF_SZ			1024

extern uint8_t		sc;
extern uint8_t		sc_end;
extern uint8_t		sc_data;
extern uint8_t		sc_data_end;
extern uint64_t		sc_entry;
extern uint64_t		sc_old_entry;

int64_t			get_fd_size(int fd) {
	int64_t		size;

	if ((size = lseek(fd, 0, SEEK_END)) < 0 || lseek(fd, 0, SEEK_SET) != 0)
		return (-1);
	return (size);
}

uint64_t		round_up(uint64_t val, uint64_t mod) {
	return (val % mod ? (val / mod) * mod + mod : val);
}

void	error_fd_unmap(int fd, void *mem, size_t size, char *msg) {
	munmap(mem, size);
	if (fd >= 0)
		close(fd);
	if (mem)
		munmap(mem, size);
	if (msg)
		fprintf(stderr, "Error: %s\n", msg);
	exit(42);
}

void	error_unmap(void *mem, size_t size, char *msg) {
	error_fd_unmap(-1, mem, size, msg);
}

void	error_fd(int fd, char *msg) {
	error_fd_unmap(fd, NULL, 0, msg);
}

void	error(char *msg) {
	error_fd_unmap(-1, NULL, 0, msg);
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
	int64_t		w_ret, s_ret;
	uint8_t		*m_src;
	Elf64_Ehdr	*e_hdr;
	Elf64_Phdr	*p_hdr, *p_txt = NULL, *p_nxt;

	if (argc != 2)
		error("need 1 argument");	
	if ((src = open(argv[1], O_RDONLY)) < 0)
		error("can't open source file");	

	if ((s_ret = get_fd_size(src)) < 0)
		error_fd(src, "can't get source file size");
	m_sz = (uint64_t)s_ret;
	if ((m_src = mmap(NULL, m_sz, PROT_READ | PROT_WRITE, MAP_PRIVATE, src, 0)) == MAP_FAILED)
		error_fd(src, "can't map source file into memory");
	close(src);
	e_hdr = (Elf64_Ehdr *)m_src;

	code_sz = &sc_end - &sc;
	data_sz = &sc_data_end - &sc_data;
	load_sz = data_sz + code_sz;

	for (i = 0, p_hdr = (Elf64_Phdr *)(m_src + e_hdr->e_phoff);
			!p_txt && i < e_hdr->e_phnum; ++i, ++p_hdr) {
		if (p_hdr->p_type == PT_LOAD && (p_hdr->p_flags & PF_X))
			p_txt = p_hdr;
	}

	if (!p_txt || i == e_hdr->e_phnum)
		error_unmap(m_src, m_sz, "can't find needed segments");
	p_nxt = p_hdr;

	m_space = p_nxt->p_vaddr - (p_txt->p_vaddr + p_txt->p_memsz);
	f_space = p_nxt->p_offset - (p_txt->p_offset + p_txt->p_filesz);

	if (m_space < load_sz)
		error_unmap(m_src, m_sz, "no space found for injection");

	if (f_space < load_sz)
		error_unmap(m_src, m_sz, "need to add padding");

	syscall(10, (uint64_t)&sc_data & PAGE_MSK, round_up(data_sz, PAGE_SZ),
			PROT_READ | PROT_WRITE | PROT_EXEC);
	sc_entry = p_txt->p_vaddr + p_txt->p_memsz;
	sc_old_entry = e_hdr->e_entry;
	e_hdr->e_entry = sc_entry;

	p_txt->p_filesz += load_sz;
	p_txt->p_memsz += load_sz;

	if ((dst = open("woody", O_WRONLY | O_CREAT | O_TRUNC, 0777)) < 0)
		error_unmap(m_src, m_sz, "can't open destination file");	
	if ((w_ret = write(dst, m_src, e_hdr->e_entry)) < 0
			|| (uint64_t)w_ret != e_hdr->e_entry
			|| (w_ret = write(dst, &sc, load_sz)) < 0
			|| (uint64_t)w_ret != load_sz)
		error_fd_unmap(dst, m_src, m_sz, "can't write to destination file");
	if (write_pad(dst, f_space - load_sz))
		error_fd_unmap(dst, m_src, m_sz, "can't write padding to destination file");
	if ((w_ret = write(dst, m_src + e_hdr->e_entry + f_space, m_sz - (e_hdr->e_entry + f_space))) < 0
			|| (uint64_t)w_ret != m_sz - (e_hdr->e_entry + f_space))
		error_fd_unmap(dst, m_src, m_sz, "can't write to destination file");

	close(dst);
	munmap(m_src, m_sz);
	return (0);
}
