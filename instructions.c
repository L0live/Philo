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
	while (get_time() - timestamp < sleep / 1000
		&& get_time() - timestamp < philo->dying / 1000)
		usleep(1);
	philo->dying -= sleep;
	if (philo->dying < 0 && print_or_die(philo, "DIE") == -1)
		return (-1);
	return (0);
}

int	eating(t_philo *philo)
{
	pthread_mutex_t	*tmp_fork;

	tmp_fork = philo->left_fork;
	if (philo->philo % 2)
		tmp_fork = philo->right_fork;
	pthread_mutex_lock(tmp_fork);
	if (print_or_die(philo, "has taken a fork") == -1)
		return (-1);
	pthread_mutex_lock(philo->fork);
	if (print_or_die(philo, "has taken a fork") == -1)
		return (-1);
	if (print_or_die(philo, "is eating") == -1)
		return (-1);
	philo->dying = philo->to_die;
	if (ft_usleep(philo, philo->to_eat) == -1)
		return (-1);
	pthread_mutex_unlock(tmp_fork);
	pthread_mutex_unlock(philo->fork);
	return (0);
}

int	sleeping(t_philo *philo, int sync)
{
	if (print_or_die(philo, "is sleeping") == -1)
		return (-1);
	if (ft_usleep(philo, philo->to_sleep) == -1)
		return (-1);
	if (philo->to_eat * 2 > philo->to_die && philo->philo % 2 == 0)
	{
		if (thinking(philo, philo->dying + 1) == -1)
			return (-1);
	}
	else
	{
		if (philo->philo % 2 != 0 && sync == 0)
			philo->dying -= philo->to_eat;
		if (thinking(philo, philo->dying) == -1)
			return (-1);
	}
	return (0);
}

int	thinking(t_philo *philo, long long sleep)
{
	if (print_or_die(philo, "is thinking") == -1)
		return (-1);
	if (ft_usleep(philo, sleep) == -1)
		return (-1);
	return (0);
}
