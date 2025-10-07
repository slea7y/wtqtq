/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_processor2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 09:25:19 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/10/07 10:32:08 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/executor.h"
#include "../../../Includes/minishell.h"
#include "../../../Includes/parser.h"

void	heredoc_signals(void)
{
	struct sigaction	ft_sigint;
	struct sigaction	ft_sigquit;

	ft_bzero(&ft_sigint, sizeof(ft_sigint));
	ft_bzero(&ft_sigquit, sizeof(ft_sigquit));
	ft_sigint.sa_flags = 0;
	ft_sigint.sa_handler = print_new_prompt;
	ft_sigquit.sa_handler = SIG_IGN;
	sigaction(SIGINT, &ft_sigint, NULL);
	sigaction(SIGQUIT, &ft_sigquit, NULL);
}

// // ok
// /// @brief controls heredocs process. checks delimiter, creates tmp filename
// // opens file to write (write only and apped), reads until end
// /// @param delimiter
// /// @param delimiter_quoted
// /// @param ctx
// /// @return
static int	open_heredoc_tempfile(char *delimiter, t_heredoc_ctx *hdoc,
		char **temp_filename)
{
	*temp_filename = create_temp_file_interactive(delimiter);
	if (!*temp_filename)
		return (-1);
	hdoc->fd = open(*temp_filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (hdoc->fd == -1)
	{
		free(*temp_filename);
		*temp_filename = NULL;
		return (-1);
	}
	return (0);
}

static char	*cleanup_heredoc_and_return(t_heredoc_ctx *hdoc,
		char *temp_filename)
{
	close(hdoc->fd);
	if (g_signal_status == 1)
	{
		unlink(temp_filename);
		free(temp_filename);
		return (NULL);
	}
	return (temp_filename);
}

char	*handle_heredoc(char *delimiter, int delimiter_quoted,
		t_shell_ctx *ctx, t_cmd_node *cmd)
{
	char			*temp_filename;
	t_heredoc_ctx	hdoc;

	if (!delimiter)
		return (NULL);
	if (open_heredoc_tempfile(delimiter, &hdoc, &temp_filename) == -1)
		return (NULL);
	hdoc.delimiter = delimiter;
	hdoc.delimiter_quoted = delimiter_quoted;
	hdoc.ctx = ctx;
	heredoc_read_loop(&hdoc, cmd, temp_filename);
	return (cleanup_heredoc_and_return(&hdoc, temp_filename));
}
