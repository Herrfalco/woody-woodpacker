/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   woody_woodpacker.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 22:56:40 by herrfalco         #+#    #+#             */
/*   Updated: 2022/07/27 22:19:51 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.h"

static void		rand_key_gen(uint8_t *key) {
	int			fd = open("/dev/urandom", O_RDONLY);
	size_t		i;

	if (fd < 0 || read(fd, key, KEY_SZ / 8) != KEY_SZ / 8)
		error_fd(fd, "can't generate random key");	
	for (i = 0; i < KEY_SZ / 8; ++i) {
		key[i] = key[i] % 62;
		if (key[i] < 10)
			key[i] += '0';
		else if (key[i] < 36)
			key[i] += 'A' - 10;
		else
			key[i] += 'a' - 36;
	}
	close(fd);
}

static void		map_file(char *path, uint8_t **mem, t_sizes *sz) {
	int			src;
	int64_t		s_ret;

	if ((src = open(path, O_RDONLY)) < 0)
		error("can't open source file");	
	if ((s_ret = get_fd_size(src)) < 0)
		error_fd(src, "can't get source file size");
	if ((sz->mem = (uint64_t)s_ret) < sizeof(Elf64_Ehdr))
		error_fd(src, "not an ELF64 binary file");
	if ((*mem = mmap(NULL, sz->mem,
					PROT_READ | PROT_WRITE, MAP_PRIVATE, src, 0)) == MAP_FAILED)
		error_fd(src, "can't map source file into memory");
	close(src);
}

static void		test_elf_hdr(uint8_t *mem, t_sizes *sz, t_hdrs *hdrs) {
	if (str_n_cmp((uint8_t *)hdrs->elf->e_ident, (uint8_t *)(IDENT), 5))
		error_unmap(mem, sz->mem, "not an ELF64 binary file");
	if (hdrs->elf->e_type != ET_EXEC && hdrs->elf->e_type != ET_DYN)
		error_unmap(mem, sz->mem, "unsupported ELF64 binary file type");
	if (hdrs->elf->e_machine != EM_X86_64)
		error_unmap(mem, sz->mem, "unsupported ELF64 binary file architechture");
	if (hdrs->elf->e_shstrndx == SHN_UNDEF || hdrs->elf->e_shstrndx == SHN_XINDEX)
		error_unmap(mem, sz->mem, "can't find section name string table");
}

static void		enc_txt_sec(uint8_t *mem, t_hdrs *hdrs, t_sizes *sz, uint8_t *key) {
	Elf64_Shdr	*s_stab, *s_hdr;
	uint64_t	i;

	s_stab = (Elf64_Shdr *)(mem + hdrs->elf->e_shoff) + hdrs->elf->e_shstrndx;
	for (i = 0, s_hdr = (Elf64_Shdr *)(mem + hdrs->elf->e_shoff);
			!hdrs->s_txt && i < hdrs->elf->e_shnum; ++i, ++s_hdr)
		if (!str_n_cmp(mem + s_stab->sh_offset + s_hdr->sh_name, (uint8_t *)".text", -1))
			hdrs->s_txt = s_hdr;
	if (!hdrs->s_txt)
		error_unmap(mem, sz->mem, "can't find .text section");
	for (i = hdrs->s_txt->sh_offset;
			i < hdrs->s_txt->sh_offset + (hdrs->s_txt->sh_size & BLOCK_MSK);
			i += 16)
		aes_encoding(mem + i, key);
}

static void		find_txt_seg(uint8_t *mem, t_hdrs *hdrs, t_sizes *sz) {
	Elf64_Phdr	*p_hdr;
	uint64_t	i;

	for (i = 0, p_hdr = (Elf64_Phdr *)(mem + hdrs->elf->e_phoff);
			!hdrs->p_txt && i < hdrs->elf->e_phnum; ++i, ++p_hdr)
		if (p_hdr->p_type == PT_LOAD && (p_hdr->p_flags & PF_X))
			hdrs->p_txt = p_hdr;
	if (!hdrs->p_txt || i == hdrs->elf->e_phnum)
		error_unmap(mem, sz->mem, "can't find needed segments");
	hdrs->p_nxt = p_hdr;
}

static void		get_sizes(t_hdrs *hdrs, t_sizes *sz) {
	sz->code = &sc_end - &sc;
	sz->data = &sc_data_end - &sc_data;
	sz->load = sz->data + sz->code;
	sz->f_pad = hdrs->p_nxt->p_offset - (hdrs->p_txt->p_offset + hdrs->p_txt->p_filesz);
	sz->m_pad = hdrs->p_nxt->p_vaddr - (hdrs->p_txt->p_vaddr + hdrs->p_txt->p_memsz);
}

static void		update_mem(uint8_t *mem, t_hdrs *hdrs, t_sizes *sz, uint8_t *key) {
	uint64_t		i;

	if (syscall(10, (uint64_t)&sc_data & PAGE_MSK, round_up(sz->data, PAGE_SZ),
			PROT_READ | PROT_WRITE | PROT_EXEC) < 0)
		error_unmap(mem, sz->mem, "can't update variables in mapped file");
	sc_entry = hdrs->p_txt->p_vaddr + hdrs->p_txt->p_memsz;
	sc_old_entry = hdrs->elf->e_entry;
	sc_text_addr = hdrs->s_txt->sh_addr;
	sc_text_sz = hdrs->s_txt->sh_size & BLOCK_MSK;
	for (i = 0; i < KEY_SZ / 8; ++i)
		(&sc_key)[i] = key[i];
	hdrs->elf->e_entry = sc_entry;
	hdrs->p_txt->p_filesz += sz->load;
	hdrs->p_txt->p_memsz += sz->load;
}

static void		write_mem(uint8_t *mem, t_hdrs *hdrs, t_sizes *sz, int x_pad) {
	int				dst;
	int64_t			w_ret;
	uint64_t		code_offset;

	if ((dst = open("woody", O_WRONLY | O_CREAT | O_TRUNC, 0777)) < 0)
		error_unmap(mem, sz->mem, "can't open destination file");	
	code_offset = hdrs->p_txt->p_offset + hdrs->p_txt->p_filesz - sz->load;
	if ((w_ret = write(dst, mem, code_offset)) < 0
			|| (uint64_t)w_ret != code_offset
			|| (w_ret = write(dst, &sc, sz->load)) < 0
			|| (uint64_t)w_ret != sz->load)
		error_fd_unmap(dst, mem, sz->mem, "can't write to destination file");
	if (write_pad(dst, sz->f_pad + x_pad * PAGE_SZ - sz->load))
		error_fd_unmap(dst, mem, sz->mem, "can't write f_padding to destination file");
	if ((w_ret = write(dst, mem + code_offset + sz->f_pad,
					sz->mem - (code_offset + sz->f_pad))) < 0
			|| (uint64_t)w_ret != sz->mem - (code_offset + sz->f_pad))
		error_fd_unmap(dst, mem, sz->mem, "can't write to destination file");
	close(dst);
}

static void		set_x_pad(uint8_t *mem, t_hdrs *hdrs, t_sizes *sz, int *x_pad) {
	Elf64_Phdr	*p_hdr;
	Elf64_Shdr	*s_hdr;
	uint64_t	i;

	if (sz->f_pad < sz->load) {
		if (sz->m_pad >= sz->load) {
			for (i = 0, p_hdr = (Elf64_Phdr *)(mem + hdrs->elf->e_phoff);
					i < hdrs->elf->e_phnum; ++i, ++p_hdr)
				if (p_hdr->p_offset >= hdrs->p_txt->p_offset + hdrs->p_txt->p_filesz)
					p_hdr->p_offset += PAGE_SZ;
			for (i = 0, s_hdr = (Elf64_Shdr *)(mem + hdrs->elf->e_shoff);
					i < hdrs->elf->e_shnum; ++i, ++s_hdr)
				if (s_hdr->sh_offset >= hdrs->p_txt->p_offset + hdrs->p_txt->p_filesz)
					s_hdr->sh_offset += PAGE_SZ;
			if (hdrs->elf->e_phoff >= hdrs->p_txt->p_offset + hdrs->p_txt->p_filesz) 
				hdrs->elf->e_phoff += PAGE_SZ;
			if (hdrs->elf->e_shoff >= hdrs->p_txt->p_offset + hdrs->p_txt->p_filesz) 
				hdrs->elf->e_shoff += PAGE_SZ;
			*x_pad = 1;
		} else
			error_unmap(mem, sz->mem, "no space found for injection");
	}
}

int		main(int argc, char **argv) {
	uint8_t		*mem = NULL;
	int			x_pad = 0;
	t_hdrs		hdrs = { 0 };
	t_sizes		sz = { 0 };
	uint8_t		key[KEY_SZ / 8 + 1] = { 0 };

	if (argc != 2)
		error("need 1 argument");	
	rand_key_gen(key);
	map_file(argv[1], &mem, &sz);
	hdrs.elf = (Elf64_Ehdr *)mem;
	test_elf_hdr(mem, &sz, &hdrs);
	enc_txt_sec(mem, &hdrs, &sz, key);
	find_txt_seg(mem, &hdrs, &sz);
	get_sizes(&hdrs, &sz);
	set_x_pad(mem, &hdrs, &sz, &x_pad);
	printf("key_value: %s\n", key);
	update_mem(mem, &hdrs, &sz, key);
	write_mem(mem, &hdrs, &sz, x_pad);
	munmap(mem, sz.mem);
	return (0);
}
