/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 09:01:22 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/10/07 09:01:43 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/executor.h"
#include "../Includes/minishell.h"
#include "../Includes/parser.h"
// #include <signal.h>
// #include <sys/ioctl.h>
// #include <sys/stat.h>
// #include <sys/wait.h>

int	was_signal_received(void)
{
	return (g_signal_status);
}

void	reset_signal_status(void)
{
	g_signal_status = 0;
}

void	set_signal_status(int status)
{
	g_signal_status = status;
}
