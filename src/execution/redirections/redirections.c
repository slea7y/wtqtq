/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 18:20:19 by majkijew          #+#    #+#             */
/*   Updated: 2025/10/07 08:33:03 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/executor.h"
#include "../../../Includes/minishell.h"
#include "../../../Includes/parser.h"

void	rewrite_heredoc_nodes(t_cmd_node *cmd, t_file_node *last_hd,
		char *temp_filname)
{
	t_file_node	*iter;

	if (!cmd || !last_hd || !temp_filname)
		return ;
	iter = cmd->files->head;
	while (iter)
	{
		if (iter->redir_type == HEREDOC)
		{
			iter->redir_type = INFILE;
			if (iter != last_hd)
				iter->filename = ft_strdup("/dev/null");
			else
				iter->filename = temp_filname;
		}
		iter = iter->next;
	}
}

static int	handle_input_redir(t_file_node *file)
{
	int	fd;

	fd = open(file->filename, O_RDONLY);
	unlink(file->filename);
	if (fd == -1)
	{
		ft_putstr_fd(file->filename, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		return (1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

static int	handle_output_redir(t_file_node *file)
{
	int	fd;

	fd = open(file->filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		ft_putstr_fd(file->filename, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		return (1);
	}
	if (file->filename && ft_strncmp(file->filename, "./heredoc_", 10) == 0)
		unlink(file->filename);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

static int	handle_append_redir(t_file_node *file)
{
	int	fd;

	fd = open(file->filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (fd == -1)
	{
		ft_putstr_fd(file->filename, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		return (1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	handle_redirections(t_cmd_node *cmd)
{
	t_file_node	*file;

	if (!cmd || !cmd->files)
		return (0);
	file = cmd->files->head;
	while (file)
	{
		if (!valid_filename(file->filename))
		{
			ft_putstr_fd(file->filename, 2);
			ft_putstr_fd(": ambiguous redirect\n", 2);
			return (1);
		}
		if (file->redir_type == INFILE && handle_input_redir(file))
			return (1);
		else if (file->redir_type == OUTFILE && handle_output_redir(file))
			return (1);
		else if (file->redir_type == OUTFILE_APPEND
			&& handle_append_redir(file))
			return (1);
		file = file->next;
	}
	return (0);
}
