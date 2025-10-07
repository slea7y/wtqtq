/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 05:25:32 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/10/07 09:32:03 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/executor.h"
#include "../../../Includes/minishell.h"
#include "../../../Includes/parser.h"

// ok
/// @brief interactive mode. readline: reads line unitl pressed enter,
// displays "> " and waits for input, returns line value,  allows signals
/// @param
/// @return
char	*read_line_interactive(void)
{
	char	*line;

	write (1, "> ", 2);
	line = get_next_line(STDIN_FILENO);
	return (line);
}

// ok
/// @brief simple condition fction bases ond if input is interactive
// by isatty
/// @param is_interactive
/// @return
char	*read_heredoc_line(int is_interactive)
{
	(void)is_interactive;
	return (read_line_interactive());
}

// ok
/// @brief checks if delimiter quoted -> no expansion, writes line in fd file
/// @param fd
/// @param line
/// @param delimiter_quoted
/// @param ctx
/// @return
int	write_line_to_file(int fd, char *line, int delimiter_quoted,
		t_shell_ctx *ctx)
{
	char	*expanded_line;

	if (delimiter_quoted)
		expanded_line = ft_strdup(line);
	else
		expanded_line = expand_string_variables(line, ctx);
	if (!expanded_line)
		return (-1);
	write(fd, expanded_line, ft_strlen(expanded_line));
	free(expanded_line);
	return (0);
}
