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

	timestamp = get_time();
	while (get_time() - timestamp < sleep
		&& get_time() - timestamp < philo->dying)
		usleep(100);
	philo->dying -= sleep;
	if (philo->dying <= 0 && print(philo, "DIE") == -1)
		return (-1);
	return (0);
}

void	wait_for_fork(t_philo *philo, t_fork *fork, int fork_nb)
{
	long	timestamp;

	timestamp = get_time();
	if (philo->philo % 2 && philo->prev->prev->philo == 1
			&& ((fork_nb == 1 && philo->to_sleep >= philo->to_eat)
			|| (fork_nb == 2 && philo->to_sleep < philo->to_eat)))
		usleep(500);
	pthread_mutex_lock(fork->fork);
	while (fork->fork_bool == TRUE
			&& philo->dying - (get_time() - timestamp) > 0)
	{
		pthread_mutex_unlock(fork->fork);
		pthread_mutex_lock(fork->fork);
	}
	fork->fork_bool = TRUE;
	pthread_mutex_unlock(fork->fork);
	philo->dying -= get_time() - timestamp;
}

void	fork_unlock(t_fork *fork)
{
	pthread_mutex_lock(fork->fork);
	fork->fork_bool = FALSE;
	pthread_mutex_unlock(fork->fork);
}

int	fork_lock(t_philo *philo, t_fork *fork_1, t_fork *fork_2)
{
	wait_for_fork(philo, fork_1, 1);
	if (print(philo, "has taken a fork") == -1)
	{
		fork_unlock(fork_1);
		return (-1);
	}
	wait_for_fork(philo, fork_2, 2);
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
	t_fork	*fork_1;
	t_fork	*fork_2;

	fork_1 = philo->prev;
	fork_2 = philo->next;
	if (fork_1->fork > fork_2->fork)
	{
		fork_1 = philo->next;
		fork_2 = philo->prev;
	}
	if (fork_lock(philo, fork_1, fork_2) == -1)
		return (-1);
	philo->dying = philo->to_die;
	if (print(philo, "is eating") == -1
		|| ft_usleep(philo, philo->to_eat) == -1)
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
	if (print(philo, "is sleeping") == -1)
		return (-1);
	if (ft_usleep(philo, philo->to_sleep) == -1)
		return (-1);
	if (print(philo, "is thinking") == -1)
		return (-1);
	return (0);
}
