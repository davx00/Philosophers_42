/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: despanol <despanol@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 23:49:25 by despanol          #+#    #+#             */
/*   Updated: 2025/07/02 20:56:15 by despanol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sch/philosophers.h"

int	ft_die_philo(t_monitoring *monitor, t_each_philo *philo,
		struct timeval current_time)
{
	struct timeval	last_meal;
	long			time_diff;

	pthread_mutex_lock(&philo->last_time_meal_mutex);
	last_meal = philo->last_meal;
	pthread_mutex_unlock(&philo->last_time_meal_mutex);
	time_diff = (current_time.tv_sec - last_meal.tv_sec) * 1000000
		+ (current_time.tv_usec - last_meal.tv_usec);
	time_diff /= 1000;
	if (time_diff > monitor->args->time_to_die)
	{
		pthread_mutex_lock(&monitor->args->monitor_lock);
		if (monitor->args->stop_simulation == 0)
		{
			monitor->args->stop_simulation = 1;
			pthread_mutex_unlock(&monitor->args->monitor_lock);
			pthread_mutex_lock(&monitor->args->print_lock);
			printf("%ld %d died\n", get_timestamp(monitor->args->time_now),
				philo->id);
			pthread_mutex_unlock(&monitor->args->print_lock);
			return (1);
		}
		pthread_mutex_unlock(&monitor->args->monitor_lock);
	}
	return (0);
}

int	ft_all_eating(t_monitoring *monitor)
{
	int	i;
	int	meals;

	if (monitor->args->meals <= 0)
		return (0);
	i = 0;
	while (i < monitor->args->number_of_philo)
	{
		pthread_mutex_lock(&monitor->philo[i].last_time_meal_mutex);
		meals = monitor->philo[i].meals;
		pthread_mutex_unlock(&monitor->philo[i].last_time_meal_mutex);
		if (meals < monitor->args->meals)
			return (0);
		i++;
	}
	pthread_mutex_lock(&monitor->args->monitor_lock);
	monitor->args->stop_simulation = 1;
	pthread_mutex_unlock(&monitor->args->monitor_lock);
	return (1);
}

void	*ft_monitoring(void *monitor_void)
{
	t_monitoring	*monitor;
	struct timeval	current_time;
	int				i;

	monitor = (t_monitoring *)monitor_void;
	while (1)
	{
		i = 0;
		gettimeofday(&current_time, NULL);
		while (i < monitor->args->number_of_philo)
		{
			if (ft_die_philo(monitor, &monitor->philo[i], current_time) == 1)
				return (NULL);
			i++;
		}
		if (ft_all_eating(monitor) == 1)
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}
