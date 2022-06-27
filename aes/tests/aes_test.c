/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aes_test.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <fcadet@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 13:08:18 by herrfalco         #+#    #+#             */
/*   Updated: 2022/06/27 13:45:07 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../aes.h"
#include "../aes_asm.h"
#include "../../utils/utils_asm.h"
#include "../../utils/test_utils.h"

static void		file(char *file) {
	int			src, dst;
	uint8_t		key[KEY_SIZE];
	char		buff[1024];

	if (rand_key(key, KEY_SIZE) < 0)
		quit_asm("can't generate random key");
	if ((dst = syscall(319, "v_file", 0)) < 0)
		quit_asm("can't open virtual file");
	sprintf(buff, "/usr/bin/%s", file);
	if ((src = open(buff, O_RDONLY)) < 0)
		quit_fd_asm(dst, "can't open source file");

	aes_fd_enc(dst, src, key);
	close(src);
	src = dst;
	if (seek_fd_asm(src) < 0)
		quit_fd_asm(src, "can't seek into source file");

	if ((dst = syscall(319, "v_file", 0)) < 0)
		quit_fd_asm(src, "can't open virtual file");
	aes_fd_dec_asm(dst, src, key);
	close(src);

	if (seek_fd_asm(dst) < 0)
		quit_fd_asm(dst, "can't seek into destination file");
	if ((src = open(buff, O_RDONLY)) < 0)
		quit_fd_asm(dst, "can't open source file");

	printf("AES on %s: ", file);
	printf("%s\n", diff_v_files(src, dst) ? "KO" : "OK");

	close(src);
	close(dst);
}

static void		v_file(uint64_t size) {
	int			src, dst, tmp;
	uint8_t		key[KEY_SIZE];

	if (rand_key(key, KEY_SIZE) < 0)
		quit_asm("can't generate random key");
	if ((dst = syscall(319, "v_file", 0)) < 0)
		quit_asm("can't open virtual file");
	if (rand_v_file(&src, size) < 0)
		quit_fd_asm(dst, "can't open source file");

	aes_fd_enc(dst, src, key);
	tmp = src;
	src = dst;
	if (seek_fd_asm(src) < 0)
		quit_fd_asm(src, "can't seek into destination file");

	if ((dst = syscall(319, "v_file", 0)) < 0)
		quit_fd_asm(src, "can't open virtual file");
	aes_fd_dec_asm(dst, src, key);
	close(src);

	src = tmp;
	if (seek_fd_asm(src) < 0)
		quit_fd_asm(src, "can't seek into destination file");

	if (size >= 1000000)
		printf("AES on %.1lf MB: ", (double)size / 1000000.);
	else if (size >= 1000)
		printf("AES on %.1lf KB: ", (double)size / 1000.);
	else
		printf("AES on %ld bytes: ", size);
	printf("%s\n", diff_v_files(src, dst) ? "KO" : "OK");

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
