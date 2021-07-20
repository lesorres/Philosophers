/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmeeseek <kmeeseek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/14 22:14:38 by kmeeseek          #+#    #+#             */
/*   Updated: 2021/07/20 23:37:23 by kmeeseek         ###   ########.fr       */
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

int	create_mutexes_and_ph(t_all *all)
{
	int	i;

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
	while (i < all->param.num_of_ph)
	{
		all->ph[i].left_fork = &all->forks[i];
		all->ph[i].right_fork = &all->forks[(i + 1) %  all->param.num_of_ph];
		all->ph[i].message = &all->message;
		all->ph[i].id = i;
		printf("id = %i\n", all->ph[i].id);
		all->ph[i].param = all->param;
		i++;
	}
	return (0);
}

void eating(t_ph *ph)
{
	// printf("here\n");
	pthread_mutex_lock(ph->left_fork);
	pthread_mutex_lock(ph->message);
	printf("time %d  has taken a fork\n", ph->id);
	pthread_mutex_unlock(ph->message);
	usleep (1000);
	pthread_mutex_lock(ph->right_fork);
	pthread_mutex_lock(ph->message);
	printf("time %d  has taken a fork\n", ph->id);
	printf("time %d  is eating\n", ph->id);
	pthread_mutex_unlock(ph->message);
	pthread_mutex_unlock(ph->left_fork);
	pthread_mutex_unlock(ph->right_fork);
	usleep(ph->param.time_to_eat);
}

void *action(void *arg)
{
	printf("here2\n");
	t_ph *ph;

	ph = (t_ph *)arg;
	while (1)
		eating(ph);
	return (0);
}

int	create_treads(t_all *all)
{
	int i;

	i = 0;
	printf("here1\n");
	while (i < all->param.num_of_ph)
	{
		printf("here3\n");
		pthread_create(&all->ph[i].philo, NULL, action, &all->ph[i]);
		pthread_join(all->ph[i].philo, NULL);
		printf("here4\n");
		i++;
	}
	i = 0;
	return (0);
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
	return (0);
}