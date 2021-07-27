/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_and_init.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmeeseek <kmeeseek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/26 20:47:33 by kmeeseek          #+#    #+#             */
/*   Updated: 2021/07/26 21:22:25 by kmeeseek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	error(char *str)
{
	printf("error: %s\n", str);
	return (1);
}

long	get_time(void)
{
	struct timeval	time;
	long			ret;

	if (gettimeofday(&time, NULL))
		return (error ("gettimeofday error"));
	ret = time.tv_sec * 1000 + time.tv_usec / 1000;
	return (ret);
}

void	my_sleep(long time_period)
{
	long	curr_time;
	long	new_time;

	curr_time = get_time();
	new_time = curr_time;
	while (time_period > new_time - curr_time)
	{
		new_time = get_time();
		usleep(200);
	}
}

int	ph_init(t_all *all)
{
	int		i;
	long	start_time;

	start_time = get_time();
	if (start_time == -1)
		return (1);
	i = 0;
	while (i < all->param.num_of_ph)
	{
		all->ph[i].left_fork = &all->forks[i];
		all->ph[i].right_fork = &all->forks[(i + 1) % all->param.num_of_ph];
		all->ph[i].message = &all->message;
		all->ph[i].id = i + 1;
		all->ph[i].param = all->param;
		all->ph[i].start_time = start_time;
		all->ph[i].last_eat_time = 0;
		all->ph[i].meal_num = 0;
		all->ph[i].set = 0;
		i++;
	}
	return (0);
}

int	create_mutexes_and_ph(t_all *all)
{
	int		i;

	i = 0;
	all->forks = malloc(sizeof(pthread_mutex_t) * all->param.num_of_ph);
	if (!all->forks)
		return (error("mutexes memory allocation error"));
	while (i < all->param.num_of_ph)
		pthread_mutex_init(&all->forks[i++], NULL);
	pthread_mutex_init(&all->message, NULL);
	all->ph = malloc(sizeof(t_ph) * all->param.num_of_ph);
	if (!all->ph)
		return (error("mutexes memory allocation error"));
	all->full = 0;
	all->die = 0;
	if (ph_init(all))
		return (1);
	return (0);
}
