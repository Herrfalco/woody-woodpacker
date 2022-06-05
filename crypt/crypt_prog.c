/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   crypt_prog.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 23:37:21 by herrfalco         #+#    #+#             */
/*   Updated: 2022/06/05 16:10:35 by herrfalco        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"
#include "crypt.h"

static int			close_and_ret(int fd1, int fd2, char *error) {
	if (fd1 > 0)
		close(fd1);
	if (fd2 > 0)
		close(fd2);
	if (error) {
		fprintf(stderr, "Error: ");
		fprintf(stderr, "%s", error);
		fprintf(stderr, "\n");
		return (-1);
	}
	return (0);
}

static int			encode_file(char *dst, char *src, uint8_t *key) {
	uint32_t	r_keys[RKEY_ARR_SIZE] = { 0 };
	int			fd_dst, fd_src;	
	uint8_t		buff[BUFF_SIZE];
	ssize_t		read_ret, write_ret;
	size_t		i;
	uint64_t	src_sz, data_sz;

	round_keys(key, r_keys);
	if ((fd_src = open(src, O_RDONLY)) < 0)
		return (close_and_ret(-1, -1, "can't open source file"));
	if ((fd_dst = open(dst, O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0)
		return (close_and_ret(fd_src, -1, "can't open destination file"));
	for (src_sz = 0, data_sz = 0; (read_ret = read(fd_src, buff, BUFF_SIZE)) > 0;) {
		src_sz += read_ret;
		if (read_ret < BUFF_SIZE)
			for (i = read_ret; i < BUFF_SIZE; ++i)
				buff[i] = 0;
		data_sz = read_ret % 16 ? (read_ret / 16) * 16 + 16 : read_ret;
		convert_data(buff, data_sz, r_keys, ENCODE);
		if ((write_ret = write(fd_dst, buff, data_sz)) < 0 || (size_t)write_ret < data_sz)
			return (close_and_ret(fd_src, fd_dst, "can't write to destination file"));
	}
	if (read_ret < 0 || (write_ret = write(fd_dst, &src_sz, 8)) < 8)
		return (close_and_ret(fd_src, fd_dst, "can't access destination or source file"));
	return (close_and_ret(fd_src, fd_dst, NULL));
}

//file need to be correctly formated
static int			decode_file(char *dst, char *src, uint8_t *key) {
	uint32_t	r_keys[RKEY_ARR_SIZE] = { 0 };
	int			fd_dst, fd_src;	
	uint8_t		buff[BUFF_SIZE];
	ssize_t		read_ret, write_ret;
	uint64_t	src_sz, data_sz, write_sz;

	round_keys(key, r_keys);
	if ((fd_src = open(src, O_RDONLY)) < 0)
		return (close_and_ret(-1, -1, "can't open source file"));
	data_sz = lseek(fd_src, -8, SEEK_END);
	if ((read_ret = read(fd_src, &src_sz, 8)) < 8)
		return (close_and_ret(fd_src, -1, "can't read source file"));
	lseek(fd_src, 0, SEEK_SET);
	if ((fd_dst = open(dst, O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0)
		return (close_and_ret(fd_src, -1, "can't open destination file"));
	for (; (read_ret = read(fd_src, buff, BUFF_SIZE)) > 0;
		data_sz -= read_ret, src_sz -= read_ret) {
		convert_data(buff, data_sz > (size_t)read_ret ? read_ret : data_sz, r_keys, DECODE);
		write_sz = src_sz > (size_t)read_ret ? read_ret : src_sz;
		if ((write_ret = write(fd_dst, buff, write_sz)) < 0 || (size_t)write_ret < write_sz)
			return (close_and_ret(fd_src, fd_dst, "can't write to destination file"));
	}
	if (read_ret < 0)
		return (close_and_ret(fd_src, fd_dst, "can't read source file"));
	return (close_and_ret(fd_src, fd_dst, NULL));
}

static int			str_cmp(char *s1, char *s2) {
	for (; *s2 && *s1 == *s2; ++s1, ++s2);
	return (*s2);
}

static int			get_key(char *arg, uint8_t *key) {
	size_t		size = KEY_SIZE / 8;

	for (; *arg != '='; ++arg);
	++arg;
	for (; *arg && size; ++key, ++arg, --size)
		*key = *arg;
	return (size || *arg ? -1 : 0);
}

int			main(int argc, char **argv) {
	uint8_t		key[KEY_SIZE / 8 + 1] = { 0 };
	char		help[] = "[-dec=KEY/-enc] SRC DST";

	if (argc != 4) {
		printf("%s %s\n", argv[0], help);
		return (0);
	}
	if (!str_cmp(argv[1], "-enc")) {
		if (rand_key(key, KEY_SIZE / 8) < 0) {
			fprintf(stderr, "Error: can't generate random key\n");
			return (1);
		}
		if (encode_file(argv[3], argv[2], key))
			return (1);
		printf("Key: %s\n", key);
	} else if (!str_cmp(argv[1], "-dec=")) {
		if (get_key(argv[1], key)) {
			fprintf(stderr, "Error: misformatted key\n");
			return (1);
		}
		if (decode_file(argv[3], argv[2], key))
			return (1);
	} else
		printf("%s %s\n", argv[0], help);
	return (0);
}
