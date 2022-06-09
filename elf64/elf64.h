/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf64.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 18:26:49 by herrfalco         #+#    #+#             */
/*   Updated: 2022/06/08 11:37:53 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ELF64_H
#define ELF64_H

#include "../includes.h"

#define MAGIC			"\x7f" "ELF"
#define MAGIC_SZ		4
#define VERSION			1
#define BASE_ADDR		0x400000
#define PAGE_SZ			0x1000

typedef enum		bit_class_e {
	OS_32 = 1,
	OS_64 = 2,		//		Targetted bit class
}					bit_class_t;

typedef enum		data_enc_e {
	LSB = 1,		//		Current data encoding
	MSB = 2,
}					data_end_t;

typedef enum		os_e {
	SYS_V,			//		Almost everytime
	HP_UX,
	NET_BSD,
	LINUX,
}					os_t;

typedef enum		eh_type_e {
	REL	= 1,		
	EXEC = 2,		//		Needed for an executable without ALSR (no PIE)
	DYN = 3,
	CORE = 4,
}					eh_type_t;

typedef enum		machine_e {
	X86 = 3,
	MIPS = 8,
	ARM = 40,
	AMD64 = 62,		//		Tragetted architechture
	ARMV8 = 183,
	RISC_V = 243,
}					machine_t;

typedef enum		sh_type_e {
	NUL,
	LOAD,			//		Segments of this type are loaded into memory
	DYNAMIC,
	INTERP,
	NOTE,
	SHLIB,
	PHDR,
	TLS,
}					sh_type_t;

typedef enum		perm_e {
	X = 1,
	W = 2,
	R = 4,
}					perm_t;

struct				seg_hdr_s {
	uint32_t		type;				// NUL	| >LOAD<	| DYNAMIC	| INTERP
										// NOTE	| SHLIB		| PHDR		| TLS
	uint32_t		flags;				// X	| W		| R
	uint64_t		offset;				// Content of the segment offset
	uint64_t		vaddr;				// Address of the segment in memory
	uint64_t		paddr;				// Used only if physical memory (0 ?)
	uint64_t		filesz;				// Size of segment in the file
	uint64_t		memsz;				// Size of segment in memory
	uint64_t		align;				// Segment alignment (in memory ?)
}					__attribute__((packed));

typedef struct seg_hdr_s		seg_hdr_t;

struct				ident_s {
	uint8_t			magic[4];			// MAGIC
	uint8_t			bit_class;			// OS_32	| >OS_64<
	uint8_t			data_enc;			// >LSB<	| MSB
	uint8_t			version;			// VERSION
	uint8_t			os;					// >SYS_V<	| HP_UP		| NET_BSD	| LINUX
	uint8_t			abi;				// ABI
	uint8_t			pad[7];				// zeros
}					__attribute__((packed));

typedef struct elf64_hdr_s		ident_t;

struct				elf64_hdr_s {
	struct ident_s	ident;
	uint16_t		type;				// REL	| >EXEC<	| DYN	| CORE
	uint16_t		machine;			// X86	| MIPS		| ARM	| >AMD64<	| ARMV8	| RISC_V
	uint32_t		version;			// VERSION
	uint64_t		entry;				// Entrypoint address
	uint64_t		seg_hoff;			// Segment header table offset
	uint64_t		sec_hoff;			// Section header table offset
	uint32_t		flags;				// FLAGS
	uint16_t		ehsize;				// sizeof(elf64_hdr_t)
	uint16_t		seg_hentsize;		// sizeof(seg_hdr_t)
	uint16_t		seg_hnum;			// nb of entry in segment header table
	uint16_t		sec_hentsize;		// size of each entry in section header table
	uint16_t		sec_hnum;			// nb of entry in section header table
	uint16_t		shstrndx;			// Section header string table index
}					__attribute__((packed));

typedef struct elf64_hdr_s		elf64_hdr_t;

#endif // ELF64_H
