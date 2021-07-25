/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmeeseek <kmeeseek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/14 22:14:38 by kmeeseek          #+#    #+#             */
/*   Updated: 2021/07/25 22:55:01 by kmeeseek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo.h"

int	error(char *str)
{
	printf("error: %s\n", str);
	return (1);
}

int	check_if_arg_is_num(int argc, char **argv)
{
	int	i;
	int	j;

	i = 1;
	while (i < argc)
	{
		j = 0;
		while (argv[i][j])
		{
			if (!ft_isdigit(argv[i][j++]))
				return (error("invalid argument"));
		}
		i++;
	}
	return (0);
}

void	write_args_to_struct(int argc, char **argv, t_all *all)
{
	all->param.num_of_ph = ft_atoi_long(argv[1]);
	all->param.time_to_die = ft_atoi_long(argv[2]);
	all->param.time_to_eat = ft_atoi_long(argv[3]);
	all->param.time_to_sleep = ft_atoi_long(argv[4]);
	if (argc == 6)
		all->param.num_of_times_ph_must_eat = ft_atoi_long(argv[5]);
	else
		all->param.num_of_times_ph_must_eat = -1;
	
}

int	check_arg_validity(t_all *all)
{
	if (all->param.num_of_ph == 0 || all->param.time_to_die == 0
		|| all->param.time_to_eat == 0 || all->param.time_to_sleep == 0
		|| all->param.num_of_times_ph_must_eat == 0)
		return (error("invalid argument"));
	return (0);
}

int	process_arguments(int argc, char **argv, t_all	*all)
{
	if (!(argc == 5 || argc == 6))
		return (error("invalid number of arguments"));
	if (check_if_arg_is_num(argc, argv))
		return (1);
	write_args_to_struct(argc, argv, all);
	if (check_arg_validity(all))
		return (1);
	return (0);
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
	long curr_time;
	long new_time;

	curr_time = get_time();
	new_time = curr_time;
	while (time_period > new_time - curr_time)
	{
		new_time = get_time();
		usleep(200);
	}
}

int	create_mutexes_and_ph(t_all *all)
{
	int		i;
	long	start_time;

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
	i = 0;
	if ((start_time = get_time()) == -1)
		return (1);
	while (i < all->param.num_of_ph)
	{
		all->ph[i].left_fork = &all->forks[i];
		all->ph[i].right_fork = &all->forks[(i + 1) %  all->param.num_of_ph];
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

void eating(t_ph *ph)
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
	printf("%li %d has taken a fork\n", curr_time , ph->id);
	printf("%li %d is eating\n", curr_time, ph->id);
	ph->last_eat_time =  get_time() - ph->start_time;
	pthread_mutex_unlock(ph->message);
	my_sleep(ph->param.time_to_eat);
	pthread_mutex_unlock(ph->left_fork);
	pthread_mutex_unlock(ph->right_fork);
	ph->meal_num++;
}

void sleeping(t_ph *ph)
{
	long	curr_time;

	curr_time = (get_time() - ph->start_time);
	pthread_mutex_lock(ph->message);
	printf("%li %d is sleeping\n", curr_time, ph->id);
	pthread_mutex_unlock(ph->message);
	my_sleep(ph->param.time_to_sleep);
}

void thinking(t_ph *ph)
{
	long	curr_time;

	curr_time = (get_time() - ph->start_time);
	pthread_mutex_lock(ph->message);
	printf("%li %d is thinking\n", curr_time , ph->id);
	pthread_mutex_unlock(ph->message);
}

void *action(void *arg)
{
	t_ph	*ph;
	int		i;

	ph = (t_ph *)arg;
	
	if (ph->id % 2 != 0)
		usleep (1000);
	i = 0;
	while (ph->param.num_of_times_ph_must_eat == -1 || (ph->param.num_of_times_ph_must_eat > 0 && i < ph->param.num_of_times_ph_must_eat))
	{
		eating(ph);
		sleeping(ph);
		thinking(ph);
		i++;
	}
	return (0);
}

void	*if_dead(void *arg)
{
	int i;
	int j;
	t_all *all;
	long curr_time;

	all = (t_all *)arg;
	all->full = 0;
	i = 0;
	while (1)
	{
		j = 0;
		while (j < all->param.num_of_ph)
		{
			if (all->ph[j].meal_num == all->param.num_of_times_ph_must_eat && !all->ph[j].set)
			{
				all->ph[j].set = 1;
				all->full++;
			}
			curr_time = get_time() - all->ph[j].start_time;
			if (all->ph[j].last_eat_time + all->ph->param.time_to_die < curr_time)
			{
				printf("%ld %d died\n", curr_time, all->ph[j].id);
				while (i < all->param.num_of_ph)
				{
					pthread_detach(all->ph[i].philo);
					i++;
				}
				return (0);
			}
			j++;
		}
		if (all->full == all->param.num_of_ph)
			break;
	}
	return (0);
}

int	create_treads(t_all *all)
{
	int i;

	i = 0;
	if (pthread_create(&all->death, NULL, if_dead, all))
		return(0);
	while (i < all->param.num_of_ph)
	{
		pthread_create(&all->ph[i].philo, NULL, action, &all->ph[i]);
		i++;
		usleep (100);
	}
	i = 0;
	pthread_join(all->death, NULL);
	// while (i < all->param.num_of_ph)
	// {
	// 	pthread_join(all->ph[i].philo, NULL);
	// 	i++;
	// }
	i = 0;
	return (0);
}

void destroy_everything(t_all *all)
{
	int i;

	i = 0;
	while (i < all->param.num_of_ph)
	{
		pthread_mutex_destroy(&all->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&all->message);
	i = 0;

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