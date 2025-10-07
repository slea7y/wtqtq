/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 22:31:00 by majkijew          #+#    #+#             */
/*   Updated: 2025/10/07 08:28:09 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/minishell.h"
#include "../../../Includes/executor.h"
#include "../../../Includes/parser.h"

char	*build_cmd_paths(char *dir, char *cmd)
{
	char	*temp;
	char	*full_path;

	temp = ft_strjoin(dir, "/");
	if (!temp)
		return (NULL);
	full_path = ft_strjoin(temp, cmd);
	free(temp);
	return (full_path);
}

char	**find_full_path(t_env_list *env_list)
{
	char	*path_val;
	char	**paths;

	path_val = get_env_value(env_list, "PATH");
	if (!path_val || path_val[0] == '\0')
	{
		if (path_val)
			free(path_val);
		return (NULL);
	}
	paths = ft_split(path_val, ':');
	free(path_val);
	if (!paths)
		return (NULL);
	return (paths);
}
