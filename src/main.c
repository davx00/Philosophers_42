/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: despanol <despanol@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 12:54:51 by despanol          #+#    #+#             */
/*   Updated: 2025/07/02 20:44:43 by despanol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sch/philosophers.h"

int	main(int argc, char **argv)
{
	t_each_philo	*philos;
	t_monitoring	monitor;

	if (check_args(argc, argv))
	{
		printf("Error while checking the arguments\n");
		return (0);
	}
	if (init_simulation(&philos, &monitor, argv, argc))
		return (1);
	if (create_threads(philos, &monitor))
		return (1);
	wait_threads(philos, &monitor);
	cleanup_resources(philos);
	return (0);
}

void	*eat(void *philos)
{
	t_each_philo	*philo;
	int				stop;

	philo = (t_each_philo *)philos;
	if (philo->id % 2 != 0)
		usleep(1000);
	while (1)
	{
		pthread_mutex_lock(&philo->args->monitor_lock);
		stop = philo->args->stop_simulation;
		pthread_mutex_unlock(&philo->args->monitor_lock);
		if (stop)
			break ;
		think(philo);
		execute_eating_action(philo);
		philosopher_sleep(philo);
	}
	return (NULL);
}

void	assign_forks(t_each_philo *philo, int i, int total_philo)
{
	philo[i].right_fork = i;
	if (total_philo == 1)
		philo[i].left_fork = -1;
	else if (i == 0)
		philo[i].left_fork = total_philo - 1;
	else
		philo[i].left_fork = i - 1;
}
