#ifndef AES_ASM_H
# define AES_ASM_H

void	new_entry_asm(uint16_t last_byte, uint16_t byte, t_dico *dico);
void	not_in_dico_asm(uint16_t last_value, t_dico *dico);

#endif
