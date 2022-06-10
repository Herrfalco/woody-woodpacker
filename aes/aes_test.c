/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aes_test.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <fcadet@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 13:08:18 by herrfalco         #+#    #+#             */
/*   Updated: 2022/06/10 15:37:25 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"
#include "../test_utils.h"
#include "aes.h"

int		main(void) {
	int			in, crypt, out, diff;
	uint8_t		key[KEY_SIZE];
	uint64_t	i;

	for (i = 0; i < 10000; i += 1133) {
		printf("aes %ld test: ", i);
		if (rand_key(key, KEY_SIZE) < 0)
			quit("can't generate random key");
		if (rand_v_file(&in, i) < 0
				|| (crypt = syscall(319, "v_file", 0)) < 0
				|| (out = syscall(319, "v_file", 0)) < 0)
			quit("can't open virtual file");
		aes_fd(crypt, in, key, ENCODE);
		aes_fd(out, crypt, key, DECODE);
		if ((diff = diff_v_files(in, out))) {
			if (diff < 0)
				quit("can't read files");
			printf("diff: %d\n", diff);
			printf("KO (size=%ld)\n", i);
			return (close_ret(in, crypt, out, 0));
		}
		printf("OK\n");
	}
	return (close_ret(in, crypt, out, 0));
}
