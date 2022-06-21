/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aes_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/05 15:41:37 by herrfalco         #+#    #+#             */
/*   Updated: 2022/06/20 18:00:59 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"
#include "aes.h"
#include "aes_utils.h"

uint64_t		round_up(uint64_t val, uint64_t mod) {
	return (val % mod ? (val / mod) * mod + mod : val);
}
