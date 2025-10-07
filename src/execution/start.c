/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 14:34:07 by majkijew          #+#    #+#             */
/*   Updated: 2025/10/07 10:08:08 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/executor.h"

static int	execute_single_command(t_cmd_node *current, t_shell_ctx *ctx)
{
	int	exit_code;

	if (process_heredoc(current, ctx) != 0)
		return (1);
	if (current->cmd && current->cmd[0] && is_bulidin(current->cmd[0]))
		exit_code = execute_builtin_with_redirections(current, ctx);
	else if (current->cmd && current->cmd[0])
		exit_code = execute_single_external(current, ctx);
	else
	{
		if (current->files && current->files->head)
			exit_code = execute_single_external(current, ctx);
		else
			exit_code = 0;
	}
	return (exit_code);
}

int	start_execution(t_cmd_list *cmds, t_shell_ctx *ctx)
{
	t_cmd_node	*current;

	if (!cmds || !cmds->head)
		return (0);
	current = cmds->head;
	if (!detect_pipes(cmds))
		return (execute_single_command(current, ctx));
	else
		return (execute_pipes(cmds, ctx));
}
