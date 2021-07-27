/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmeeseek <kmeeseek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/14 22:20:43 by kmeeseek          #+#    #+#             */
/*   Updated: 2021/07/26 21:20:04 by kmeeseek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_param
{
	long	num_of_ph;
	long	time_to_die;
	long	time_to_eat;
	long	time_to_sleep;
	long	num_of_times_ph_must_eat;
}				t_param;

typedef struct s_ph
{
	int				id;
	pthread_t		philo;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*message;
	pthread_mutex_t	*start;
	t_param			param;
	long			start_time;
	long			last_eat_time;
	long			meal_num;
	int				set;
}			t_ph;

typedef struct s_all
{
	t_param			param;
	t_ph			*ph;
	pthread_mutex_t	*forks;
	pthread_mutex_t	message;
	pthread_t		death;
	int				full;
	int				die;

}				t_all;

int		ft_isdigit(int c);
long	ft_atoi_long(char *str);
int		error(char *str);
int		check_if_arg_is_num(int argc, char **argv);
void	write_args_to_struct(int argc, char **argv, t_all *all);
int		check_arg_validity(t_all *all);
int		process_arguments(int argc, char **argv, t_all	*all);
long	get_time(void);
void	my_sleep(long time_period);
int		ph_init(t_all *all);
int		create_mutexes_and_ph(t_all *all);
void	eating(t_ph *ph);
void	sleeping(t_ph *ph);
void	thinking(t_ph *ph);
void	*action(void *arg);

#endif