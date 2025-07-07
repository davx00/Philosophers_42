/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: despanol <despanol@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 09:13:40 by despanol          #+#    #+#             */
/*   Updated: 2025/06/29 13:46:56 by despanol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sch/philosophers.h"

static int	simulation_stopped(t_each_philo *philo)
{
	int	stopped;

	pthread_mutex_lock(&philo->args->monitor_lock);
	stopped = philo->args->stop_simulation;
	pthread_mutex_unlock(&philo->args->monitor_lock);
	return (stopped);
}

static void	pick_fork(t_each_philo *philo)
{
	long	milisecs;

	if (simulation_stopped(philo))
		return ;
	pthread_mutex_lock(&philo->args->print_lock);
	if (!simulation_stopped(philo))
	{
		milisecs = get_timestamp(philo->args->time_now);
		printf("%ld %d has taken a fork\n", milisecs, philo->id);
	}
	pthread_mutex_unlock(&philo->args->print_lock);
}

int	first_case(t_each_philo *philo)
{
	pthread_mutex_lock(&philo->args->forks[philo->left_fork]);
	if (simulation_stopped(philo))
	{
		pthread_mutex_unlock(&philo->args->forks[philo->left_fork]);
		return (0);
	}
	pick_fork(philo);
	pthread_mutex_lock(&philo->args->forks[philo->right_fork]);
	if (simulation_stopped(philo))
	{
		pthread_mutex_unlock(&philo->args->forks[philo->right_fork]);
		pthread_mutex_unlock(&philo->args->forks[philo->left_fork]);
		return (0);
	}
	pick_fork(philo);
	return (1);
}

int	second_case(t_each_philo *philo)
{
	pthread_mutex_lock(&philo->args->forks[philo->right_fork]);
	if (simulation_stopped(philo))
	{
		pthread_mutex_unlock(&philo->args->forks[philo->right_fork]);
		return (0);
	}
	pick_fork(philo);
	pthread_mutex_lock(&philo->args->forks[philo->left_fork]);
	if (simulation_stopped(philo))
	{
		pthread_mutex_unlock(&philo->args->forks[philo->left_fork]);
		pthread_mutex_unlock(&philo->args->forks[philo->right_fork]);
		return (0);
	}
	pick_fork(philo);
	return (1);
}
