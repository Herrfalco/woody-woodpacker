/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aes_asm.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 16:06:58 by fcadet            #+#    #+#             */
/*   Updated: 2022/06/20 17:54:17 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AES_ASM_H
#define AES_ASM_H

extern void			decode_block_asm(uint8_t *block, uint32_t *rkeys, uint8_t rnb);
extern void			sub_bytes_asm(uint8_t *block, method_t type);
extern void			sub_word_asm(uint8_t *block);
extern void			shift_rows_asm(uint8_t *block, method_t type);
extern void			shift_word_asm(uint32_t *word, uint8_t shift);
extern void			add_rkeys_asm(uint8_t *block, uint32_t *rkeys, uint8_t round);
extern void			mix_columns_asm(uint8_t *block, method_t type);
extern void			aes_data_dec_asm(uint8_t *data, uint64_t size, uint32_t *r_keys, uint8_t rnb);
extern uint8_t		get_rnb_asm(uint16_t key_sz);

#endif	// AES_ASM_H
