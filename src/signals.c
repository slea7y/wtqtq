/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 03:56:15 by majkijew          #+#    #+#             */
/*   Updated: 2025/10/07 11:21:16 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/executor.h"
#include "../Includes/minishell.h"
#include "../Includes/parser.h"
// #include <signal.h>
// #include <sys/ioctl.h>
// #include <sys/stat.h>
// #include <sys/wait.h>
#include <termios.h>

volatile sig_atomic_t	g_signal_status = 0;

static void	sigint_handler(int sig)
{
	(void)sig;
	g_signal_status = 130;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

static void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	g_signal_status = 130;
	write(1, "\n", 1);
}

static void	setup_signal_handler(int sig, void (*handler)(int), int flags)
{
	struct sigaction	sa;

	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = flags;
	if (sigaction(sig, &sa, NULL) == -1)
	{
		perror("sigaction");
		exit(1);
	}
}

void	start_signals(void)
{
	setup_signal_handler(SIGINT, sigint_handler, 0);
	setup_signal_handler(SIGQUIT, SIG_IGN, 0);
	setup_signal_handler(SIGTSTP, SIG_IGN, 0);
}

void	setup_heredoc_and_child_sig(int is_heredoc)
{
	if (is_heredoc)
	{
		setup_signal_handler(SIGINT, heredoc_sigint_handler, 0);
		setup_signal_handler(SIGQUIT, SIG_DFL, 0);
		setup_signal_handler(SIGTSTP, SIG_IGN, 0);
	}
	else
	{
		setup_signal_handler(SIGINT, SIG_DFL, 0);
		setup_signal_handler(SIGQUIT, SIG_IGN, 0);
		setup_signal_handler(SIGTSTP, SIG_DFL, 0);
	}
}
