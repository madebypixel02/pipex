/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <aperez-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/22 18:53:21 by aperez-b          #+#    #+#             */
/*   Updated: 2021/09/29 12:29:49 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"
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
	data->output_fd = open(argv[argc - 1], O_CREAT | O_RDWR | O_TRUNC, 0666);
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

/*static void	*recursive_pipex(t_pipexdata *data, char **envp)
{
	int			fd[2];
	pid_t		pid;
	t_pipexcmd	*cmd;

	cmd = (struct s_pipexcmd *)data->cmds;
	if (pipe(fd) == -1)
		return (pipex_exit(data, NULL, NO_MEMORY, NULL));
	pid = fork();
	if (pid == -1)
		return (pipex_exit(data, NULL, NO_MEMORY, NULL));
	if (!pid)
	{
		close(fd[WRITE_FD]);
		if (dup2(STDIN_FILENO, fd[READ_FD]) == -1)
			return (pipex_exit(data, NULL, NO_MEMORY, NULL));
		//close(fd[READ_FD]);
		if (dup2(fd[READ_FD], data->output_fd) == -1)
			return (pipex_exit(data, NULL, NO_MEMORY, NULL));
		execve(cmd->full_path, cmd->cmd, envp);
		return (pipex_exit(data, NULL, 1, NULL));
	}
	
	close(fd[READ_FD]);
	if (dup2(STDOUT_FILENO, fd[WRITE_FD]) == -1)
		return (pipex_exit(data, NULL, NO_MEMORY, NULL));
	execve(cmd->full_path, cmd->cmd, envp);
	waitpid(-1, NULL, 0);
	return (NULL);
}*/

void	*pipex(t_pipexdata *data, char **envp)
{
	t_pipexcmd	*cmd;
	t_pipexcmd	*cmd2;
	pid_t		pid;
	int			fd[2];

	cmd = (struct s_pipexcmd *)data->cmds->content;
	cmd2 = (struct s_pipexcmd *)data->cmds->next->content;
	if (!cmd)
		return (pipex_exit(data, NULL, 1, NULL));
	if (dup2(data->input_fd, STDIN_FILENO) == -1)
		return (pipex_exit(data, NULL, NO_MEMORY, NULL));
	if (pipe(fd) == -1)
		return (pipex_exit(data, NULL, NO_MEMORY, NULL));
	pid = fork();
	if (pid == -1)
		return (pipex_exit(data, NULL, NO_MEMORY, NULL));
	if (!pid)
	{
		pid = fork();
		if (!pid)
		{
			close(fd[READ_END]);
			if (dup2(fd[WRITE_END], STDOUT_FILENO) == -1)
				return (pipex_exit(data, NULL, NO_MEMORY, NULL));
			execve(cmd->full_path, cmd->cmd, envp);
			close(fd[WRITE_END]);
			return (pipex_exit(data, NULL, 1, NULL));
		}
		close(fd[WRITE_END]);
		waitpid(pid, NULL, 0);
		if (dup2(fd[READ_END], STDIN_FILENO) == -1)
			return (pipex_exit(data, NULL, NO_MEMORY, NULL));
		if (dup2(data->output_fd, STDOUT_FILENO) == -1)
			return (pipex_exit(data, NULL, NO_MEMORY, NULL));
		execve(cmd2->full_path, cmd2->cmd, envp);
		close(fd[READ_END]);
		return (pipex_exit(data, NULL, 1, NULL));
	}
	close(fd[WRITE_END]);
	waitpid(pid, NULL, 0);
	close(fd[READ_END]);
	close(data->output_fd);
	//recursive_pipex(data, envp);
	close(data->input_fd);
	close(data->output_fd);
	return (NULL);
}
