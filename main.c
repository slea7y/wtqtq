/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 22:02:12 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/10/07 09:47:10 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Includes/executor.h"
#include "Includes/minishell.h"
#include "Includes/parser.h"

static char	*get_input(void)
{
	char	*input;

	if (isatty(fileno(stdin)))
		input = readline("minishell$ ");
	else
	{
		input = get_next_line(STDIN_FILENO);
		if (input && input[ft_strlen(input) - 1] == '\n')
			input[ft_strlen(input) - 1] = '\0';
	}
	return (input);
}

int	process_input(char *input, t_shell_ctx *ctx)
{
	if (!build_command_pipeline(input, ctx))
		return (1);
	execute_and_clear(input, ctx);
	return (1);
}

int	main_loop(char *input, t_shell_ctx *ctx)
{
	int	signal_status;

	if (input[0] == '\0')
	{
		free(input);
		return (0);
	}
	signal_status = was_signal_received();
	if (signal_status)
	{
		ctx->last_exit_code = signal_status;
		reset_signal_status();
	}
	if (isatty(fileno(stdin)))
		add_history(input);
	process_input(input, ctx);
	return (0);
}

int	main(int ac, char **av, char **envp)
{
	char		*input;
	t_env_list	env_list;
	t_shell_ctx	ctx;

	(void)av;
	(void)ac;
	start_signals();
	init_list_env(&env_list, envp);
	ctx.env = &env_list;
	ctx.exit_free = 0;
	ctx.last_exit_code = 0;
	while (1)
	{
		input = get_input();
		if (!input)
		{
			if (isatty(fileno(stdin)))
				ft_putstr_fd("exit\n", 1);
			break ;
		}
		main_loop(input, &ctx);
	}
	free_env_list(&env_list);
	return (ctx.last_exit_code);
}
