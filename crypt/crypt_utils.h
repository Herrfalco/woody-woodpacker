/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   crypt_utils.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/05 15:48:44 by herrfalco         #+#    #+#             */
/*   Updated: 2022/06/05 15:58:51 by herrfalco        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CRYPT_UTILS_H
#define CRYPT_UTILS_H

endian_t		get_endian(void);
uint8_t			get_rnb(void);
void			rot_word(uint32_t *word, uint8_t shift, method_t type);
void			sub_word(uint8_t *word, method_t type);

#endif // CRYPT_UTILS_H
