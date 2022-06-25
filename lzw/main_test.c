/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 14:19:09 by fcadet            #+#    #+#             */
/*   Updated: 2022/06/24 14:19:21 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"
#include "../utils/utils_asm.h"
#include "../utils/test_utils.h"
#include "lzw.h"

int				main(int ac, char **av) {
	int	fd = 0;
	int	new_fd = 0;

	if (ac != 2)
		quit_asm("1 arg needed to set fd size");
//	fd = open(av[1], O_RDONLY);
	rand_v_file(&fd, atoi(av[1]));
	new_fd = lzw(fd);
	printf("%ld\n", get_fd_size_asm(new_fd));
	unlzw(new_fd);
	if ((new_fd = open("uncompressed_file", O_RDONLY)) == -1)
		quit_asm("can't open uncompressed file");
	if (diff_v_files(fd, new_fd))
		quit_2_fd_asm(fd, new_fd, "files are differents");
	else
		quit_2_fd_asm(fd, new_fd, "files are equals");
	return (0);
}
