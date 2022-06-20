/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aes_asm.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 16:06:58 by fcadet            #+#    #+#             */
/*   Updated: 2022/06/17 15:24:43 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AES_ASM_H
#define AES_ASM_H

extern void			decode_block_asm(uint8_t *block, uint32_t *rkeys, uint8_t rnb);
void				sub_bytes_asm(uint8_t *block, method_t type);
void				shift_rows_asm(uint8_t *block, method_t type);
void				add_rkeys_asm(uint8_t *block, uint32_t *rkeys, uint8_t round);
void				mix_columns_asm(uint8_t *block, method_t type);
void				aes_data_dec_asm(uint8_t *data, uint64_t size, uint32_t *r_keys, uint8_t rnb);

#endif	// AES_ASM_H
