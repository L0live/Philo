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

# define FALSE 0
# define TRUE 1
# define VIP_ACCESS 2

typedef struct s_philo t_philo;

typedef struct s_fork
{
	pthread_mutex_t	*fork;
	pthread_mutex_t	*check_fork;
	int				fork_bool;
	struct s_philo	*prev;
	struct s_philo	*next;
}	t_fork;

typedef struct s_philo
{
	long			philo;
	long long		dying;
	long long		to_die;
	long long		to_eat;
	long long		to_sleep;
	long			must_eat;
	int				odd;
	pthread_mutex_t	*print;
	t_fork			*prev;
	t_fork			*next;
}	t_philo;

typedef struct s_thread
{
	pthread_t		thread;
	t_philo			*philo;
	pthread_mutex_t	*print;
	struct s_thread	*next;
}	t_thread;

void		*philo_main(void *ptr);
int			ft_usleep(t_philo *philo, long long sleep);
int			eating(t_philo *philo);
int			sleeping(t_philo *philo);
int			thread_main(t_philo *philo);
t_thread	*thread_init(t_philo *philo, int odd, long count);
long long	get_time(void);
int			check_params(char **av);
long		ph_atol(char *str);
int			print_or_die(t_philo *philo, char *str);
t_philo		*philo_init(t_fork *next, long nb, char **av, long count);
t_fork		*mutex_init(t_philo *philo, long nb, char **av, long count);
void		philo_lst_clear(t_philo	*philo);
void		fork_lst_clear(t_fork *fork);
void		fork_clear(t_fork *fork);
void		thread_clear(t_thread *thread);
void		lst_connector(t_fork *fork);

#endif