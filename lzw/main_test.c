/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 14:19:09 by fcadet            #+#    #+#             */
/*   Updated: 2022/06/25 15:18:58 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"
#include "../utils/utils_asm.h"
#include "../utils/test_utils.h"
#include "lzw.h"

static int		file(char *file) {
	int			v_crypt, v_decrypt, source;
	char		buff[1024];

	if ((v_crypt = syscall(319, "v_file", 0)) < 0)
		quit_asm("can't open virtual file");
	if ((v_decrypt = syscall(319, "v_file", 0)) < 0)
		quit_fd_asm(v_crypt, "can't open virtual file");
	sprintf(buff, "/usr/bin/%s", file);
	if ((source = open(buff, O_RDONLY)) < 0)
		quit_3_fd_asm(v_crypt, v_decrypt, source, "can't open source file");

	lzw(v_crypt, source);

	if (seek_2_fd_asm(v_decrypt, source))
		quit_3_fd_asm(v_crypt, v_decrypt, source, "can't seek into files");
	if (!diff_v_files(v_crypt, source))
		quit_3_fd_asm(v_crypt, v_decrypt, source, "file is not compressed");

	if (get_fd_size_asm(v_crypt) > get_fd_size_asm(source))
		quit_3_fd_asm(v_crypt, v_decrypt, source, "compression is very bad");
	unlzw(v_decrypt, v_crypt);

	if (seek_2_fd_asm(v_decrypt, source))
		quit_3_fd_asm(v_crypt, v_decrypt, source, "can't seek into files");

	printf("LZW on %s: ", file);
	printf("%s\n", diff_v_files(v_decrypt, source) ? "KO" : "OK");

	return (close_ret(v_crypt, v_decrypt, source, 0));
}

int				main(void) {
	int			in, crypt, out, diff;
	char		*files[5] = { "zip", "top", "touch", "apt-get", "ssh" };
	uint64_t	i;

	for (int i = 0; i < 5; ++i)
		file(files[i]);
	printf("------------------------------------\n");
	for (i = 0; i < 10000000; i += i * 2 + 1) {
		if (rand_v_file(&in, i) < 0
				|| (crypt = syscall(319, "v_file", 0)) < 0
				|| (out = syscall(319, "v_file", 0)) < 0)
			quit_asm("can't open virtual file");
		lzw(crypt, in);
		if (seek_fd_asm(crypt))
			quit_3_fd_asm(in, out, crypt, "can't seek into files");
		unlzw(out, crypt);
		if (seek_2_fd_asm(in, out))
			quit_3_fd_asm(in, out, crypt, "can't seek into files");
		printf("LZW random file (%ld bytes): ", i);
		if ((diff = diff_v_files(in, out))) {
			if (diff < 0)
				quit_asm("can't read files");
			printf("KO\n");
			return (close_ret(in, crypt, out, 0));
		}
		printf("OK\n");
		close_ret(in, crypt, out, 0);
	}
	return (close_ret(in, crypt, out, 0));
}
