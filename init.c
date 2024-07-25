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

t_philo	*philo_init(t_fork *next, long nb, char **av, long count)
{
	t_philo	*philo;

	if (!count)
		lst_connector(next);
	if (!count)
		return (next->prev);
	philo = malloc(sizeof(t_philo));
	if (!philo)
		fork_lst_clear(next);
	if (!philo)
		return (NULL);
	philo->next = next;
	philo->philo = nb;
	philo->to_die = (long long)ph_atol(av[0]) * 1000;
	philo->to_eat = (long long)ph_atol(av[1]) * 1000;
	philo->to_sleep = (long long)ph_atol(av[2]) * 1000;
	philo->must_eat = ph_atol(av[3]);
	philo->dying = philo->to_die;
	philo->prev = mutex_init(philo, nb, av, count);
	if (!philo->prev)
	{
		philo_lst_clear(philo);
		return (NULL);
	}
	return (philo);
}

t_fork	*mutex_init(t_philo *philo, long nb, char **av, long count)
{
	t_fork	*fork;

	fork = malloc(sizeof(t_fork));
	if (!fork)
		return (NULL);
	fork->next = philo;
	fork->fork_bool = FALSE;
	if (!philo->next)
	{
		philo->print = malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init(philo->print, NULL);
	}
	else
		philo->print = philo->next->next->print;
	fork->fork = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(fork->fork, NULL);
	fork->check_fork = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(fork->check_fork, NULL);
	fork->prev = philo_init(fork, nb + 1, av, count - 1);
	if (!fork->prev)
	{
		fork_clear(fork);
		return (NULL);
	}
	return (fork);
}

t_thread	*thread_init(t_philo *philo, int odd, long count)
{
	t_thread	*lst;
	pthread_t	thread;

	thread = 0;
	lst = malloc(sizeof(t_thread));
	if (!lst)
		return (NULL);
	philo->odd = odd;
	lst->philo = philo;
	lst->print = philo->print;
	lst->thread = thread;
	lst->next = NULL;
	if (count == 1)
		return (lst);
	lst->next = thread_init(philo->prev->prev, odd, count - 1);
	if (!lst->next)
	{
		free(lst);
		return (NULL);
	}
	return (lst);
}
