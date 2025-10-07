/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maja <maja@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 09:47:43 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/10/07 16:17:11 by maja             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Includes/minishell.h"
#include "Includes/executor.h" 
#include "Includes/parser.h"

int	handle_lexer_error(t_token_list *tokens, char *input,
		t_shell_ctx *ctx)
{
	ctx->last_exit_code = 2;
	if (tokens)
		free_token_list(tokens);
	free(input);
	return (0);
}

t_cmd_list	*build_command_pipeline(char *input, t_shell_ctx *ctx)
{
	t_token_list	*tokens;
	t_cmd_list		*cmds;

	tokens = start_lexer(input);
	if (!tokens || tokens->found_error)
		return (handle_lexer_error(tokens, input, ctx), NULL);
	start_segmentation(tokens, ctx);
	cmds = start_parser(tokens, ctx, input);
	if (!cmds || cmds->syntax_error)
	{
		ctx->token_list = tokens;
		return (handle_parser_error(cmds, tokens, input, ctx), NULL);
	}
	ctx->token_list = tokens;
	ctx->cmd_list = cmds;
	return (cmds);
}

void	execute_and_clear(char *input, t_shell_ctx *ctx)
{
	int	last_status;

	last_status = start_execution(ctx->cmd_list, ctx);
	ctx->last_exit_code = last_status;
	if (last_status >= 128 && last_status <= 130)
		set_signal_status(last_status);
	else
		reset_signal_status();
	if (ctx->cmd_list)
		free_cmd_list(ctx->cmd_list);
	if (ctx->token_list)
		free_token_list(ctx->token_list);
	if (input)
		free(input);
	ctx->cmd_list = NULL;
	ctx->token_list = NULL;
	if (ctx->exit_free)
	{
		// rl_clear_history();
		if (ctx->env)
			free_env_list(ctx->env);
		exit(ctx->last_exit_code);
	}
}
