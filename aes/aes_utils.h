/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aes_utils.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/05 15:48:44 by herrfalco         #+#    #+#             */
/*   Updated: 2022/06/23 15:51:44 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AES_UTILS_H
#define AES_UTILS_H

#include "aes.h"

uint64_t		round_up(uint64_t val, uint64_t mod);

#endif // AES_UTILS_H
