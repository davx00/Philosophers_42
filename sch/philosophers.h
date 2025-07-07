/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: despanol <despanol@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 12:58:18 by despanol          #+#    #+#             */
/*   Updated: 2025/07/02 20:43:21 by despanol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_gen_args
{
	int				number_of_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				meals;
	pthread_mutex_t	*forks;
	int				stop_simulation;
	pthread_mutex_t	monitor_lock;
	pthread_mutex_t	print_lock;
	struct timeval	time_now;
}				t_gen_args;

typedef struct s_each_philo
{
	int				id;
	pthread_t		thread_philo_id;
	int				meals;
	struct timeval	last_meal;
	pthread_mutex_t	last_time_meal_mutex;
	int				right_fork;
	int				left_fork;
	t_gen_args		*args;
}				t_each_philo;

typedef struct s_monitoring
{
	pthread_t		monitor;
	t_gen_args		*args;
	t_each_philo	*philo;
}				t_monitoring;

int				isnumber(char *nbr);
int				check_args(int argc, char **argv);
void			init_args(t_gen_args *args, char **argv, int argc);
t_each_philo	*init_philo(char **argv, int argc);
int				ft_all_eating(t_monitoring *monitor);
int				ft_die_philo(t_monitoring *monitor, t_each_philo *philo,
					struct timeval current_time);
void			*ft_monitoring(void *monitor_void);
void			*eat(void *philos);
long			get_timestamp(struct timeval start_time);
int				ft_atoi(const char *str);
int				first_case(t_each_philo *philo);
int				second_case(t_each_philo *philo);
void			execute_eating_action(t_each_philo *philo);
void			philosopher_sleep(t_each_philo *philo);
void			think(t_each_philo *philo);
void			cleanup_resources(t_each_philo *philos);
int				init_simulation(t_each_philo **philos, t_monitoring *monitor,
					char **argv, int argc);
int				create_threads(t_each_philo *philos, t_monitoring *monitor);
void			wait_threads(t_each_philo *philos, t_monitoring *monitor);
int				acquire_forks(t_each_philo *philo);
void			execute_eating_process(t_each_philo *philo, int forks_acquired);
void			update_meal_data(t_each_philo *philo);
void			print_eating_status(t_each_philo *philo);
void			release_eating_forks(t_each_philo *philo);
void			assign_forks(t_each_philo *philo, int i, int total_philo);

#endif