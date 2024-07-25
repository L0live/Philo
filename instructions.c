/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   instructions.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yanolive <yanolive@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 14:35:23 by yanolive          #+#    #+#             */
/*   Updated: 2024/06/21 17:29:52 by yanolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_usleep(t_philo *philo, long long sleep)
{
	long long	timestamp;

	timestamp = get_time() * 1000;
	while (get_time() * 1000 - timestamp < sleep
		&& get_time() * 1000 - timestamp < philo->dying)
		usleep(1);
	philo->dying -= sleep;
	if (philo->dying <= 0 && print_or_die(philo, "DIE") == -1)
		return (-1);
	return (0);
}

int	fork_lock(t_philo *philo, t_fork *fork_1, t_fork *fork_2)
{
	long long	timestamp;

	timestamp = get_time();
	pthread_mutex_lock(fork_1->fork);
	philo->dying -= (get_time() - timestamp) * 1000;
	if (print_or_die(philo, "has taken a fork") == -1)
	{
		pthread_mutex_unlock(fork_1->fork);
		return (-1);
	}
	if (fork_1 == fork_2 && ft_usleep(philo, philo->dying) == -1)
	{
		pthread_mutex_unlock(fork_1->fork);
		return (-1);
	}
	timestamp = get_time();
	pthread_mutex_lock(fork_2->fork);
	philo->dying -= (get_time() - timestamp) * 1000;
	if (print_or_die(philo, "has taken a fork") == -1)
	{
		pthread_mutex_unlock(fork_1->fork);
		pthread_mutex_unlock(fork_2->fork);
		return (-1);
	}
	return (0);
}

int	eating(t_philo *philo)
{
	t_fork	*fork_1;
	t_fork	*fork_2;

	fork_1 = philo->prev;
	fork_2 = philo->next;
	if (fork_lock(philo, fork_1, fork_2) == -1)
		return (-1);
	philo->dying = philo->to_die;
	if (print_or_die(philo, "is eating") == -1
		|| ft_usleep(philo, philo->to_eat) == -1)
	{
		pthread_mutex_unlock(fork_1->fork);
		pthread_mutex_unlock(fork_2->fork);
		return (-1);
	}
	pthread_mutex_unlock(fork_1->fork);
	pthread_mutex_unlock(fork_2->fork);
	return (0);
}

int	sleeping(t_philo *philo)
{
	if (print_or_die(philo, "is sleeping") == -1)
		return (-1);
	if (ft_usleep(philo, philo->to_sleep) == -1)
		return (-1);
	if (print_or_die(philo, "is thinking") == -1)
		return (-1);
	if (philo->odd == TRUE && ft_usleep(philo, 1000) == -1)
		return (-1);
	return (0);
}
