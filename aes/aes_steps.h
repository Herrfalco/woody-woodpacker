/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aes_steps.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/05 15:43:12 by herrfalco         #+#    #+#             */
/*   Updated: 2022/06/15 15:40:11 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AES_STEPS_H
#define AES_STEPS_H

void	shift_rows(uint8_t *block, method_t type);
void	mix_columns(uint8_t *block, method_t type);
void	add_rkeys(uint8_t *block, uint32_t *rkeys, uint8_t round);

#endif // AES_STEPS_H
