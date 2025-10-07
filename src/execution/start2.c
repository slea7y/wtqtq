/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 10:07:52 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/10/07 10:30:27 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/executor.h"

int	search_in_paths(char **cmd, char **envp, char **paths, t_shell_ctx *ctx)
{
	char	*path_to_cmd;
	int		i;

	i = 0;
	while (paths[i])
	{
		path_to_cmd = build_cmd_paths(paths[i], cmd[0]);
		if (!path_to_cmd)
			return (free_env(paths), 1);
		if (access(path_to_cmd, X_OK) == 0)
		{
			free_env(paths);
			execve(path_to_cmd, &cmd[0], envp);
			perror("execve");
			ctx->exit_free = 1;
			free(path_to_cmd);
			return (1);
		}
		free(path_to_cmd);
		i++;
	}
	ft_putstr_fd(cmd[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	free_env(paths);
	return (127);
}

// "./" for the relative paths
int	execute_cmd(t_cmd_node *cmd, char **envp, t_shell_ctx *ctx)
{
	char	**paths;

	if (cmd->cmd[0][0] == '/' || ft_strncmp(cmd->cmd[0], "./", 2) == 0)
	{
		if (access(cmd->cmd[0], X_OK) == 0)
		{
			execve(cmd->cmd[0], cmd->cmd, envp);
			perror("execve");
			ctx->exit_free = 1;
			return (1);
		}
		ft_putstr_fd(cmd->cmd[0], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (127);
	}
	paths = find_full_path(ctx->env);
	if (!paths)
	{
		ft_putstr_fd(cmd->cmd[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		ctx->exit_free = 1;
		return (127);
	}
	return (search_in_paths(cmd->cmd, envp, paths, ctx));
}

int	is_bulidin(char *cmd)
{
	if (!cmd)
		return (0);
	else
		return (ft_strcmp(cmd, "exit") == 0 || ft_strcmp(cmd, "echo") == 0
			|| ft_strcmp(cmd, "env") == 0 || ft_strcmp(cmd, "export") == 0
			|| ft_strcmp(cmd, "cd") == 0 || ft_strcmp(cmd, "unset") == 0
			|| ft_strcmp(cmd, "pwd") == 0);
}

int	execute_bulidins(t_cmd_node *cmd, t_shell_ctx *ctx)
{
	int	exit_code;

	if (!cmd || !cmd->cmd[0])
		return (1);
	if (ft_strcmp(cmd->cmd[0], "echo") == 0)
		exit_code = ft_echo(cmd, ctx);
	else if (ft_strcmp(cmd->cmd[0], "env") == 0)
		exit_code = ft_env(cmd, ctx);
	else if (ft_strcmp(cmd->cmd[0], "export") == 0)
		exit_code = ft_export(cmd, ctx);
	else if (ft_strcmp(cmd->cmd[0], "cd") == 0)
		exit_code = ft_cd(cmd, ctx);
	else if (ft_strcmp(cmd->cmd[0], "unset") == 0)
		exit_code = ft_unset(cmd, ctx);
	else if (ft_strcmp(cmd->cmd[0], "pwd") == 0)
		exit_code = ft_pwd(cmd, ctx);
	else if (ft_strcmp(cmd->cmd[0], "exit") == 0)
		exit_code = ft_exit(cmd, ctx);
	else
		exit_code = 0;
	return (exit_code);
}

int	execute_builtin_with_redirections(t_cmd_node *current,
		t_shell_ctx *ctx)
{
	int	exit_code;
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (handle_redirections(current) != 0)
		exit_code = 1;
	else
		exit_code = execute_bulidins(current, ctx);
	if (saved_stdin != -1)
	{
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
	}
	if (saved_stdout != -1)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
	return (exit_code);
}
