/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yanolive <yanolive@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 14:35:11 by yanolive          #+#    #+#             */
/*   Updated: 2024/06/21 17:28:32 by yanolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/time.h>

# define TRUE 1
# define FALSE 0

typedef struct s_thread	t_thread;

typedef struct s_philo
{
	int				philo;
	long			to_die;
	long			to_eat;
	long			to_sleep;
	long			must_eat;
	long long		last_meal;
	int				fork_bool;
	pthread_mutex_t	*fork;
	t_thread		*thread;
}	t_philo;

typedef struct s_thread
{
	pthread_t		*threads;
	t_philo			**philos;
	int				philo_nb;
	long long		time;
	pthread_mutex_t	*print;
	pthread_mutex_t	*super;
}	t_thread;

void		*philo_main(void *ptr);
int			ft_usleep(long long timestamp, long sleep);
int			eating(t_philo *philo);
int			sleeping(t_philo *philo);
int			thread_main(t_thread *thread, long philo_nb);
t_thread	*thread_init(char **av, long philo_nb);
long long	get_time(void);
int			check_params(char **av);
long		ph_atol(char *str);
int			print(t_philo *philo, char *str);
void		philos_clear(t_philo **philos);
void		thread_join(t_thread *thread);
t_thread	*thread_clear(t_thread *thread);

#endif