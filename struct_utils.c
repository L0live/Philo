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
	t_philo	*tmp;
	int		len;

	len = philo->philo;
	free(philo->print);
	while (len > 0)
	{
		tmp = philo;
		philo = philo->next;
		free(tmp->fork);
		if (tmp->philo % 2)
			free(tmp->right_fork);
		free(tmp);
		--len;
	}
}

void	philo_clear(t_philo	*philo)
{
	if (philo->print)
	{
		free(philo->print);
		philo->print = NULL;
	}
	free(philo->fork);
	if (philo->left_fork)
		free(philo->left_fork);
	else if (philo->right_fork)
		free(philo->right_fork);
	free(philo);
}

void	philo_connector(t_philo *philo)
{
	t_philo	*tmp;

	tmp = philo;
	while (tmp->next)
		tmp = tmp->next;
	philo->prev = tmp;
	tmp->next = philo;
}

void	thread_clear(t_thread *thread)
{
	t_thread	*tmp;

	tmp = thread;
	while (tmp)
	{
		thread = thread->next;
		pthread_join(tmp->thread, NULL);
		free(tmp);
		tmp = thread;
	}
}
