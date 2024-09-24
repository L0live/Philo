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

t_philo	*philo_init(t_thread *thread, char **av, int philo_nb)
{
	t_philo	*philo;

	philo = malloc(sizeof(t_philo));
	if (!philo)
		return (NULL);
	philo->philo = philo_nb;
	philo->to_die = ph_atol(av[0]);
	philo->to_eat = ph_atol(av[1]);
	philo->to_sleep = ph_atol(av[2]);
	philo->must_eat = ph_atol(av[3]);
	philo->fork_bool = FALSE;
	philo->fork = malloc(sizeof(pthread_mutex_t));
	if (!philo->fork)
	{
		free(philo);
		return (NULL);
	}
	pthread_mutex_init(philo->fork, NULL);
	philo->thread = thread;
	return (philo);
}

t_philo	**create_philos(t_thread *thread, char **av, long philo_nb)
{
	t_philo	**philos;
	int		i;

	philos = malloc(sizeof(t_philo *) * (philo_nb + 1));
	if (!philos)
		return (NULL);
	i = 0;
	while (i < philo_nb)
	{
		philos[i] = philo_init(thread, av, i + 1);
		if (!philos[i])
		{
			philos_clear(philos);
			return (NULL);
		}
		++i;
	}
	philos[i] = NULL;
	return (philos);
}

pthread_t	*create_threads(long philo_nb)
{
	pthread_t	*threads;
	int			i;

	threads = malloc(sizeof(pthread_t) * (philo_nb + 1));
	if (!threads)
		return (NULL);
	i = 0;
	while (i <= philo_nb)
	{
		threads[i] = 0;
		++i;
	}
	return (threads);
}

t_thread	*thread_init(char **av, long philo_nb)
{
	t_thread	*thread;

	thread = malloc(sizeof(t_thread));
	if (!thread)
		return (NULL);
	thread->philo_nb = philo_nb;
	thread->print = NULL;
	thread->super = NULL;
	thread->philos = NULL;
	thread->threads = NULL;
	thread->print = malloc(sizeof(pthread_mutex_t));
	if (!thread->print)
		return (thread_clear(thread));
	pthread_mutex_init(thread->print, NULL);
	thread->super = malloc(sizeof(pthread_mutex_t));
	if (!thread->print)
		return (thread_clear(thread));
	pthread_mutex_init(thread->super, NULL);
	thread->threads = create_threads(philo_nb);
	if (!thread->threads)
		return (thread_clear(thread));
	thread->philos = create_philos(thread, av, philo_nb);
	if (!thread->philos)
		return (thread_clear(thread));
	return (thread);
}
