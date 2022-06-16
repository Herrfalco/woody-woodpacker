/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aes_steps.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/05 15:43:12 by herrfalco         #+#    #+#             */
/*   Updated: 2022/06/16 12:31:34 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AES_STEPS_H
#define AES_STEPS_H

void	mix_columns(uint8_t *block, method_t type);

#endif // AES_STEPS_H
