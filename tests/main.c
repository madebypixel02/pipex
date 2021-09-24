/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <aperez-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/22 18:52:57 by aperez-b          #+#    #+#             */
/*   Updated: 2021/09/24 15:11:54 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/pipex.h"
#include <sys/wait.h>
#include <unistd.h>

char	*find_command(char *argv, char **envp)
{
	char	*full_path;
	char	*temp;
	char	**env_path;
	int		i;

	i = -1;
	full_path = NULL;
	while (!ft_strnstr(*envp, "PATH", ft_strlen(*envp)))
		envp++;
	env_path = ft_split(*envp, ':');
	if (!env_path)
		return (NULL);
	while (env_path[++i])
	{
		free(full_path);
		temp = ft_strjoin(env_path[i], "/");
		full_path = ft_strjoin(temp, argv);
		free(temp);
		if (access(full_path, F_OK) == 0)
			break ;
	}
	ft_free_matrix(&env_path);
	return (full_path);
}

static void	parse_commands(int argc, char **argv, char **envp, t_list **cmds)
{
	int		i;
	char	*command;

	i = 0;
	while (argv[++i] && argv[i + 1])
	{
		if (i == 1)
		{
			command = find_command(argv[i + 1], envp);
			ft_lstadd_back(cmds, pipex_lstnew(command, argv[i]));
			i++;
		}
		else
		{
			command = find_command(argv[i], envp);
			if (i == argc - 2)
			{
				ft_lstadd_back(cmds, pipex_lstnew(command, argv[i + 1]));
				i++;
			}
			else
				ft_lstadd_back(cmds, pipex_lstnew(command, NULL));
		}
		free(command);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_list	*cmds;

	if (argc < 5)
		return (ft_putstr_fd("Incorrect number of arguments!\n", 2));
	cmds = NULL;
	parse_commands(argc, argv, envp, &cmds);
	pipex_printlist(cmds);
	ft_lstclear(&cmds, pipex_freecmd);
	return (0);
}
