/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aes_test.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <fcadet@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 13:08:18 by herrfalco         #+#    #+#             */
/*   Updated: 2022/06/23 16:11:03 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../utils/utils.h"
#include "../../utils/utils_asm.h"
#include "../../utils/test_utils.h"
#include "../aes.h"

#define KEY		"ckH1eMrBA0as6qabw352mxmP0Bxw76NB"

/*
void	encrypt_files(char *file) {
	int			crypt, source;
	uint8_t		key[] = KEY;
	char		buff[1024];

	sprintf(buff, "files/%s", file);
	if ((crypt = open(buff, O_WRONLY | O_TRUNC | O_CREAT, 0666)) < 0)
		quit("can't open destination file");
	sprintf(buff, "/usr/bin/%s", file);
	if ((source = open(buff, O_RDONLY)) < 0)
		quit_fd(crypt, "can't open source file");

	aes_fd(crypt, source, key, ENCODE);
	close_ret(crypt, source, -1, 0);
}

int		main(void) {
	char		*files[5] = { "zip", "top", "touch", "apt-get", "ssh" };

	for (int i = 0; i < 5; ++i)
		encrypt_files(files[i]);
	return (0);
}
*/

static int		file(char *file) {
	int			v_crypt, v_decrypt, crypt, source;
	uint8_t		key[] = KEY;
	char		buff[1024];

	if ((v_crypt = syscall(319, "v_file", 0)) < 0)
		quit_asm("can't open virtual file");
	if ((v_decrypt = syscall(319, "v_file", 0)) < 0)
		quit_fd_asm(v_crypt, "can't open virtual file");
	sprintf(buff, "files/%s", file);
	if ((crypt = open(buff, O_RDONLY)) < 0)
		quit_2_fd_asm(v_crypt, v_decrypt, "can't open crypted file");
	sprintf(buff, "/usr/bin/%s", file);
	if ((source = open(buff, O_RDONLY)) < 0) {
		close_ret(v_crypt, v_decrypt, crypt, 0);
		quit_asm("can't open decrypted file");
	}

	aes_fd_enc(v_crypt, source, key);
	aes_fd_dec(v_decrypt, crypt, key);

	if (seek_asm(v_crypt, v_decrypt) || seek_asm(crypt, source)) {
		close_ret(v_crypt, v_decrypt, crypt, 0);
		quit_fd_asm(source, "can't seek into files");
	}

	printf("AES %s encoding: ", file);
	printf("%s\n", diff_v_files(v_crypt, crypt) ? "KO" : "OK");
	printf("AES %s decoding: ", file);
	printf("%s\n", diff_v_files(v_decrypt, source) ? "KO" : "OK");

	close(source);
	return (close_ret(v_crypt, v_decrypt, crypt, 0));
}

int		main(void) {
	int			in, crypt, out, diff;
	char		*files[5] = { "zip", "top", "touch", "apt-get", "ssh" };
	uint8_t		key[KEY_SIZE];
	uint64_t	i;

	for (int i = 0; i < 5; ++i)
		file(files[i]);
	for (i = 0; i < 10000000; i += i * 2 + 1) {
		if (rand_key(key, KEY_SIZE) < 0)
			quit_asm("can't generate random key");
		if (rand_v_file(&in, i) < 0
				|| (crypt = syscall(319, "v_file", 0)) < 0
				|| (out = syscall(319, "v_file", 0)) < 0)
			quit_asm("can't open virtual file");
		aes_fd_enc(crypt, in, key);
		if (seek_asm(crypt, 0)) {
			close_ret(in, out, crypt, 0);	
			quit_asm("can't seek into files");
		}
		aes_fd_dec(out, crypt, key);
		if (seek_asm(in, out)) {
			close_ret(in, out, crypt, 0);	
			quit_asm("can't seek into files");
		}
		printf("AES random file (%ld bytes): ", i);
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
