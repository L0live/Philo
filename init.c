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
	philo->to_die = (long long)ph_atol(av[0]);
	philo->to_eat = (long long)ph_atol(av[1]);
	philo->to_sleep = (long long)ph_atol(av[2]);
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
	fork->prev = philo_init(fork, nb + 1, av, count - 1);
	if (!fork->prev)
	{
		fork_clear(fork);
		return (NULL);
	}
	return (fork);
}

t_philo	**philo_to_array(t_philo *philo, long count)
{
	t_philo	**philos;
	t_philo	*tmp;
	int		i;

	philos = malloc(sizeof(t_philo *) * (count + 1));
	if (!philos)
		return (NULL);
	i = 0;
	tmp = philo;
	while (i < count)
	{
		philos[i] = tmp;
		tmp = tmp->prev->prev;
		++i;
	}
	philos[i] = NULL;
	return (philos);
}



t_thread	*thread_init(t_philo *philo, long philo_nb)
{
	t_thread	*thread;
	int			i;

	thread = malloc(sizeof(t_thread));
	if (!thread)
		return (NULL);
	thread->philos = philo_to_array(philo, philo_nb);
	if (!thread->philos)
	{
		free(thread);
		return (NULL);
	}
	thread->print = philo->print;
	thread->threads = malloc(sizeof(pthread_t) * (philo_nb + 1));
	if (!thread->threads)
	{
		free(thread->philos);
		free(thread);
		return (NULL);
	}
	i = -1;
	while (++i <= philo_nb)
		thread->threads[i] = 0;
	return (thread);
}
