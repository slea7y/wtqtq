/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_p.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majkijew <majkijew@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 10:05:00 by majkijew          #+#    #+#             */
/*   Updated: 2025/10/07 12:42:37 by majkijew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/executor.h"
#include "../../../Includes/minishell.h"
#include "../../../Includes/parser.h"
#include <sys/stat.h>
#include <sys/wait.h>
#include <termios.h>

// when u call frok childproces gets it s own memory placce and
// apparentlly memory just gets cleaned by the os (wild)
// in child process memory is completlly separet form the parent
// so i should be free to just exit here :-DDD
void	handle_external_child(t_cmd_node *cmd, char **envp, t_shell_ctx *ctx)
{
	int	exit_code;

	exit_code = handle_redirections(cmd);
	if (exit_code != 0)
	{
		free_env(envp);
		exit_child(ctx, 1);
	}
	if (cmd->cmd && cmd->cmd[0])
		exit_code = execute_cmd(cmd, envp, ctx);
	else
		exit_code = 0;
	free_env(envp);
	exit_child(ctx, exit_code);
}

// after child process has been created and exectued parents needs to wait
// for child to finish, get childs exit status, clean up resorces, return the
// desired exit B-)
// the &status thingi is where the child termination info is getting stored
// and the 0 after that means "wait for any child state to change" (normal
// terimantion, siganls etc.)
// WIFEXITED(status) =did the child exit normally? if YES WIFEXITED(status)
// extracts the actual exit code
// WIFSIGNALED(status== was the child killed by the signal? (rip baby)
// if yes WTERMSIG(status) is the signal number
// esle some other error so exit should be 1
int	handle_parent_proc(pid_t pid_try)
{
	int	exit_code;
	int	status;

	waitpid(pid_try, &status, 0);
	if (WIFEXITED(status))
		exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
		{
			exit_code = 130;
			set_signal_status(130);
		}
		else
		{
			exit_code = 128 + WTERMSIG(status);
			set_signal_status(exit_code);
		}
	}
	else
		exit_code = 1;
	return (exit_code);
}

static void	setup_parent_signals(struct sigaction *old_int,
		struct sigaction *old_quit)
{
	struct sigaction	ign;

	sigaction(SIGINT, NULL, old_int);
	sigaction(SIGQUIT, NULL, old_quit);
	ign.sa_handler = SIG_IGN;
	sigemptyset(&ign.sa_mask);
	ign.sa_flags = 0;
	sigaction(SIGINT, &ign, NULL);
	sigaction(SIGQUIT, &ign, NULL);
}

static void	run_child_process(t_shell_ctx *ctx, char **envp,
		struct sigaction *old_int, struct sigaction *old_quit)
{
	sigaction(SIGINT, old_int, NULL);
	sigaction(SIGQUIT, old_quit, NULL);
	setup_heredoc_and_child_sig(0);
	handle_external_child(ctx->cmd_list->head, envp, ctx);
}

static int	run_parent_process(pid_t pid, char **envp,
		struct sigaction *old_int, struct sigaction *old_quit)
{
	int	exit_code;

	exit_code = handle_parent_proc(pid);
	if (was_signal_received())
	{
		exit_code = was_signal_received();
		reset_signal_status();
	}
	sigaction(SIGINT, old_int, NULL);
	sigaction(SIGQUIT, old_quit, NULL);
	free_env(envp);
	return (exit_code);
}

int	execute_single_external(t_cmd_node *cmd, t_shell_ctx *ctx)
{
	pid_t				pid_try;
	char				**envp;
	struct sigaction	old_quit;
	struct sigaction	old_int;

	if (!cmd || !ctx)
		return (1);
	envp = env_to_array(ctx->env);
	if (!envp)
		return (1);
	setup_parent_signals(&old_int, &old_quit);
	pid_try = fork();
	if (pid_try < 0)
	{
		perror("fork");
		free_env(envp);
		sigaction(SIGINT, &old_int, NULL);
		sigaction(SIGQUIT, &old_quit, NULL);
		return (1);
	}
	if (pid_try == 0)
		run_child_process(ctx, envp, &old_int, &old_quit);
	return (run_parent_process(pid_try, envp, &old_int, &old_quit));
}
