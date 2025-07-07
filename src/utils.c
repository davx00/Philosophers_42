/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: despanol <despanol@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 13:39:34 by despanol          #+#    #+#             */
/*   Updated: 2025/07/02 20:44:09 by despanol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sch/philosophers.h"

int	ft_atoi(const char *str)
{
	int	sign;
	int	result;

	sign = 1;
	result = 0;
	while (*str == ' ' || *str == '\f' || *str == '\n' || *str == '\r'
		|| *str == '\t' || *str == '\v')
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + *str - '0';
		str++;
	}
	return (sign * result);
}

t_each_philo	*init_philo(char **argv, int argc)
{
	t_each_philo	*philo;
	t_gen_args		*args;
	int				i;

	i = 0;
	args = malloc(sizeof(t_gen_args));
	init_args(args, argv, argc);
	philo = malloc(sizeof(t_each_philo) * args->number_of_philo);
	while (i < args->number_of_philo)
	{
		philo[i].id = i + 1;
		philo[i].meals = 0;
		gettimeofday(&philo[i].last_meal, NULL);
		assign_forks(philo, i, args->number_of_philo);
		if (pthread_mutex_init(&philo[i].last_time_meal_mutex, NULL))
			perror("Error initializing last_time_meal_mutex");
		philo[i].args = args;
		i++;
	}
	return (philo);
}

void	init_args(t_gen_args *args, char **argv, int argc)
{
	int	i;

	i = 0;
	gettimeofday(&args->time_now, NULL);
	args->number_of_philo = ft_atoi(argv[1]);
	args->time_to_die = ft_atoi(argv[2]);
	args->time_to_eat = ft_atoi(argv[3]);
	args->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		args->meals = ft_atoi(argv[5]);
	else
		args->meals = -1;
	args->stop_simulation = 0;
	args->forks = malloc(sizeof(pthread_mutex_t) * args->number_of_philo);
	while (i < args->number_of_philo)
	{
		if (pthread_mutex_init(&args->forks[i], NULL))
			perror("Error in mutex init");
		i++;
	}
	if (pthread_mutex_init(&args->monitor_lock, NULL))
		perror("Error initializing monitor_lock");
	if (pthread_mutex_init(&args->print_lock, NULL))
		perror("Error initializing print_lock");
}

void	cleanup_resources(t_each_philo *philos)
{
	int	i;

	i = 0;
	if (!philos || !philos->args)
		return ;
	while (i < philos->args->number_of_philo)
	{
		pthread_mutex_destroy(&philos[i].last_time_meal_mutex);
		i++;
	}
	i = 0;
	while (i < philos->args->number_of_philo)
	{
		pthread_mutex_destroy(&philos->args->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&philos->args->monitor_lock);
	pthread_mutex_destroy(&philos->args->print_lock);
	free(philos->args->forks);
	free(philos->args);
	free(philos);
}

long	get_timestamp(struct timeval start_time)
{
	struct timeval	current_time;
	long			start_ms;
	long			current_ms;

	gettimeofday(&current_time, NULL);
	start_ms = start_time.tv_sec * 1000 + start_time.tv_usec / 1000;
	current_ms = current_time.tv_sec * 1000 + current_time.tv_usec / 1000;
	return (current_ms - start_ms);
}
