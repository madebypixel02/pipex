/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <aperez-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/22 18:53:21 by aperez-b          #+#    #+#             */
/*   Updated: 2021/09/25 16:37:09 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/pipex.h"
#include <bits/types/struct_iovec.h>
#include <sys/wait.h>
#include <unistd.h>

static t_pipexcmd	*pipxex_newcmd(char *full_path, char **cmd)
{
	t_pipexcmd	*newcmd;

	if (!cmd)
		return (NULL);
	newcmd = malloc(sizeof(t_pipexcmd));
	if (!newcmd)
		return (NULL);
	newcmd->full_path = ft_strdup(full_path);
	if (full_path && !newcmd->full_path)
	{
		free(newcmd);
		return (NULL);
	}
	newcmd->cmd = cmd;
	return (newcmd);
}

t_list	*pipex_lstnew(char *full_path, char **cmd)
{
	t_list	*newnode;

	newnode = malloc(sizeof(t_list));
	if (!newnode)
		return (NULL);
	newnode->content = pipxex_newcmd(full_path, cmd);
	if (!newnode->content)
	{
		free(newnode);
		return (NULL);
	}
	newnode->next = NULL;
	return (newnode);
}

t_pipexdata	*pipex_get_data(int argc, char **argv, char **envp)
{
	t_pipexdata	*data;
	int			i;

	i = 0;
	data = malloc(sizeof(struct s_pipexdata));
	if (!data)
		return ((t_pipexdata *)pipex_exit(data, NULL, NO_MEMORY, NULL));
	data->env_path = NULL;
	data->env_path = NULL;
	data->cmds = NULL;
	data->input_fd = open(argv[1], O_RDONLY);
	if (data->input_fd == -1)
		return ((t_pipexdata *)pipex_exit(data, argv[1], NO_FILE, NULL));
	data->output_fd = open(argv[argc - 1], O_CREAT | O_RDWR, 0666);
	if (access(argv[argc - 1], F_OK) == -1)
		return ((t_pipexdata *)pipex_exit(data, argv[argc - 1], NO_FILE, NULL));
	if (access(argv[argc - 1], W_OK) == -1)
		return ((t_pipexdata *)pipex_exit(data, argv[argc - 1], NO_PERM, NULL));
	while (!ft_strnstr(envp[i], "PATH", ft_strlen(envp[i])))
		i++;
	data->env_path = ft_split(envp[i], ':');
	if (!data->env_path)
		return ((t_pipexdata *)pipex_exit(data, NULL, NO_MEMORY, NULL));
	data->cmds = parse_commands(argc, argv, data);
	return (data);
}

static void	*recursive_pipex(t_list *cmds, char **envp)
{
	int			fd[2];
	pid_t		pid;
	t_pipexcmd	*cmd;

	cmd = (struct s_pipexcmd *)cmds;
	if (pipe(fd) == -1)
		return (NULL);
	pid = fork();
	if (pid == -1)
		return (NULL);
	if (!pid)
	{
		close(fd[WRITE_FD]);
		if (dup2(fd[READ_FD], STDIN_FILENO) == -1)
			return (NULL);
		close(fd[READ_FD]);
		execve(cmd->full_path, cmd->cmd, envp);
		exit(0);
		return (NULL);
	}
	close(fd[READ_FD]);
	if (dup2(STDIN_FILENO, fd[WRITE_FD]) == -1)
		return (NULL);
	waitpid(-1, NULL, 0);
	return (NULL);
}

void	*pipex(int io_fd[2], t_list *cmds, char **envp)
{
	t_pipexcmd	*cmd;

	cmd = (struct s_pipexcmd *)cmds;
	if (!cmds)
		return (NULL);
	if (dup2(io_fd[0], STDIN_FILENO) == -1)
		return (NULL);
	execve(cmd->full_path, cmd->cmd, envp);
	close(io_fd[0]);
	recursive_pipex(cmds, envp);
	printf("%s\n", "HELLO!\n");
	if (dup2(STDIN_FILENO, io_fd[1]) == -1)
		return (NULL);
	close(io_fd[1]);
	return (NULL);
}
