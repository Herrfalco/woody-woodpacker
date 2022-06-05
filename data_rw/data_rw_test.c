/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_rw_test.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 22:10:26 by herrfalco         #+#    #+#             */
/*   Updated: 2022/06/05 16:12:50 by herrfalco        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"
#include "data_rw.h"

static int		test(uint16_t *vals, size_t vals_nb, size_t vals_sz, char *name) {
	static uint16_t		mask = 0;
	uint16_t			value;
	size_t				i;
	int					fd = open("test_file", O_WRONLY | O_CREAT | O_TRUNC, 0666);

	if (fd < 0)
		return (fprintf(stderr, "Error: Can't open test_file\n"));
	if (!mask)
		mask = ((uint16_t)~mask) >> (16 - vals_sz);
	for (i = 0; i < vals_nb; ++i)
		value_writer(fd, vals[i], vals_sz, i == vals_nb - 1);
	close(fd);
	if ((fd = open("test_file", O_RDONLY)) < 0)
		return (fprintf(stderr, "Error: Can't open test_file\n"));
	printf("byte_writer %s test: ", name);
	for (i = 0; value_reader(fd, &value, vals_sz) > 0; ++i) {
		if (value != (vals[i] & mask)) {
			printf("KO\n");
			close(fd);
			return (0);
		}
	}
	printf(i != vals_nb ? "KO\n" : "OK\n");
	close(fd);
	return (0);
}

int		main(void) {
	uint16_t	vals_9[] = {	0x1ce, 0x05a, 0x1a5, 0x137, 0x038,
								0x1c7, 0x0cc, 0x133, 0x1c3, 0x1ff,
								0x001, 0x111 };
	uint16_t	vals_10[] = {	0x3ce, 0x25a, 0x1a5, 0x137, 0x038,
								0x1c7, 0x3cc, 0x333, 0x3c3 };
	uint16_t	vals_11[] = {	0x7ce, 0x65a, 0x5a5, 0x437, 0x338,
								0x2c7, 0x1cc, 0x033, 0x3c3, 0x73c, 0x142 };
	uint16_t	vals_12[] = {	0x8ce, 0xa5a, 0x5a5, 0x137, 0xe38,
								0x1c7, 0xccc, 0x333, 0x3c3, 0xc3c };
	uint16_t	vals_rand[1000000];
	int			fd, read_ret;

	test(vals_9, 12, 9, "9 bits");
	test(vals_10, 9, 10, "10 bits");
	test(vals_11, 11, 11, "11 bits");
	test(vals_12, 10, 12, "12 bits");
	if ((fd = open("/dev/urandom", O_RDONLY)) < 0
		|| (read_ret = read(fd, &vals_rand, 2000000)) != 2000000) {
		fprintf(stderr, "Error: Can't generate random values\n");
		return (1);
	}
	test(vals_rand, 1000000, 12, "12 bits random");
	return (0);
}
