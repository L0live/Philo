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
		philo_lst_clear(philo->next->next);
		return (EXIT_FAILURE);
	}
	philo_lst_clear(philo->next->next);
	return (EXIT_SUCCESS);
}

int	thread_main(t_philo *philo)
{
	t_thread	*thread;
	t_thread	*tmp;
	int			odd;

	odd = FALSE;
	if (philo->next->next->philo % 2)
		odd = TRUE;
	thread = thread_init(philo, odd, philo->next->next->philo);
	if (!thread)
		return (-1);
	tmp = thread;
	pthread_mutex_lock(philo->print);
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
	pthread_mutex_unlock(philo->print);
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
	if (philo->philo % 2 && ++i
		&& (print_or_die(philo, "is thinking") == -1
		|| ft_usleep(philo, philo->to_eat) == -1))
		return (NULL);
	while (philo->must_eat)
	{
		if ((philo->philo + i) % 2 == 0 && philo->must_eat != 0)
		{
			philo->must_eat -= 1;
			if (eating(philo) == -1)
				break ;
			++i;
			continue ;
		}
		if (i && sleeping(philo) == -1)
			break ;
		if (print_or_die(philo, "is thinking") == -1)
			break ;
		if (philo->odd == TRUE && ft_usleep(philo, 500) == -1)
			break ;
		++i;
	}
	return (NULL);
}
