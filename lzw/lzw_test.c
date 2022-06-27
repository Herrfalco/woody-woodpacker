/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lzw_test.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 14:19:09 by fcadet            #+#    #+#             */
/*   Updated: 2022/06/27 13:40:48 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"
#include "../utils/utils_asm.h"
#include "../utils/test_utils.h"
#include "lzw.h"

static void		file(char *file) {
	int64_t		a_size, b_size;
	int			src, dst;
	char		buff[1024];

	if ((dst = syscall(319, "v_file", 0)) < 0)
		quit_asm("can't open virtual file");
	sprintf(buff, "/usr/bin/%s", file);
	if ((src = open(buff, O_RDONLY)) < 0)
		quit_fd_asm(dst, "can't open source file");

	lzw(dst, src);
	if ((a_size = get_fd_size_asm(src)) < 0)
		quit_2_fd_asm(src, dst, "can't get source size");
	if ((b_size = get_fd_size_asm(dst)) < 0)
		quit_2_fd_asm(src, dst, "can't get destination size");
	if (!diff_v_files(src, dst))
		quit_2_fd_asm(src, dst, "file is not compressed");
	close(src);
	src = dst;
	if (seek_fd_asm(src) < 0)
		quit_fd_asm(src, "can't seek into source file");

	if ((dst = syscall(319, "v_file", 0)) < 0)
		quit_fd_asm(src, "can't open virtual file");
	unlzw(dst, src);
	close(src);

	if (seek_fd_asm(dst) < 0)
		quit_fd_asm(dst, "can't seek into destination file");
	if ((src = open(buff, O_RDONLY)) < 0)
		quit_fd_asm(dst, "can't open source file");

	printf("LZW on %s: ", file);
	sprintf(buff, "OK (%+ld%%)", b_size * 100 / a_size - 100);
	printf("%s\n", diff_v_files(src, dst) ? "KO" : buff);

	close(src);
	close(dst);
}

static void		v_file(uint64_t size) {
	int64_t		a_size, b_size;
	int			src, dst, tmp;
	char		buff[1024];

	if ((dst = syscall(319, "v_file", 0)) < 0)
		quit_asm("can't open virtual file");
	if (rand_v_file(&src, size) < 0)
		quit_fd_asm(dst, "can't open source file");

	lzw(dst, src);
	if ((a_size = get_fd_size_asm(src)) < 0)
		quit_2_fd_asm(src, dst, "can't get source size");
	if ((b_size = get_fd_size_asm(dst)) < 0)
		quit_2_fd_asm(src, dst, "can't get destination size");
	if (a_size && !diff_v_files(src, dst))
		quit_2_fd_asm(src, dst, "file is not compressed");
	tmp = src;
	src = dst;
	if (seek_fd_asm(src) < 0)
		quit_fd_asm(src, "can't seek into destination file");

	if ((dst = syscall(319, "v_file", 0)) < 0)
		quit_fd_asm(src, "can't open virtual file");
	unlzw(dst, src);
	close(src);

	src = tmp;
	if (seek_fd_asm(src) < 0)
		quit_fd_asm(src, "can't seek into destination file");

	if (size >= 1000000)
		printf("LZW on %.1lf MB: ", (double)size / 1000000.);
	else if (size >= 1000)
		printf("LZW on %.1lf KB: ", (double)size / 1000.);
	else
		printf("LZW on %ld bytes: ", size);
	sprintf(buff, "OK (%+ld%%)", size ? b_size * 100 / a_size - 100 : 0);
	printf("%s\n", diff_v_files(src, dst) ? "KO" : buff);

	close(src);
	close(dst);
}

int		main(void) {
	char		*files[10] = { "zip", "top", "touch", "apt-get", "ssh",
		"sort", "sed", "pwd", "ps", "less" };
	uint64_t	i;

	printf("------------------------------------\n");
	for (i = 0; i < 10; ++i)
		file(files[i]);
	printf("------------------------------------\n");
	for (i = 0; i < 5000000; i += i * 2 + 1)
		v_file(i);
	return (0);
}
