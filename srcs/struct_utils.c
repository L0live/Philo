/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yanolive <yanolive@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 14:37:34 by yanolive          #+#    #+#             */
/*   Updated: 2024/06/21 17:29:07 by yanolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_lst_clear(t_philo	*philo)
{
	t_philo	*tmp_philo;
	t_fork	*tmp_fork;
	int		len;

	len = philo->philo;
	free(philo->print);
	while (len > 0)
	{
		tmp_philo = philo;
		tmp_fork = philo->next;
		philo = tmp_fork->next;
		free(tmp_philo);
		free(tmp_fork->fork);
		free(tmp_fork);
		--len;
	}
}

void	fork_lst_clear(t_fork *fork)
{
	t_fork	*tmp_fork;
	t_philo	*tmp_philo;
	int		len;

	len = fork->next->philo;
	free(fork->next->print);
	while (len > 0)
	{
		tmp_fork = fork;
		tmp_philo = fork->next;
		fork = tmp_philo->next;
		free(tmp_fork->fork);
		free(tmp_fork);
		free(tmp_philo);
		--len;
	}
}

void	fork_clear(t_fork *fork)
{
	free(fork->fork);
	free(fork);
}

void	lst_connector(t_fork *fork)
{
	t_fork	*tmp;

	tmp = fork;
	while (tmp->next->next)
		tmp = tmp->next->next;
	tmp->next->next = fork;
	fork->prev = tmp->next;
}

void	thread_clear(t_thread *thread)
{
	int	i;

	i = 0;
	while (thread->threads[i])
	{
		pthread_join(thread->threads[i], NULL);
		++i;
	}
	free(thread->threads);
	free(thread->philos);
	free(thread);
}
