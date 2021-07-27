/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmeeseek <kmeeseek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/26 20:42:01 by kmeeseek          #+#    #+#             */
/*   Updated: 2021/07/26 21:16:57 by kmeeseek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	eating(t_ph *ph)
{
	long	curr_time;

	if (ph->param.num_of_ph == 1)
	{
		curr_time = (get_time() - ph->start_time);
		printf("%li %d has taken a fork\n", curr_time, ph->id);
	}
	pthread_mutex_lock(ph->left_fork);
	pthread_mutex_lock(ph->right_fork);
	curr_time = (get_time() - ph->start_time);
	pthread_mutex_lock(ph->message);
	printf("%li %d has taken a fork\n", curr_time, ph->id);
	printf("%li %d has taken a fork\n", curr_time, ph->id);
	printf("%li %d is eating\n", curr_time, ph->id);
	ph->last_eat_time = get_time() - ph->start_time;
	pthread_mutex_unlock(ph->message);
	my_sleep(ph->param.time_to_eat);
	pthread_mutex_unlock(ph->left_fork);
	pthread_mutex_unlock(ph->right_fork);
	ph->meal_num++;
}

void	sleeping(t_ph *ph)
{
	long	curr_time;

	curr_time = (get_time() - ph->start_time);
	pthread_mutex_lock(ph->message);
	printf("%li %d is sleeping\n", curr_time, ph->id);
	pthread_mutex_unlock(ph->message);
	my_sleep(ph->param.time_to_sleep);
}

void	thinking(t_ph *ph)
{
	long	curr_time;

	curr_time = (get_time() - ph->start_time);
	pthread_mutex_lock(ph->message);
	printf("%li %d is thinking\n", curr_time, ph->id);
	pthread_mutex_unlock(ph->message);
}

void	*action(void *arg)
{
	t_ph	*ph;
	int		i;

	ph = (t_ph *)arg;
	if (ph->id % 2 != 0)
		usleep (1000);
	i = 0;
	while (ph->param.num_of_times_ph_must_eat == -1 \
	|| (ph->param.num_of_times_ph_must_eat > 0 \
	&& i < ph->param.num_of_times_ph_must_eat))
	{
		eating(ph);
		sleeping(ph);
		thinking(ph);
		i++;
	}
	return (0);
}
