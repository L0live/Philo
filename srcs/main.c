/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yanolive <yanolive@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 14:31:41 by yanolive          #+#    #+#             */
/*   Updated: 2024/06/21 17:31:06 by yanolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	t_thread	*thread;

	if ((ac < 5 || ac > 6 || check_params(&av[1]) == -1))
	{
		write(2, "Error\n", 6);
		return (EXIT_FAILURE);
	}
	thread = thread_init(&av[2], ph_atol(av[1]));
	if (!thread || thread_main(thread, ph_atol(av[1])) == -1)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

void	supervisor(t_thread *thread, long philo_nb)
{
	int	eat_count;
	int	i;

	eat_count = 0;
	i = 0;
	while (TRUE)
	{
		pthread_mutex_lock(thread->super);
		if (thread->philos[i]->must_eat == 0)
		{
			++eat_count;
			thread->philos[i]->must_eat -= 1;
		}
		if (get_time() - thread->philos[i]->last_meal >= thread->philos[i]->to_die)
		{
			pthread_mutex_unlock(thread->super);
			print(thread->philos[i], "died");
			break ;
		}
		pthread_mutex_unlock(thread->super);
		if (eat_count == philo_nb)
		{
			print(thread->philos[i], NULL);
			break ;
		}
		++i;
		if (!thread->philos[i])
			i = 0;
	}
}

void	set_time(t_thread *thread)
{
	int	i;

	thread->time = get_time();
	i = 0;
	while (thread->philos[i])
	{
		thread->philos[i]->last_meal = thread->time;
		++i;
	}
}

int	thread_main(t_thread *thread, long philo_nb)
{
	int	i;

	i = 0;
	pthread_mutex_lock(thread->print);
	while (i < philo_nb)
	{
		if (pthread_create(&thread->threads[i], NULL, philo_main, thread->philos[i]) != 0)
		{
			write(2, "Error\n", 6);
			thread_clear(thread);
			return (-1);
		}
		++i;
	}
	set_time(thread);
	pthread_mutex_unlock(thread->print);
	supervisor(thread, philo_nb);
	thread_join(thread);
	return (0);
}

void	*philo_main(void *ptr)
{
	t_philo		*philo;
	int			i;
	long long	timestamp;

	philo = ptr;
	i = 0;
	timestamp = get_time();
	if (philo->philo % 2 && ++i && philo->thread->philos[1]
		&& (print(philo, "is thinking") == -1
			|| ft_usleep(timestamp, philo->to_eat) == -1))
		return (NULL);
	while (TRUE)
	{
		if ((philo->philo + i) % 2 == 0)
		{
			if (eating(philo) == -1)
				break ;
			++i;
			continue ;
		}
		if (sleeping(philo) == -1)
			break ;
		++i;
	}
	return (NULL);
}
