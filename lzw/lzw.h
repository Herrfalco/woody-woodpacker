#ifndef COMPR_H
# define COMPR_H

# define DICO_SIZE	3834
# define BUFF_SIZE	1024
# define RESET_CODE	256
# define STOP_CODE	257
# define INCR_CODE	258
# define DICO_START	259
# define MAX_BYTE	255

# include "../includes.h"

struct			s_dico {
	uint64_t	size;
	uint16_t	entry[DICO_SIZE + 1][2];
	uint8_t		bits;
}				__attribute__((packed));

typedef struct	s_dico		t_dico;

void		init_dico(t_dico *dico);
int			get_bits_nb(uint64_t dico_size);
void		new_entry(uint16_t last_byte, uint16_t byte, t_dico *dico);
int			check_dico(uint16_t last_value, uint16_t value, t_dico *dico);
uint16_t	entry_writer(int fd, uint16_t value, t_dico *dico);
void		not_in_dico(uint16_t last_value, t_dico *dico);

int			lzw(int fd);
void		unlzw(int fd);

#endif
