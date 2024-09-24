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
		if (!ph_atol(av[k]) || ph_atol(av[k]) == -1)
			return (-1);
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
	if (nbr > 2147483647)
		return (-1);
	return (nbr);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	if (!s1 || !s2)
		return (-1);
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

int	print(t_philo *philo, char *str)
{
	static int	print_bool = TRUE;

	if (!ft_strcmp(str, "is eating"))
	{
		pthread_mutex_lock(philo->thread->super);
		philo->must_eat -= 1;
		pthread_mutex_unlock(philo->thread->super);
	}
	pthread_mutex_lock(philo->thread->print);
	if (!str || print_bool == FALSE)
	{
		print_bool = FALSE;
		pthread_mutex_unlock(philo->thread->print);
		return (-1);
	}
	if (print_bool == TRUE)
		printf("%lld %d %s\n", get_time() - philo->thread->time, philo->philo, str);
	if (!ft_strcmp(str, "died"))
		print_bool = FALSE;
	pthread_mutex_unlock(philo->thread->print);
	return (0);
}
