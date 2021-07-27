/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmeeseek <kmeeseek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/14 22:14:38 by kmeeseek          #+#    #+#             */
/*   Updated: 2021/07/26 21:19:51 by kmeeseek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_ph_death(t_all *all, int j)
{
	int		i;
	long	curr_time;

	i = 0;
	curr_time = get_time() - all->ph[j].start_time;
	if (all->ph[j].last_eat_time + all->ph->param.time_to_die \
	< curr_time)
	{
		pthread_mutex_lock(&all->message);
		printf("%ld %d died\n", curr_time, all->ph[j].id);
		while (i < all->param.num_of_ph)
		{
			pthread_detach(all->ph[i].philo);
			i++;
		}
		return (1);
		all->die = 1;
	}
	return (0);
}

void	*if_dead(void *arg)
{
	int		j;
	t_all	*all;

	all = (t_all *)arg;
	while (1)
	{
		j = 0;
		while (j < all->param.num_of_ph)
		{
			if (all->ph[j].meal_num == all->param.num_of_times_ph_must_eat \
			&& !all->ph[j].set)
			{
				all->ph[j].set = 1;
				all->full++;
			}
			if (check_ph_death(all, j))
				return (0);
			j++;
		}
		if (all->full == all->param.num_of_ph)
			break ;
	}
	return (0);
}

int	create_treads(t_all *all)
{
	int	i;

	i = 0;
	if (pthread_create(&all->death, NULL, if_dead, all))
		return (0);
	while (i < all->param.num_of_ph)
	{
		pthread_create(&all->ph[i].philo, NULL, action, &all->ph[i]);
		i++;
	}
	pthread_join(all->death, NULL);
	return (0);
}

void	destroy_everything(t_all *all)
{
	int	i;

	i = 0;
	while (i < all->param.num_of_ph)
	{
		pthread_mutex_destroy(&all->forks[i]);
		i++;
	}
	if (all->die == 1)
		pthread_mutex_unlock(&all->message);
	pthread_mutex_destroy(&all->message);
	i = 0;
	free (all->forks);
	free (all->ph);
}

int	main(int argc, char **argv)
{
	t_all	all;

	if (process_arguments(argc, argv, &all))
		return (1);
	if (create_mutexes_and_ph(&all))
		return (1);
	if (create_treads(&all))
		return (1);
	destroy_everything(&all);
	return (0);
}
