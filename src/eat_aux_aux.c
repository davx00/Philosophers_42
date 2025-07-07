/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat_aux_aux.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: despanol <despanol@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 10:36:37 by despanol          #+#    #+#             */
/*   Updated: 2025/07/02 20:10:48 by despanol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sch/philosophers.h"

void	execute_eating_process(t_each_philo *philo, int forks_acquired)
{
	int	stop;

	pthread_mutex_lock(&philo->args->monitor_lock);
	stop = philo->args->stop_simulation;
	pthread_mutex_unlock(&philo->args->monitor_lock);
	if (stop)
	{
		if (forks_acquired)
			release_eating_forks(philo);
		return ;
	}
	update_meal_data(philo);
	print_eating_status(philo);
	usleep(philo->args->time_to_eat * 1000);
	if (forks_acquired)
		release_eating_forks(philo);
}

void	update_meal_data(t_each_philo *philo)
{
	pthread_mutex_lock(&philo->last_time_meal_mutex);
	gettimeofday(&philo->last_meal, NULL);
	philo->meals += 1;
	pthread_mutex_unlock(&philo->last_time_meal_mutex);
}

void	print_eating_status(t_each_philo *philo)
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
		printf("%ld %d is eating\n", milisecs, philo->id);
	}
	pthread_mutex_unlock(&philo->args->monitor_lock);
	pthread_mutex_unlock(&philo->args->print_lock);
}

void	release_eating_forks(t_each_philo *philo)
{
	if (philo->left_fork != -1)
		pthread_mutex_unlock(&philo->args->forks[philo->left_fork]);
	if (philo->right_fork != -1)
		pthread_mutex_unlock(&philo->args->forks[philo->right_fork]);
}

int	acquire_forks(t_each_philo *philo)
{
	if (philo->left_fork < philo->right_fork)
		return (first_case(philo));
	else
		return (second_case(philo));
}
