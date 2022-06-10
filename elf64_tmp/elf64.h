#ifndef ELF64_H
# define ELF64_H

# include <stdint.h>

# define EI_NIDENT	16

typedef struct	s_prog_hdr {
	uint32_t	type;			// NULL | [LOAD] | DYNAMIC | INTERP | NOTE | SHLIB | PHDR | TLS
	uint64_t	offset;			// Content offset
	uint64_t	vaddr;			// Segment address
	uint64_t	paddr;			// Used only if physical memory
	uint32_t	filesz;			// Segment's size
	uint32_t	memsz;			// Segment's memory size
	uint32_t	flags;			// X | W | R
	uint32_t	align;			// Segment alignment
}				t_prog_hdr;

typedef struct	s_ident {
	uint8_t		magic[4];		// 7f 45 4c 46 = '.' 'E' 'L' 'F'
	uint8_t		bit_class;		// NONE | 32bits | [64bits]
	uint8_t		data_enc;		// NONE | [LSB] | MSB 
	uint8_t		version;		// VERSION = 1
	uint8_t		os_abi;			// [SYSTEM-V] | HP-UX | NetBSD | Linux | ...
	uint8_t		abi_version;	// 0
	uint8_t		padding[7];		// 0
}				t_ident; 

typedef struct	s_elf64_hdr {
	struct s_ident		ident;
	uint16_t	e_type;			//	ET_NONE | ET_REL | [ET_EXEC] | ET_DYN | ET_CORE
	uint16_t	e_machine;		// 0x03 = x86 | 0x08 = MIPS | 0x28 = ARM | [0x3E = AMD64] | 0xB7 = ARMv8 | 0xF3 = RISC-V
	uint32_t	e_version;		// VERSION = 1
	uint64_t	e_entry;		// Entrypoint address or 0
	uint64_t	e_phoff;		// Program header offset
	uint64_t	e_shoff;		// Section header offset
	uint32_t	e_flags;		// FLAGS
	uint16_t	e_ehsize;		// Header's size in byte
	uint16_t	e_phentsize;	// Program header's size in byte
	uint16_t	e_phnum;		// Number of program headers
	uint16_t	e_shentsize;	// Sections header's size in byte
	uint16_t	e_shnum;		// Number of section headers
	uint16_t	e_shstrndx;		// Section header string table index
}				t_elf64_hdr;

#endif
