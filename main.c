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
	t_philo	*philo;

	if ((ac < 5 || ac > 6 || check_params(&av[1]) == -1))
	{
		write(2, "Error\n", 6);
		return (EXIT_FAILURE);
	}
	philo = philo_init(NULL, 1, &av[2], ph_atol(av[1]));
	if (!philo)
		return (EXIT_FAILURE);
	if (thread_main(philo) == -1)
	{
		philo_lst_clear(philo->next);
		return (EXIT_FAILURE);
	}
	philo_lst_clear(philo->next);
	return (EXIT_SUCCESS);
}

int	thread_main(t_philo *philo)
{
	t_thread	*thread;
	t_thread	*tmp;

	thread = thread_init(philo, philo->next->philo);
	if (!thread)
		return (-1);
	tmp = thread;
	while (tmp)
	{
		if (pthread_create(&tmp->thread, NULL, philo_main, tmp->philo) != 0)
		{
			write(2, "Error\n", 6);
			thread_clear(thread);
			return (-1);
		}
		tmp = tmp->next;
	}
	thread_clear(thread);
	return (0);
}

void	*philo_main(void *ptr)
{
	t_philo	*philo;
	int		i;

	i = 0;
	philo = ptr;
	print_or_die(philo, "SET TIME");
	while (philo && philo->dying >= 0 && philo->must_eat != 0)
	{
		if (philo != philo->next && (philo->philo + i) % 2 == 0)
		{
			philo->must_eat -= 1;
			if (eating(philo) == -1)
				break ;
		}
		else if (philo != philo->next)
		{
			if (sleeping(philo, i) == -1)
				break ;
		}
		else if (thinking(philo, philo->dying + 1) == -1)
			break ;
		++i;
	}
	return (NULL);
}
