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

typedef enum	flush_e {
	NO_FLUSH,
	FLUSH,
	ONLY_FLUSH,
}				flush_t;

struct			rw_buff_s {
	uint8_t		bytes[BUFF_SIZE];
	uint64_t	b_size;
	uint64_t	idx;
	uint32_t	dword;
	uint64_t	dw_size;
	uint8_t		error;
}				__attribute__((packed));

typedef struct	rw_buff_s		rw_buff_t;

struct			s_dico {
	uint64_t	size;
	uint16_t	entry[DICO_SIZE + 1][2];
	uint8_t		bits;
}				__attribute__((packed));

typedef struct	s_dico		t_dico;

int64_t		file_reader(int64_t fd, uint8_t *byte, rw_buff_t *buff);
int64_t		file_writer(int64_t fd, uint8_t byte, flush_t flush, rw_buff_t *buff);
int64_t			value_writer(int64_t fd, uint16_t value, uint64_t size, flush_t flush, rw_buff_t *buff);
int64_t			value_reader(int64_t fd, uint16_t *value, uint64_t size, rw_buff_t *buff);

void		init_dico(t_dico *dico);
int			get_bits_nb(uint64_t dico_size);
void		new_entry(uint16_t last_byte, uint16_t byte, t_dico *dico);
int			check_dico(uint16_t last_value, uint16_t value, t_dico *dico);
uint16_t	entry_writer(int fd, uint16_t value, t_dico *dico, rw_buff_t *buff);
void		not_in_dico(uint16_t last_value, t_dico *dico);

void		lzw(int dst, int src);
void		unlzw(int dst, int src);

#endif
