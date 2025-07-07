/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: despanol <despanol@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 13:13:09 by despanol          #+#    #+#             */
/*   Updated: 2025/07/05 12:24:24 by despanol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sch/philosophers.h"

int	check_args(int argc, char **argv)
{
	int	i;

	i = 1;
	if (argc >= 5 && argc <= 6)
	{
		while (i < argc)
		{
			if (ft_atoi(argv[i]) == 0)
			{
				return (1);
			}
			if (isnumber(argv[i]) == 1)
			{
				return (1);
			}
			i++;
		}
	}
	else
	{
		return (1);
	}
	return (0);
}

int	isnumber(char *nbr)
{
	int	i;

	i = 0;
	while (nbr[i])
	{
		if (!(nbr[i] >= 48 && nbr[i] <= 57))
			return (1);
		i++;
	}
	return (0);
}
