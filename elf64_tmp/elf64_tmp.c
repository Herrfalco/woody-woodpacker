#include "elf64.h"

void	str_cpy(uint8_t *dst, uint8_t	*src) {
	for (int i = 0; i < 4; ++i)
		dst[i] = src[i];
}

int	main(void) {
	uint8_t		magic[] = ".ELF";
	t_elf64_hdr	file_hdr = { 0 };
	t_prog_hdr	prog_hdr = { 0 };

	str_cpy(&file_hdr.ident.magic[4], magic);
	file_hdr.ident.bit_class = 2;
	file_hdr.ident.data_enc = 1;
	file_hdr.ident.version = 1;
	file_hdr.ident.os_abi = 0;
	file_hdr.ident.abi_version = 0;
	file_hdr.e_type = 2;
	file_hdr.e_machine = 62;
	file_hdr.e_version = 1;
	file_hdr.e_phoff = sizeof(t_elf64_hdr);
	file_hdr.e_ehsize = sizeof(t_elf64_hdr);
	file_hdr.e_phentsize = 0x1000;
	file_hdr.e_phnum = 1;

	prog_hdr.type = 1;
	prog_hdr.offset = sizeof(t_elf64_hdr) + sizeof(t_prog_hdr);
	prog_hdr.vaddr = 0xffffffffffffffff - 0x1000;
	prog_hdr.filesz = 0x1000;
	prog_hdr.memsz = 0x1000;
	prog_hdr.flags = 0;
	prog_hdr.align = 2;
}
