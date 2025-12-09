/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 15:15:56 by lsarraci          #+#    #+#             */
/*   Updated: 2025/08/09 19:51:51 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include<stdarg.h>
# include<unistd.h>

int		ft_printf(const char *fstring, ...);
int		ft_putchar(int c);
long	ft_putstr(const char *ch);
int		ft_converter(const char ch, va_list lst);
int		ft_printnbr(long n);
int		ft_printmod(unsigned int n);
int		ft_printhex(unsigned int ch, int upper);
int		ft_printptr(unsigned long ptr);

#endif