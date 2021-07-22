/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmeeseek <kmeeseek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/14 22:14:38 by kmeeseek          #+#    #+#             */
/*   Updated: 2021/07/22 23:20:44 by kmeeseek         ###   ########.fr       */
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

// long	get_time(void)
// {
// 	struct timeval	time;
// 	long			ret;

// 	if (gettimeofday(&time, NULL))
// 		return (error ("gettimeofday error"));
// 	ret = time.tv_sec * 1000 + time.tv_usec / 1000;
// 	return (ret);
// }

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
		usleep(1);
	}
}

// void    my_sleep(long tim)
// {
//     struct timeval  t1;
//     ssize_t         timing;
//     gettimeofday(&t1, NULL);
//     timing = t1.tv_sec * 1000 + t1.tv_usec / 1000;
//     while (tim > (t1.tv_sec * 1000 + t1.tv_usec / 1000) - timing)
//     {
//         gettimeofday(&t1, NULL);
//         // usleep(1);
//     }
// }

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
	// printf ("here1\n");
	if ((start_time = get_time()) == -1)
		return (1);
	all->ph = malloc(sizeof(t_ph) * all->param.num_of_ph);
	if (!all->ph)
		return (error("mutexes memory allocation error"));
	i = 0;
	while (i < all->param.num_of_ph)
	{
		all->ph[i].left_fork = &all->forks[i];
		all->ph[i].right_fork = &all->forks[(i + 1) %  all->param.num_of_ph];
		all->ph[i].message = &all->message;
		all->ph[i].id = i + 1;
		// printf("id = %i\n", all->ph[i].id);
		all->ph[i].param = all->param;
		all->ph[i].start_time = start_time;
		all->ph[i].last_eat_time = 0;
		i++;
	}
	return (0);
}

void eating(t_ph *ph)
{
	long	curr_time;

	// printf("here\n");
	pthread_mutex_lock(ph->left_fork);
	pthread_mutex_lock(ph->right_fork);

	curr_time = (get_time() - ph->start_time);
	pthread_mutex_lock(ph->message);
	printf("%li %d has taken a fork\n", curr_time, ph->id);
	printf("%li %d has taken a fork\n", curr_time , ph->id);
	printf("%li %d is eating\n", curr_time, ph->id);
	pthread_mutex_unlock(ph->message);
	// usleep (1000);
	// curr_time = get_time();
	// usleep(ph->param.time_to_eat);
	my_sleep(ph->param.time_to_eat);
	pthread_mutex_unlock(ph->left_fork);
	pthread_mutex_unlock(ph->right_fork);
	ph->last_eat_time = curr_time + ph->param.time_to_eat;
}

	// long	curr_time;

	// // printf("here\n");
	// pthread_mutex_lock(ph->left_fork);
	// curr_time = get_time();
	// pthread_mutex_lock(ph->message);
	// printf("%li %d has taken a fork\n", curr_time - ph->start_time, ph->id);
	// pthread_mutex_unlock(ph->message);
	// // usleep (1000);
	// pthread_mutex_lock(ph->right_fork);
	// curr_time = get_time();
	// pthread_mutex_lock(ph->message);
	// printf("%li %d has taken a fork\n", curr_time - ph->start_time, ph->id);
	// printf("%li %d is eating\n", curr_time - ph->start_time, ph->id);
	// pthread_mutex_unlock(ph->message);
	// usleep(ph->param.time_to_eat);
	// pthread_mutex_unlock(ph->left_fork);
	// pthread_mutex_unlock(ph->right_fork);
	// ph->last_eat_time = curr_time - ph->start_time + ph->param.time_to_eat;

void sleeping(t_ph *ph)
{
	long	curr_time;

	curr_time = (get_time() - ph->start_time);
	pthread_mutex_lock(ph->message);
	// printf("%li %d is sleeping\n", ph->last_eat_time / 1000, ph->id);
	printf("%li %d is sleeping\n", curr_time, ph->id);
	pthread_mutex_unlock(ph->message);
	// usleep(ph->param.time_to_sleep);
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
	// printf("here2\n");
	t_ph *ph;

	ph = (t_ph *)arg;
	if (ph->id % 2 == 0)
		usleep (1000);
	// printf("ph.id = %i\n", ph->id);
	while (1)
	{
		eating(ph);
		sleeping(ph);
		thinking(ph);
	}
	return (0);
}

void	if_dead(t_all *all)
{
	int i;

	i = 0;
	while (i < all->param.num_of_ph)
	{
		if ((get_time() - all->ph->start_time - all->ph[i].last_eat_time) > all->param.time_to_die)
		{
			pthread_mutex_lock(&all->message);
			printf("%li %d died\n", (get_time() - all->ph->start_time) , i + 1);
			pthread_mutex_unlock(&all->message);
			exit (1);
		}
		i++;
	}
	// return (0);
}

int	create_treads(t_all *all)
{
	int i;

	i = 0;
	// printf("here1\n");
	while (i < all->param.num_of_ph)
	{
		// printf("here3\n");
		pthread_create(&all->ph[i].philo, NULL, action, &all->ph[i]);
		// printf("%ld\n %ld\n %ld\n %ld\n %ld\n", all->param.num_of_ph, all->param.time_to_die, all->param.time_to_eat, all->param.time_to_sleep, all->param.num_of_times_ph_must_eat);
		// printf("ph.id = %i", all->ph[i].id);
		i++;
		// printf("here4\n");
	}
	i = 0;
	if_dead(all);
	{
		while (i < all->param.num_of_ph)
		{
			pthread_detach(all->ph[i].philo);
			i++;
		}
		return (1);
	}
	while (i < all->param.num_of_ph)
	{
		pthread_join(all->ph[i].philo, NULL);
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