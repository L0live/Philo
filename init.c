/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yanolive <yanolive@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 14:38:00 by yanolive          #+#    #+#             */
/*   Updated: 2024/06/21 17:29:27 by yanolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_philo	*philo_init(t_philo *next, long nb, char **av, long count)
{
	t_philo	*philo;

	philo = malloc(sizeof(t_philo));
	if (!philo)
	{
		philo_clear(next);
		return (NULL);
	}
	philo->next = next;
	philo->philo = nb;
	philo->to_die = (long long)ph_atol(av[0]) * 1000;
	philo->to_eat = (long long)ph_atol(av[1]) * 1000;
	philo->to_sleep = (long long)ph_atol(av[2]) * 1000;
	philo->must_eat = ph_atol(av[3]);
	philo->dying = philo->to_die;
	philo->prev = NULL;
	philo = mutex_init(philo, next, nb);
	if (count > 1)
		philo->prev = philo_init(philo, nb + 1, av, count - 1);
	else
		philo_connector(philo);
	return (philo);
}

t_philo	*mutex_init(t_philo *philo, t_philo *next, long nb)
{
	if (!next)
	{
		philo->print = malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init(philo->print, NULL);
	}
	else
		philo->print = next->print;
	philo->fork = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(philo->fork, NULL);
	philo->left_fork = NULL;
	philo->right_fork = NULL;
	if (nb % 2 == 0)
		philo->left_fork = philo->next->right_fork;
	else
	{
		philo->right_fork = malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init(philo->right_fork, NULL);
	}
	return (philo);
}

t_thread	*thread_init(t_philo *philo, long count)
{
	t_thread	*lst;
	pthread_t	thread;

	thread = 0;
	lst = malloc(sizeof(t_thread));
	if (!lst)
		return (NULL);
	lst->philo = philo;
	lst->print = philo->print;
	lst->thread = thread;
	lst->next = NULL;
	if (count == 1)
		return (lst);
	lst->next = thread_init(philo->prev, count - 1);
	if (!lst->next)
	{
		free(lst);
		return (NULL);
	}
	return (lst);
}
