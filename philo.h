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

typedef struct s_philo
{
	long			philo;
	long long		dying;
	long long		to_die;
	long long		to_eat;
	long long		to_sleep;
	long			must_eat;
	pthread_mutex_t	*fork;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*print;
	struct s_philo	*prev;
	struct s_philo	*next;
}	t_philo;

typedef struct s_thread
{
	pthread_t		thread;
	t_philo			*philo;
	pthread_mutex_t	*print;
	struct s_thread	*next;
}	t_thread;

void		*philo_main(void *ptr);
int			eating(t_philo *philo);
int			sleeping(t_philo *philo, int sync);
int			thinking(t_philo *philo, long long sleep);
int			thread_main(t_philo *philo);
t_thread	*thread_init(t_philo *philo, long count);
long long	get_time(void);
int			check_params(char **av);
long		ph_atol(char *str);
int			print_or_die(t_philo *philo, char *str);
t_philo		*philo_init(t_philo *next, long nb, char **av, long count);
t_philo		*mutex_init(t_philo *philo, t_philo *next, long nb);
void		philo_lst_clear(t_philo	*philo);
void		philo_clear(t_philo	*philo);
void		thread_clear(t_thread *thread);
void		philo_connector(t_philo *philo);

#endif