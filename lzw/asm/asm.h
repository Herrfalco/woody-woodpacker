/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 23:21:39 by fcadet            #+#    #+#             */
/*   Updated: 2022/06/28 15:58:33 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AES_ASM_H
# define AES_ASM_H

void		new_entry_asm(uint16_t last_byte, uint16_t byte, t_dico *dico);
uint16_t	find_first_pattern_asm(uint16_t value, t_dico *dico);
int64_t		entry_writer_asm(int64_t fd, uint16_t value, t_dico *dico, rw_buff_t *buff);
void		unlzw_asm(int64_t dst, int64_t src);
uint64_t	unlzw_chunk_asm(int64_t dst, int64_t src, rw_buff_t *r_buff, rw_buff_t *w_buff);
int64_t		value_reader_asm(int64_t fd, uint16_t *value, uint64_t size, rw_buff_t *buff);
int64_t		file_writer_asm(int64_t fd, uint8_t byte, flush_t flush, rw_buff_t *buff);
int64_t		file_reader(int64_t fd, uint8_t *byte, rw_buff_t *buff);

#endif
