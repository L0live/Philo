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

void	philos_clear(t_philo **philos)
{
	int	i;

	i = 0;
	while (philos[i])
	{
		free(philos[i]->fork);
		free(philos[i]);
		++i;
	}
	free(philos);
}

t_thread	*thread_clear(t_thread *thread)
{
	if (thread->print)
		free(thread->print);
	if (thread->super)
		free(thread->super);
	if (thread->philos)
		philos_clear(thread->philos);
	if (thread->threads)
		free(thread->threads);
	free(thread);
	return (NULL);
}

void	thread_join(t_thread *thread)
{
	int	i;

	i = 0;
	while (thread->threads[i])
	{
		pthread_join(thread->threads[i], NULL);
		++i;
	}
	thread_clear(thread);
}
