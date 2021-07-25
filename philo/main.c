/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmeeseek <kmeeseek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/14 22:14:38 by kmeeseek          #+#    #+#             */
/*   Updated: 2021/07/25 21:41:33 by kmeeseek         ###   ########.fr       */
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
	// pthread_mutex_init(&all->start, NULL);
	// printf ("here1\n");
	all->ph = malloc(sizeof(t_ph) * all->param.num_of_ph);
	if (!all->ph)
		return (error("mutexes memory allocation error"));
	i = 0;
	if ((start_time = get_time()) == -1)
		return (1);
	// all->full = 0;
	while (i < all->param.num_of_ph)
	{
		all->ph[i].left_fork = &all->forks[i];
		all->ph[i].right_fork = &all->forks[(i + 1) %  all->param.num_of_ph];
		all->ph[i].message = &all->message;
		all->ph[i].id = i + 1;
		all->ph[i].param = all->param;
		all->ph[i].start_time = start_time;
		all->ph[i].last_eat_time = 0;
		// all->ph[i].start = &all->start;
		// all->ph[i].dead = (all->dead);
		// all->ph[i].dead2 = 0;
		i++;
	}
	return (0);
}

// int new_printf(t_ph *ph, long p1, int p2, char *str)
// {
// 	if (*(ph->dead) > 0)
// 	{
// 		pthread_mutex_unlock(ph->message);
// 		return (1);
// 	}
// 	printf("%li %d %s\n", p1, p2, str);
// 	return (0);
// }

void eating(t_ph *ph)
{
	long	curr_time;

	pthread_mutex_lock(ph->left_fork);
	pthread_mutex_lock(ph->right_fork);

	curr_time = (get_time() - ph->start_time);
	pthread_mutex_lock(ph->message);
	printf("%li %d has taken a fork\n", curr_time, ph->id);
	printf("%li %d has taken a fork\n", curr_time , ph->id);
	printf("%li %d is eating\n", curr_time, ph->id);
	ph->last_eat_time =  get_time() - ph->start_time;
	// ph->last_eat_time = curr_time;
	// if (ph->id == 1)
	// 	printf("last_eat_time1 = %ld\n", ph->last_eat_time);
	pthread_mutex_unlock(ph->message);
	// usleep (1000);
	// curr_time = get_time();
	// usleep(ph->param.time_to_eat);
	my_sleep(ph->param.time_to_eat);
	// ph->last_eat_time = (get_time() - ph->start_time) + ph->param.time_to_eat;
	// ph->last_eat_time = curr_time + ph->param.time_to_eat;
	pthread_mutex_unlock(ph->left_fork);
	pthread_mutex_unlock(ph->right_fork);
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
	// long	death_time;

	ph = (t_ph *)arg;
	
	// if (ph->id == ph->param.num_of_ph)
	// 	pthread_mutex_unlock(ph->start);
	// else
	// 	pthread_mutex_lock(ph->start);
	
	// while (ph->id == ph->param.num_of_ph)
	// 	1 == 1;
	if (ph->id % 2 != 0)
		usleep (1000);
	i = 0;
	while (ph->param.num_of_times_ph_must_eat == -1 || (ph->param.num_of_times_ph_must_eat > 0 && i < ph->param.num_of_times_ph_must_eat))
	{
		// death_time = get_time() - ph->start_time - ph->last_eat_time + 1;
		// printf("%li %d death_time\n", death_time , ph->id);
		// if (death_time > ph->param.time_to_die)
		// {
		// 	pthread_mutex_lock(ph->message);
		// 	*(ph->dead) = death_time;
		// 	printf("%li %d died\n", death_time , ph->id);
		// 	return(0);
		// }
		eating(ph);
		sleeping(ph);
		thinking(ph);
		i++;
	}
	return (0);
}

// void	*if_dead(void *arg)
// {
// 	int i;
// 	t_all *all;

// 	all = (t_all *)arg;
// 	*(all->dead) = 0;
// 	i = 0;
// 	while (1)
// 	{
// 		if (*(all->dead))
// 		{
// 			while (i < all->param.num_of_ph)
// 			{
// 				pthread_detach(all->ph[i].philo);
// 				i++;
// 			}
// 			return (0);
// 		}
// 	}
// 	return (0);
// }

void		ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

void			ft_putnbr_fd(int n, int fd)
{
	long int	k;

	k = n;
	if (k < 0)
	{
		k = k * (0 - 1);
		ft_putchar_fd('-', fd);
	}
	if (k >= 10)
	{
		ft_putnbr_fd((k / 10), fd);
		ft_putnbr_fd((k % 10), fd);
	}
	else
		ft_putchar_fd((k + 48), fd);
}

void	*if_dead(void *arg)
{
	int i;
	int j;
	t_all *all;
	long curr_time;

	all = (t_all *)arg;
	// *(all->dead) = 0;
	i = 0;
	while (1)
	{
		j = 0;
		while (j < all->param.num_of_ph)
		{
			// if (all->full == all->param.num_of_ph)
			// 	break;
			curr_time = get_time() - all->ph[j].start_time;
			if (all->ph[j].last_eat_time + all->ph->param.time_to_die < curr_time)
			{
				// pthread_mutex_lock(&all->message);
				printf("%ld %d died\n", curr_time, all->ph[j].id);
				while (i < all->param.num_of_ph)
				{
					pthread_detach(all->ph[i].philo);
					i++;
				}
				// pthread_mutex_unlock(&all->message);
				return (0);
			}
			j++;
		}
		// if (all->full == all->param.num_of_ph)
		// 	break;
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
	// if_dead(all);
	// {
	// 	while (i < all->param.num_of_ph)
	// 	{
	// 		pthread_detach(all->ph[i].philo);
	// 		i++;
	// 	}
	// 	return (1);
	// }
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
	// destroy_everything(&all);
	return (0);
}