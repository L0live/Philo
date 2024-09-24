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

int	ft_usleep(long long timestamp, long sleep)
{
	while (get_time() - timestamp < sleep)
		usleep(100);
	return (0);
}

void	get_forks(t_philo *philo, t_philo **fork_1, t_philo **fork_2)
{
	*fork_1 = philo->thread->philos[philo->philo - 1];
	*fork_2 = philo->thread->philos[philo->philo];
	if (!philo->thread->philos[philo->philo])
	{
		*fork_1 = philo->thread->philos[0];
		*fork_2 = philo->thread->philos[philo->philo - 1];
	}
}

void	wait_for_fork(t_philo *philo, t_philo *philo_fork)
{
	if (philo_fork->thread->philo_nb % 2)
		usleep(500);
	pthread_mutex_lock(philo_fork->fork);
	while (philo_fork->fork_bool == TRUE
		&& get_time() - philo->last_meal <= philo->to_die)
	{
		pthread_mutex_unlock(philo_fork->fork);
		pthread_mutex_lock(philo_fork->fork);
	}
	philo_fork->fork_bool = TRUE;
	pthread_mutex_unlock(philo_fork->fork);
}

void	fork_unlock(t_philo *philo_fork)
{
	pthread_mutex_lock(philo_fork->fork);
	philo_fork->fork_bool = FALSE;
	pthread_mutex_unlock(philo_fork->fork);
}

int	fork_lock(t_philo *philo, t_philo *fork_1, t_philo *fork_2)
{
	wait_for_fork(philo, fork_1);
	if (print(philo, "has taken a fork") == -1)
	{
		fork_unlock(fork_1);
		return (-1);
	}
	wait_for_fork(philo, fork_2);
	if (print(philo, "has taken a fork") == -1)
	{
		fork_unlock(fork_1);
		fork_unlock(fork_2);
		return (-1);
	}
	return (0);
}

int	eating(t_philo *philo)
{
	long long	timestamp;
	t_philo	*fork_1;
	t_philo	*fork_2;

	get_forks(philo, &fork_1, &fork_2);
	if (fork_lock(philo, fork_1, fork_2) == -1)
		return (-1);
	timestamp = get_time();
	pthread_mutex_lock(philo->thread->super);
	philo->last_meal = get_time();
	pthread_mutex_unlock(philo->thread->super);
	if (print(philo, "is eating") == -1
		|| ft_usleep(timestamp, philo->to_eat) == -1)
	{
		fork_unlock(fork_1);
		fork_unlock(fork_2);
		return (-1);
	}
	fork_unlock(fork_1);
	fork_unlock(fork_2);
	return (0);
}

int	sleeping(t_philo *philo)
{
	long long	timestamp;

	timestamp = get_time();
	if (print(philo, "is sleeping") == -1)
		return (-1);
	if (ft_usleep(timestamp, philo->to_sleep) == -1)
		return (-1);
	if (print(philo, "is thinking") == -1)
		return (-1);
	return (0);
}
