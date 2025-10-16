/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 15:57:18 by nmascaro          #+#    #+#             */
/*   Updated: 2025/10/16 10:45:10 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	check_overflow(unsigned long long res, int digit, int sign)
{
	if (res > (9223372036854775807ULL - digit) / 10)
		return (1);
	if ((sign == 1 && res > 9223372036854775807ULL)
		|| (sign == -1 && res > 9223372036854775808ULL))
		return (1);
	return (0);
}

static int	handle_sign(const char *str, int *i)
{
	int	sign;

	sign = 1;
	if (str[*i] == '-' || str[*i] == '+')
	{
		if (str[*i] == '-')
			sign = -1;
		(*i)++;
	}
	return (sign);
}

long long	ft_atol(const char *nptr, int *overflow)
{
	int					sign;
	unsigned long long	result;
	int					i;
	int					digit;

	result = 0;
	i = 0;
	*overflow = 0;
	while (nptr[i] == ' ' || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	sign = handle_sign(nptr, &i);
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		digit = nptr[i] - '0';
		if (check_overflow(result, digit, sign))
		{
			*overflow = 1;
			return (0);
		}
		result = 10 * result + digit;
		i++;
	}
	return (sign * (long long)result);
}
