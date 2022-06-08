#ifndef COMPR_H
# define COMPR_H

# define DICO_SIZE 4095

typedef struct	s_dico {
	size_t		size;
	uint16_t	entry[DICO_SIZE + 1][2];
}				t_dico;

#endif
