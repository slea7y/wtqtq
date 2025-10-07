/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_validation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 18:36:09 by tdietztp          #+#    #+#             */
/*   Updated: 2025/10/05 22:38:28 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/parser.h"

/// @brief validates pipes syntax. waits for cmd,
//	if it gets a pipe instead-> false. needs cdm afer |
/// @param tokens
/// @return
bool	validate_pipes(t_token_list *tokens)
{
	t_token	*current;
	bool	expecting_cmd;

	current = tokens->head;
	expecting_cmd = true;
	while (current)
	{
		if (current->token == TOKEN_PIPE)
		{
			if (expecting_cmd)
			{
				ft_putstr_fd("hell: syntax error near unexpected token '|'\n",
					2);
				return (false);
			}
			expecting_cmd = true;
		}
		if (current->token == TOKEN_WORD || current->token == TOKEN_BUILTIN)
			expecting_cmd = false;
		current = current->next;
	}
	if (expecting_cmd)
		return (ft_putstr_fd("hell: syntax error near unexpected token '|'\n",
				2), false);
	return (true);
}

/// @brief checks if after redirection comes a cmd or word
/// @param tokens
/// @return
bool	validate_redirections(t_token_list *tokens)
{
	t_token	*current;

	current = tokens->head;
	while (current)
	{
		if (current->token >= TOKEN_OUTFILE && current->token <= TOKEN_HEREDOC)
		{
			if (!current->next || (current->next->token != TOKEN_WORD
					&& current->next->token != TOKEN_BUILTIN))
			{
				ft_putstr_fd("hell: syntax error near unexpected token \'", 2);
				if (current->token == TOKEN_OUTFILE)
					ft_putstr_fd(">", 2);
				else if (current->token == TOKEN_APPEND)
					ft_putstr_fd(">>", 2);
				else if (current->token == TOKEN_INFILE)
					ft_putstr_fd("<", 2);
				else if (current->token == TOKEN_HEREDOC)
					ft_putstr_fd("<<", 2);
				return (ft_putstr_fd("\'\n", 2), false);
			}
		}
		current = current->next;
	}
	return (true);
}

/// @brief loops to the last token and checks if redirection
/// @param tokens
/// @return youre mama gay
bool	validate_redirection_end(t_token_list *tokens)
{
	t_token	*current;

	if (!tokens || !tokens->head)
		return (false);
	current = tokens->head;
	while (current && current->next)
		current = current->next;
	if (current && (current->token >= TOKEN_OUTFILE
			&& current->token <= TOKEN_HEREDOC))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token \'", 2);
		if (current->token == TOKEN_OUTFILE)
			ft_putstr_fd(">", 2);
		else if (current->token == TOKEN_APPEND)
			ft_putstr_fd(">>", 2);
		else if (current->token == TOKEN_INFILE)
			ft_putstr_fd("<", 2);
		else if (current->token == TOKEN_HEREDOC)
			ft_putstr_fd("<<", 2);
		ft_putstr_fd("\'\n", 2);
		return (false);
	}
	return (true);
}

bool	validate_invalid_chars(t_token_list *tokens)
{
	(void) tokens;
	return (true);
}

/// @brief chekcs if token is a single char with ~ or /
/// @param tokens
/// @return
// bool	validate_invalid_chars(t_token_list *tokens)
		///SOMETHING TO THINK ABOUT WE PASS MORE WITHOUT IT
// {
// 	(void)tokens;
// t_token	*current;

// if (!tokens || !tokens->head)
// 	return (false);
// current = tokens->head;
// while (current)
// {
// 	if (current->token == TOKEN_WORD)
// 	{
// 		if (current->value && (ft_strncmp(current->value, "~", 2) == 0
// 				|| ft_strncmp(current->value, "/", 2) == 0))
// 		{
// 			ft_putstr_fd("minishell: syntax error near unexpected token \'",
// 				2);
// 			ft_putstr_fd(current->value, 2);
// 			ft_putstr_fd("\'\n", 2);
// 			return (false);
// 		}
// 	}
// 	current = current->next;
// }
// 	return (true);
// }

/// @brief line was a bit to long lol. just prints a message
/// @param
/// @return
bool	validate_syntax(t_token_list *tokens)
{
	if (!tokens || !tokens->head)
		return (false);
	if (!validate_quotes(tokens))
		return (false);
	if (!validate_pipes(tokens))
		return (false);
	if (!validate_redirections(tokens))
		return (false);
	if (!validate_redirection_end(tokens))
		return (false);
	if (!validate_invalid_chars(tokens))
		return (false);
	return (true);
}
