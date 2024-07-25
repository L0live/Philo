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
	t_philo		*philo;
	int			odd;

	if ((ac < 5 || ac > 6 || check_params(&av[1]) == -1))
	{
		write(2, "Error\n", 6);
		return (EXIT_FAILURE);
	}
	philo = philo_init(NULL, 1, &av[2], ph_atol(av[1]));
	if (!philo)
		return (EXIT_FAILURE);
	odd = FALSE;
	if (philo->next->next->philo % 2)
		odd = TRUE;
	thread = thread_init(philo, odd, philo->next->next->philo);
	if (!thread || thread_main(thread, philo->next->next->philo) == -1)
	{
		philo_lst_clear(philo->next->next);
		return (EXIT_FAILURE);
	}
	philo_lst_clear(philo->next->next);
	return (EXIT_SUCCESS);
}

void	must_eat_count(t_thread *thread, int philo_nb)
{
	t_thread	*tmp;
	int			eat_count;

	pthread_mutex_unlock(thread->print);
	tmp = thread;
	eat_count = 1;
	while (tmp)
	{
		pthread_mutex_lock(tmp->print);
		if (tmp->philo->must_eat == 0)
		{
			++eat_count;
			tmp->philo->must_eat -= 1;
		}
		pthread_mutex_unlock(tmp->print);
		if (eat_count == philo_nb || print_or_die(tmp->philo, "SET TIME") == -1)
		{
			print_or_die(tmp->philo, NULL);
			break ;
		}
		tmp = tmp->next;
		if (!tmp->next)
			tmp = thread;
	}
	pthread_mutex_lock(thread->print);
}

int	thread_main(t_thread *thread, int philo_nb)
{
	t_thread	*tmp;

	tmp = thread;
	pthread_mutex_lock(thread->print);
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
	if (thread->philo->must_eat > 0 && philo_nb > 1)
		must_eat_count(thread, philo_nb);
	pthread_mutex_unlock(thread->print);
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
		&& philo != philo->next->next
		&& (print_or_die(philo, "is thinking") == -1
			|| ft_usleep(philo, philo->to_eat) == -1))
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
