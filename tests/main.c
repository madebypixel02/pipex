/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <aperez-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/22 18:52:57 by aperez-b          #+#    #+#             */
/*   Updated: 2021/09/25 09:33:23 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/pipex.h"
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

char	*find_command(char *cmd, char **env_path)
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
		full_path = ft_strjoin(temp, cmd);
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

static t_list	*parse_commands(int argc, char **argv, char ***env_path)
{
	int		i;
	char	*full_path;
	char	**cmd;
	t_list	*cmds;

	i = 1;
	cmds = NULL;
	while (++i < argc - 1)
	{
		cmd = ft_split(argv[i], ' ');
		full_path = find_command(*cmd, *env_path);
		ft_lstadd_back(&cmds, pipex_lstnew(full_path, cmd));
		free(full_path);
		cmd = NULL;
	}
	ft_free_matrix(env_path);
	return (cmds);
}

int	main(int argc, char **argv, char **envp)
{
	t_list	*cmds;
	char	**env_path;
	int		io_fd[2];
	//int		fd[2];

	if (argc < 5)
		return (ft_putstr_fd("Incorrect number of arguments!\n", 2));
	if (access(argv[1], F_OK | R_OK) == -1)
		return (ft_putstr_fd("Input file unavailable for reading\n", 1));
	io_fd[0] = open(argv[1], O_RDONLY);
	if (io_fd[0] == -1)
		return (ft_putstr_fd("Error opening infile!\n", 1));
	io_fd[1] = open(argv[argc - 1], O_CREAT | O_RDWR, 0777);
	if (io_fd[1] == -1)
		return (ft_putstr_fd("Error opening outile!\n", 1));
	cmds = NULL;
	while (!ft_strnstr(*envp, "PATH", ft_strlen(*envp)))
		envp++;
	env_path = ft_split(*envp, ':');
	cmds = parse_commands(argc, argv, &env_path);
	pipex_printlist(cmds);
	//pipex(fd, io_fd, cmds);
	close(io_fd[0]);
	close(io_fd[1]);
	ft_lstclear(&cmds, pipex_freecmd);
	return (0);
}
