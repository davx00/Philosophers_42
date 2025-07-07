/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_aux.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: despanol <despanol@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 12:18:08 by despanol          #+#    #+#             */
/*   Updated: 2025/07/02 20:13:21 by despanol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sch/philosophers.h"

int	init_simulation(t_each_philo **philos, t_monitoring *monitor,
		char **argv, int argc)
{
	*philos = init_philo(argv, argc);
	if (!*philos)
		return (1);
	monitor->args = (*philos)->args;
	monitor->philo = *philos;
	return (0);
}

int	create_threads(t_each_philo *philos, t_monitoring *monitor)
{
	int	i;

	i = 0;
	if (pthread_create(&monitor->monitor, NULL, ft_monitoring, monitor))
	{
		cleanup_resources(philos);
		return (1);
	}
	while (i < philos->args->number_of_philo)
	{
		if (pthread_create(&philos[i].thread_philo_id, NULL, eat, &philos[i]))
		{
			pthread_mutex_lock(&philos->args->monitor_lock);
			philos->args->stop_simulation = 1;
			pthread_mutex_unlock(&philos->args->monitor_lock);
			pthread_join(monitor->monitor, NULL);
			while (--i >= 0)
				pthread_join(philos[i].thread_philo_id, NULL);
			cleanup_resources(philos);
			return (1);
		}
		i++;
	}
	return (0);
}

void	wait_threads(t_each_philo *philos, t_monitoring *monitor)
{
	int	i;

	i = 0;
	pthread_join(monitor->monitor, NULL);
	while (i < philos->args->number_of_philo)
	{
		pthread_join(philos[i].thread_philo_id, NULL);
		i++;
	}
}
