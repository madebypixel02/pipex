/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <aperez-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/22 18:52:57 by aperez-b          #+#    #+#             */
/*   Updated: 2021/09/24 19:34:26 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/pipex.h"
#include <sys/wait.h>
#include <unistd.h>

char	*find_command(char *argv, char **env_path)
{
	char	*full_path;
	char	*temp;
	int		i;

	i = -1;
	full_path = NULL;
	while (env_path[++i])
	{
		free(full_path);
		temp = ft_strjoin(env_path[i], "/");
		if (!temp)
			return (NULL);
		full_path = ft_strjoin(temp, argv);
		free(temp);
		if (!full_path)
			return (NULL);
		if (access(full_path, F_OK) == 0)
			break ;
	}
	if (!env_path[i])
	{
		free(full_path);
		return (NULL);
	}
	return (full_path);
}

void	parse_commands(int argc, char **argv, char **env_path, t_list **cmds)
{
	int		i;
	char	*full_path;

	i = 0;
	while (argv[++i] && argv[i + 1])
	{
		if (i == 1)
		{
			full_path = find_command(argv[i + 1], env_path);
			ft_lstadd_back(cmds, pipex_lstnew(full_path, \
				ft_split(argv[i + 1], ' '), argv[i]));
			i++;
		}
		else
		{
			full_path = find_command(argv[i], env_path);
			if (i == argc - 2)
			{
				ft_lstadd_back(cmds, pipex_lstnew(full_path, \
					ft_split(argv[i], ' '), argv[i + 1]));
				i++;
			}
			else
				ft_lstadd_back(cmds, pipex_lstnew(full_path, \
					ft_split(argv[i], ' '), NULL));
		}
		free(full_path);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_list	*cmds;
	char	**env_path;

	if (argc < 5)
		return (ft_putstr_fd("Incorrect number of arguments!\n", 2));
	cmds = NULL;
	while (!ft_strnstr(*envp, "PATH", ft_strlen(*envp)))
		envp++;
	env_path = ft_split(*envp, ':');
	parse_commands(argc, argv, env_path, &cmds);
	ft_free_matrix(&env_path);
	pipex_printlist(cmds);
	ft_lstclear(&cmds, pipex_freecmd);
	return (0);
}
