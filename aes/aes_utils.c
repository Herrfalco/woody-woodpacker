/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aes_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/05 15:41:37 by herrfalco         #+#    #+#             */
/*   Updated: 2022/06/15 15:46:51 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"
#include "aes.h"
#include "aes_utils.h"
#include "aes_tables.h"

uint64_t		round_up(uint64_t val, uint64_t mod) {
	return (val % mod ? (val / mod) * mod + mod : val);
}

uint8_t			get_rnb(void) {
	switch (KEY_SIZE) {
		case 128: 
			return (11);
		case 192:
			return (13);
		default:
			return (15);
	}
}
