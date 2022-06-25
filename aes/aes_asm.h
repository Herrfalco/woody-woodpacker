/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aes_asm.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 16:06:58 by fcadet            #+#    #+#             */
/*   Updated: 2022/06/25 12:33:00 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AES_ASM_H
#define AES_ASM_H

#include "../includes.h"

extern void			decode_block_asm(uint8_t *block, uint32_t *rkeys, uint64_t rnb);
extern void			sub_bytes_asm(uint8_t *block, uint64_t type);
extern void			sub_word_asm(uint8_t *block);
extern void			shift_rows_asm(uint8_t *block, uint64_t type);
extern void			shift_word_asm(uint32_t *word, uint64_t shift);
extern void			add_rkeys_asm(uint8_t *block, uint32_t *rkeys, uint64_t round);
extern void			mix_columns_asm(uint8_t *block, uint64_t type);
extern void			aes_data_dec_asm(uint8_t *data, uint64_t size, uint32_t *r_keys, uint64_t rnb);
extern uint64_t		get_rnb_asm(uint64_t key_sz);
extern void			round_keys_asm(uint8_t *key, uint32_t *rkeys);
extern void			aes_fd_dec_asm(uint64_t fd_dst, uint64_t fd_src, uint8_t *key);

#endif	// AES_ASM_H
