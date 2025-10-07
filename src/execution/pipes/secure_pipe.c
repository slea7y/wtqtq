/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   secure_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 08:21:51 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/10/07 11:16:24 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/executor.h"
#include "../../../Includes/minishell.h"
#include "../../../Includes/parser.h"

/// @brief Only creates a pipe if there's a next command (current->next)
// If it's the last command, no pipe is needed
// Returns 0 on pipe failure, 1 on success
int	correct_pipe(t_cmd_node *current, int pipe_fd[2])
{
	if (current->next && pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (0);
	}
	return (1);
}

int	update_pipe(t_cmd_node *current, int pipe_fd[2], int prev_pipe)
{
	if (prev_pipe != -1)
		close(prev_pipe);
	if (current->next)
	{
		close(pipe_fd[1]);
		return (pipe_fd[0]);
	}
	return (-1);
}

int	detect_pipes(t_cmd_list *cmds)
{
	t_cmd_node	*current;

	current = cmds->head;
	if (!cmds || !cmds->head)
		return (0);
	while (current)
	{
		if (current->next)
			return (1);
		current = current->next;
	}
	return (0);
}
