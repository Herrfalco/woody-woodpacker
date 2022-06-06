/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf64.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 18:26:49 by herrfalco         #+#    #+#             */
/*   Updated: 2022/06/06 18:46:02 by herrfalco        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ELF64_H
#define ELF64_H

struct				ident_s {
	uint8_t			magic[4];			//		.ELF
	uint8_t			bit_class;			//		1 = 32bit / 2 = 64bit
	uint8_t			data_enc;			//		
}					__attribute__((packed));

typedef struct elf64_hdr_s		indent_t;

struct				elf64_hdr_s {
	ident_t			ident[EI_NIDENT];
	uint16_t		type;
	uint16_t		machine;
	uint32_t		version;
	uint64_t		entry;
	uint64_t		phoff;
	uint64_t		shoff;
	uint32_t		flags;
	uint16_t		ehsize;
	uint16_t		phentsize;
	uint16_t		phnum;
	uint16_t		shentsize;
	uint16_t		shnum;
	uint16_t		shstrndx;
}					__attribute__((packed));

typedef struct elf64_hdr_s		elf64_hdr_t;

#endif // ELF64_H
