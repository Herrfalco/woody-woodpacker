#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>

#define MAX_SIZE 1000000
#define MAX_BYTE 256

typedef struct	s_dico {
	uint8_t	entry[MAX_SIZE][2];
	int		size;
}				t_dico;

void	new_entry(uint8_t first, uint8_t second, t_dico *dico) {
	dico->entry[dico->size][0] = first;
	dico->entry[dico->size][1] = second;
	dico->size++;
}

void	read_file(int fd, t_dico *dico) {
	int		val;
	uint8_t	byte, last_byte;
	int		read_ret, i, idx;

	i = 0;
	while ((read_ret = read(fd, &val, sizeof(char))) > 0) {
		printf("val: %d\n", val);
		if (dico->size == 0) {
			byte = (uint8_t)val;
			dico->entry[0][i] = byte;
			write(1, &byte, 1);
			i == 0 ? i++ : dico->size++;
			last_byte = byte;
		}
		else if (val >= MAX_BYTE) {
			write(1, &dico->entry[val - MAX_BYTE][0], 1);
			write(1, &dico->entry[val - MAX_BYTE][1], 1);
			new_entry(last_byte , (uint8_t)dico->entry[val - MAX_BYTE][0], dico);
			last_byte = (uint8_t)dico->entry[byte - MAX_BYTE][1];
		}
		else {
			byte = (uint8_t)val;
			write(1, &byte, 1);
			new_entry(last_byte, byte, dico);
			last_byte = byte;
		}
	}
}

int		main(int ac, char **av) {
	int 	fd;
	t_dico	dico;

	bzero(&dico, sizeof(t_dico));
	if (ac != 2) {
		printf("USAGE: ./[exe] [file]\n");
		exit(1);
	}
	if ((fd = open(av[1], O_RDONLY)) == -1) {
		printf("Error: couldn't open file.\n");
		exit(1);
	}
	read_file(fd, &dico);
	return (0);
}
