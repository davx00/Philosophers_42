/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat_aux.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: despanol <despanol@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 09:43:47 by despanol          #+#    #+#             */
/*   Updated: 2025/07/02 20:12:04 by despanol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sch/philosophers.h"

void	execute_eating_action(t_each_philo *philo)
{
	int	finish_eating;
	int	stop;
	int	forks_acquired;

	if (philo->left_fork == -1)
		usleep(philo->args->time_to_die * 1000 + 1);
	pthread_mutex_lock(&philo->args->monitor_lock);
	finish_eating = (philo->meals >= philo->args->meals
			&& philo->args->meals != -1);
	stop = philo->args->stop_simulation;
	pthread_mutex_unlock(&philo->args->monitor_lock);
	if (finish_eating || stop)
		return ;
	forks_acquired = acquire_forks(philo);
	execute_eating_process(philo, forks_acquired);
}

void	philosopher_sleep(t_each_philo *philo)
{
	long	milisecs;
	int		stop;

	pthread_mutex_lock(&philo->args->monitor_lock);
	stop = philo->args->stop_simulation;
	pthread_mutex_unlock(&philo->args->monitor_lock);
	if (stop)
		return ;
	pthread_mutex_lock(&philo->args->print_lock);
	pthread_mutex_lock(&philo->args->monitor_lock);
	if (!philo->args->stop_simulation)
	{
		milisecs = get_timestamp(philo->args->time_now);
		printf("%ld %d is sleeping\n", milisecs, philo->id);
	}
	pthread_mutex_unlock(&philo->args->monitor_lock);
	pthread_mutex_unlock(&philo->args->print_lock);
	usleep(philo->args->time_to_sleep * 1000);
}

void	think(t_each_philo *philo)
{
	long	milisecs;
	int		stop;

	pthread_mutex_lock(&philo->args->monitor_lock);
	stop = philo->args->stop_simulation;
	pthread_mutex_unlock(&philo->args->monitor_lock);
	if (stop)
		return ;
	pthread_mutex_lock(&philo->args->print_lock);
	pthread_mutex_lock(&philo->args->monitor_lock);
	if (!philo->args->stop_simulation)
	{
		milisecs = get_timestamp(philo->args->time_now);
		printf("%ld %d is thinking\n", milisecs, philo->id);
	}
	pthread_mutex_unlock(&philo->args->monitor_lock);
	pthread_mutex_unlock(&philo->args->print_lock);
	usleep(500);
}
