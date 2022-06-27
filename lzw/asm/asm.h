#ifndef AES_ASM_H
# define AES_ASM_H

void		new_entry_asm(uint16_t last_byte, uint16_t byte, t_dico *dico);
uint16_t	find_first_pattern_asm(uint16_t value, t_dico *dico);
int64_t		entry_writer_asm(int64_t fd, uint16_t value, t_dico *dico, rw_buff_t *buff);
void		unlzw_asm(int64_t dst, int64_t src);

#endif
