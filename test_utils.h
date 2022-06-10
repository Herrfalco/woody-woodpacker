/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_utils.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 15:26:15 by fcadet            #+#    #+#             */
/*   Updated: 2022/06/10 15:29:06 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_UTILS_H
#define TEST_UTILS_H

int		close_ret(int f1, int f2, int f3, int ret);
int		rand_v_file(int *file, uint64_t size);
int		diff_v_files(int f1, int f2);

#endif // TEST_UTILS_H
