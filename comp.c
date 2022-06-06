#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <stdbool.h>

#define	DICO_SIZE 4096
#define	MAX_BYTE 256
#define MAX_BITS 12
#define	BUFFER_SIZE 4096
#define TRUE 1
#define FALSE 0

typedef struct	s_utils {
	uint32_t	dico[DICO_SIZE];
	uint16_t	dico_size;
	uint32_t	r_buffer;
	uint32_t	wr_buffer;
	uint16_t	in_r_buff;
	int			fd;
	int			lzw_fd;
}				t_utils;

void		exit_error(char *str, t_utils *utils) {
	printf("%s\n", str);
	if (utils->fd)
		close(utils->fd);
	if (utils->lzw_fd)
		close(utils->lzw_fd);
	exit(1);
}

uint8_t		read_file(t_utils *utils) {
	uint32_t	read_ret;
	uint8_t		byte;

	if ((read_ret = read(utils->fd, &byte, sizeof(uint8_t))) > 0)
		return (byte);
	else
		return (-1);
}

void		create_buff(uint8_t byte, t_utils *utils) {
	if (utils->in_r_buff == 2)
		utils->in_r_buff = 0;
	if (!utils->in_r_buff) {
		utils->r_buffer = byte << 24;
		utils->in_r_buff = 1;
	}
	else {
		utils->r_buffer |= byte << 8;
		utils->in_r_buff = 2;
	}
}

void		create_entry(t_utils *utils) {
	utils->dico[dico_size] = utils->r_buffer;
	utils->dico_size++;
}

void		check_dico(t_utils *utils) {
	if (!utils->dico_size)
		return (create_entry(utils));
	for (uint16_t i = 0; i < utils->dico_size; i++) {
		if (dico[i] == utils->r_buffer) {
			utils->wr_buffer = i << 4;
		}
	}

}

void		compression(t_utils *utils) {
	uint8_t		byte;
	bool		eof = FALSE;

	if ((byte = read_file(utils)) < 0)
		exit_error("Error: read error.", utils);
	utils->r_buffer = byte << 24;
	utils->in_r_buff = 1;
	while (!eof) {
		if (utils->in_r_buff == 2)
			utils->in_r_buff = 0;
		if ((byte = read_file(utils)) < 0)
			exit_error("Error: read error.", utils);
		create_buff(byte, utils);
		if (utils->in_r_buff == 2)
			check_dico(utils);
	}

}

int		main(int ac, char **av) {
	t_utils	utils;

	bzero(&utils, sizeof(t_utils));
	if (ac != 2)
		exit_error("USAGE: ./[exe] [file]", &utils);
	if ((utils.fd = open(av[1], O_RDONLY)) == -1)
		exit_error("Error: Couldn't open file.", &utils);
	if ((utils.lzw_fd = open(strcat(av[1], ".lzw"), O_WRONLY | O_APPEND | O_CREAT, 0644)) == -1) 
		exit_error("Error: Couldn't create new file.", &utils);
	compression(&utils);
	close(utils.fd);
	close(utils.lzw_fd);
	return (0);
}
