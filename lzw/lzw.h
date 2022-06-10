#ifndef COMPR_H
# define COMPR_H

# define DICO_SIZE 4095
# define BUFF_SIZE 1024

typedef struct	s_dico {
	size_t		size;
	uint16_t	entry[DICO_SIZE + 1][2];
}				t_dico;

void		new_entry(uint16_t last_byte, uint16_t byte, t_dico *dico);
int			check_dico(uint16_t last_value, uint16_t value, t_dico *dico);
uint16_t	entry_writer(int fd, uint16_t value, t_dico *dico);
void		not_in_dico(uint16_t last_value, t_dico *dico);

int			lzw(int fd);
void		unlzw(int fd);

#endif
