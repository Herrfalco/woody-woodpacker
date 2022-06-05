/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   crypt_steps.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/05 15:43:12 by herrfalco         #+#    #+#             */
/*   Updated: 2022/06/05 15:50:36 by herrfalco        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CRYPT_STEPS_H
#define CRYPT_STEPS_H

void	sub_bytes(uint8_t *block, method_t type);
void	shift_rows(uint8_t *block, method_t type);
void	mix_columns(uint8_t *block, method_t type);
void	add_rkeys(uint8_t *block, uint32_t *rkeys, uint8_t *rk_i);

#endif // CRYPT_STEPS_H
