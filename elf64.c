/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf64.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 22:56:40 by herrfalco         #+#    #+#             */
/*   Updated: 2022/07/20 18:06:22 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.h"

static void		map_file(char *path, uint8_t **mem, t_sizes *sz) {
	int			src;
	int64_t		s_ret;

	if ((src = open(path, O_RDONLY)) < 0)
		error("can't open source file");	
	if ((s_ret = get_fd_size(src)) < 0)
		error_fd(src, "can't get source file size");
	sz->mem = (uint64_t)s_ret;
	if ((*mem = mmap(NULL, sz->mem, PROT_READ | PROT_WRITE, MAP_PRIVATE, src, 0)) == MAP_FAILED)
		error_fd(src, "can't map source file into memory");
	close(src);
}

static void		enc_txt_sec(uint8_t *mem, t_hdrs *hdrs, t_sizes *sz, uint8_t *key) {
	Elf64_Shdr	*s_stab, *s_hdr;
	uint64_t	i;

	if (hdrs->elf->e_shstrndx == SHN_UNDEF || hdrs->elf->e_shstrndx == SHN_XINDEX)
		error_unmap(mem, sz->mem, "can't find .text section");
	s_stab = (Elf64_Shdr *)(mem + hdrs->elf->e_shoff) + hdrs->elf->e_shstrndx;
	for (i = 0, s_hdr = (Elf64_Shdr *)(mem + hdrs->elf->e_shoff);
			!hdrs->s_txt && i < hdrs->elf->e_shnum; ++i, ++s_hdr)
		if (!str_cmp(mem + s_stab->sh_offset + s_hdr->sh_name, (uint8_t *)".text"))
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
	sz->pad = hdrs->p_nxt->p_offset - (hdrs->p_txt->p_offset + hdrs->p_txt->p_filesz);
}

static void		update_mem(t_hdrs *hdrs, t_sizes *sz, uint8_t *key) {
	uint64_t		i;

	syscall(10, (uint64_t)&sc_data & PAGE_MSK, round_up(sz->data, PAGE_SZ),
			PROT_READ | PROT_WRITE | PROT_EXEC);
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

static void		write_mem(uint8_t *mem, t_hdrs *hdrs, t_sizes *sz) {
	int				dst;
	int64_t			w_ret;

	if ((dst = open("woody", O_WRONLY | O_CREAT | O_TRUNC, 0777)) < 0)
		error_unmap(mem, sz->mem, "can't open destination file");	
	if ((w_ret = write(dst, mem, hdrs->elf->e_entry)) < 0
			|| (uint64_t)w_ret != hdrs->elf->e_entry
			|| (w_ret = write(dst, &sc, sz->load)) < 0
			|| (uint64_t)w_ret != sz->load)
		error_fd_unmap(dst, mem, sz->mem, "can't write to destination file");
	if (write_pad(dst, sz->pad - sz->load))
		error_fd_unmap(dst, mem, sz->mem, "can't write padding to destination file");
	if ((w_ret = write(dst, mem + hdrs->elf->e_entry + sz->pad,
					sz->mem - (hdrs->elf->e_entry + sz->pad))) < 0
			|| (uint64_t)w_ret != sz->mem - (hdrs->elf->e_entry + sz->pad))
		error_fd_unmap(dst, mem, sz->mem, "can't write to destination file");
	close(dst);
}

int		main(int argc, char **argv) {
	uint8_t		*mem = NULL;
	t_hdrs		hdrs = { 0 };
	t_sizes		sz = { 0 };
	uint8_t		key[] = "0123456789ABCDEF";

	if (argc != 2)
		error("need 1 argument");	
	map_file(argv[1], &mem, &sz);
	hdrs.elf = (Elf64_Ehdr *)mem;
	enc_txt_sec(mem, &hdrs, &sz, key);
	find_txt_seg(mem, &hdrs, &sz);
	get_sizes(&hdrs, &sz);
	if (sz.pad < sz.load)
		error_unmap(mem, sz.mem, "no space found for injection");
	update_mem(&hdrs, &sz, key);
	write_mem(mem, &hdrs, &sz);
	munmap(mem, sz.mem);
	return (0);
}
