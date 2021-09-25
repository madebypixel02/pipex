/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <aperez-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/22 18:52:57 by aperez-b          #+#    #+#             */
/*   Updated: 2021/09/25 14:17:09 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/pipex.h"
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

int	find_command(t_pipexdata *data, char *cmd, char **full_path)
{
	char	*temp;
	int		i;

	i = -1;
	*full_path = NULL;
	while (data->env_path[++i])
	{
		free(*full_path);
		temp = ft_strjoin(data->env_path[i], "/");
		if (!temp)
			return (-2);
		*full_path = ft_strjoin(temp, cmd);
		free(temp);
		if (!full_path)
			return (-2);
		if (access(*full_path, F_OK) == 0)
			break ;
	}
	if (!data->env_path[i])
	{
		free(*full_path);
		return (-1);
	}
	return (0);
}

t_list	*parse_commands(int argc, char **argv, t_pipexdata *data)
{
	int		i;
	char	*full_path;
	char	**cmd;
	t_list	*cmds;

	cmds = NULL;
	i = 1;
	full_path = NULL;
	while (++i < argc - 1)
	{
		data->cmds = cmds;
		cmd = ft_split(argv[i], ' ');
		if (!cmd)
			return ((t_list *)pipex_exit(data, \
				"pipex: device out of memory\n", 2));
		if (find_command(data, *cmd, &full_path) < 0)
		{
			pipex_cmd_not_found(*cmd);
			ft_free_matrix(&cmd);
			return (pipex_exit(data, NULL, 2));
		}
		ft_lstadd_back(&cmds, pipex_lstnew(full_path, cmd));
		free(full_path);
	}
	return (cmds);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipexdata	*data;

	data = NULL;
	if (argc < 5)
		return (*(int *)pipex_exit(data, \
			"pipex: incorrect number of arguments!\n", 2));
	if (access(argv[1], F_OK | R_OK) == -1)
		return (*(int *)pipex_exit(data, \
			"pipex: input file unavailable for reading\n", 2));
	data = pipex_get_data(argc, argv, envp);
	pipex_printlist(data->cmds);
	return (*(int *)pipex_exit(data, NULL, 1));
}
