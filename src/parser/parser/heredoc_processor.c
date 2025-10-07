/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_processor.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maja <maja@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 04:04:12 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/10/07 16:51:33 by maja             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/executor.h"
#include "../../../Includes/minishell.h"
#include "../../../Includes/parser.h"

// ok
/// @brief returns individual filename with counter for each heredoc
/// @param counter
/// @return
static char	*get_temp_filename(int counter)
{
	char	*num_str;
	char	*filename;

	num_str = ft_itoa(counter);
	if (!num_str)
		return (NULL);
	filename = ft_strjoin("./heredoc_", num_str);
	if (!filename)
	{
		free(num_str);
		return (NULL);
	}
	free(num_str);
	return (filename);
}

// ok
/// @brief checks for availible filename, creates fd for test. creates file
// flags: create if not there, write only, deletes old content, permissions
/// @param delimiter
/// @return
char	*create_temp_file_interactive(char *delimiter)
{
	char	*temp_filename;
	int		fd;
	int		counter;

	(void)delimiter;
	counter = 0;
    while (counter < 1000)
	{
		temp_filename = get_temp_filename(counter);
		if (!temp_filename)
			return (NULL);
        fd = open(temp_filename, O_CREAT | O_EXCL | O_WRONLY, 0600);
        if (fd != -1)
		{
			close(fd);
			return (temp_filename);
		}
		free(temp_filename);
		counter++;
	}
	return (NULL);
}

// ok
/// @brief cheks if line is delimiter. check if return empty = EOF
// gets line len, gets delimterlen, removes /n,
// compares content and len to check if its delimiter on a single line
/// @param line
/// @param hdoc
/// @return
static int	process_heredoc_line(char *line, t_heredoc_ctx *hdoc)
{
	size_t	line_len;
	size_t	delimiter_len;
	int		is_delimiter;

	if (!line)
		return (1);
	line_len = ft_strlen(line);
	delimiter_len = ft_strlen(hdoc->delimiter);
	is_delimiter = 0;
	if (line_len > 0 && line[line_len - 1] == '\n')
		line_len--;
	if (line_len == delimiter_len && ft_strncmp(line, hdoc->delimiter,
			delimiter_len) == 0)
		is_delimiter = 1;
	if (is_delimiter)
		return (1);
	return (write_line_to_file(hdoc->fd, line, hdoc->delimiter_quoted,
			hdoc->ctx));
}

void	print_new_prompt(int signum)
{
	(void)signum;
	g_signal_status = 1;
	write(1, "\n", 1);
}

// ok
/// @brief read input until end. check if non or interactive mode
// calls readline or get_next_line. reads line. checks for delimiter for exit
/// @param hdoc
void	heredoc_read_loop(t_heredoc_ctx *hdoc, t_cmd_node *cmd, char *smth)
{
	char	*line;
	int		is_interactive;

	(void)cmd;
	(void)smth;
	is_interactive = isatty(STDIN_FILENO);
	heredoc_signals();
	while (1)
	{
		line = read_heredoc_line(is_interactive);
		if (process_heredoc_line(line, hdoc))
		{
			free(line);
			break ;
		}
		free(line);
	}
	start_signals();
}
