/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aes_utils.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: herrfalco <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/05 15:48:44 by herrfalco         #+#    #+#             */
/*   Updated: 2022/06/09 13:06:49 by herrfalco        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AES_UTILS_H
#define AES_UTILS_H

typedef enum	endian_e {
	NO,
	BIG,
	LITTLE,
}				endian_t;

endian_t		get_endian(void);
uint8_t			get_rnb(void);
void			rot_word(uint32_t *word, uint8_t shift, method_t type);
void			sub_word(uint8_t *word, method_t type);

#endif // AES_UTILS_H
