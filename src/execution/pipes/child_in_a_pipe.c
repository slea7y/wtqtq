/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_in_a_pipe.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 01:29:40 by majkijew          #+#    #+#             */
/*   Updated: 2025/10/07 10:27:27 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/executor.h"
#include "../../../Includes/minishell.h"
#include "../../../Includes/parser.h"

int	handle_pipe_redirections(t_cmd_node *cmd, int prev_pipe, int pipe_fd[2])
{
	if (prev_pipe != -1)
	{
		if (dup2(prev_pipe, STDIN_FILENO) == -1)
		{
			perror("dup2 stdin");
			return (1);
		}
	}
	if (cmd->next)
	{
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		{
			perror("dup2 stdout");
			return (1);
		}
		close(pipe_fd[0]);
	}
	return (0);
}

void	exit_child(t_shell_ctx *ctx, int exit_code)
{
	if (ctx->token_list)
		free_token_list(ctx->token_list);
	if (ctx->env)
		free_env_list(ctx->env);
	if (ctx->cmd_list)
		free_cmd_list(ctx->cmd_list);
	exit(exit_code);
}

void	cmd_in_child(t_cmd_node *current, t_shell_ctx *ctx, char **envp)
{
	int	exit_code;

	if (current->cmd && current->cmd[0] && is_bulidin(current->cmd[0]))
	{
		free_env(envp);
		exit_code = execute_bulidins(current, ctx);
		exit_child(ctx, exit_code);
	}
	else
	{
		exit_code = execute_cmd(current, envp, ctx);
		free_env(envp);
		exit_child(ctx, exit_code);
	}
}

// we have to reset singals in a child process cuz othervise it might
// cose unexpected behavior
// here i am alredy in a child proc so i am freee to exit
void	prep_child_for_exec(t_cmd_node *current, t_shell_ctx *ctx,
		int prev_pipe, int pipe_fd[2])
{
	char	**envp;
	int		exit_code;

	setup_heredoc_and_child_sig(0);
	if (process_heredoc(current, ctx) != 0)
		exit_child(ctx, 1);
	envp = env_to_array(ctx->env);
	if (!envp)
		exit_child(ctx, 1);
	exit_code = handle_pipe_redirections(current, prev_pipe, pipe_fd);
	if (exit_code != 0)
	{
		free_env(envp);
		exit_child(ctx, exit_code);
	}
	exit_code = handle_redirections(current);
	if (exit_code != 0)
	{
		free_env(envp);
		exit_child(ctx, exit_code);
	}
	cmd_in_child(current, ctx, envp);
}
