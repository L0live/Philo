/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yanolive <yanolive@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 14:35:49 by yanolive          #+#    #+#             */
/*   Updated: 2024/06/21 17:31:39 by yanolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_params(char **av)
{
	int	i;
	int	k;

	k = 0;
	while (av[k])
	{
		i = 0;
		if (!av[k][i])
			return (-1);
		while (av[k][i])
		{
			if (av[k][i] < '0' || av[k][i] > '9')
				return (-1);
			++i;
		}
		++k;
	}
	return (0);
}

long	ph_atol(char *str)
{
	long	nbr;
	int		i;

	i = 0;
	nbr = 0;
	if (!str)
		return (-1);
	while (str[i])
	{
		nbr *= 10;
		nbr += str[i] - 48;
		++i;
	}
	return (nbr);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] || s2[i])
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
		++i;
	}
	return (s1[i] - s2[i]);
}

long long	get_time(void)
{
	struct timeval	tv;
	long long		timestamp;

	gettimeofday(&tv, NULL);
	timestamp = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (timestamp);
}

int	print_or_die(t_philo *philo, char *str)
{
	static long long	time = 0;

	pthread_mutex_lock(philo->print);
	if (time == -1 || !ft_strcmp(str, "SET TIME"))
	{
		if (time == 0)
			time = get_time();
		pthread_mutex_unlock(philo->print);
		return (-1);
	}
	if (philo->dying < 0)
	{
		printf("%lld %ld %s\n", get_time() - time, philo->philo, "died");
		time = -1;
		pthread_mutex_unlock(philo->print);
		return (-1);
	}
	printf("%lld %ld %s\n", get_time() - time, philo->philo, str);
	pthread_mutex_unlock(philo->print);
	return (0);
}
