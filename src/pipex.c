/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <aperez-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/22 18:53:21 by aperez-b          #+#    #+#             */
/*   Updated: 2021/09/29 19:15:30 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"
#include <sys/wait.h>
#include <unistd.h>

t_pipexdata	*pipex_get_data(int argc, char **argv, int here_doc, char **envp)
{
	t_pipexdata	*data;
	int			i;

	i = 0;
	data = malloc(sizeof(struct s_pipexdata));
	if (!data)
		return ((t_pipexdata *)pipex_exit(data, NULL, NO_MEMORY, NULL));
	data->env_path = NULL;
	data->cmds = NULL;
	data->in_fd = open(argv[1], O_RDONLY);
	if (data->in_fd == -1)
		return ((t_pipexdata *)pipex_exit(data, argv[1], NO_FILE, NULL));
	if (!here_doc)
		data->out_fd = open(argv[argc - 1], O_CREAT | O_RDWR | O_TRUNC, 0666);
	if (access(argv[argc - 1], F_OK) == -1)
		return ((t_pipexdata *)pipex_exit(data, argv[argc - 1], NO_FILE, NULL));
	if (access(argv[argc - 1], W_OK) == -1)
		return ((t_pipexdata *)pipex_exit(data, argv[argc - 1], NO_PERM, NULL));
	while (!ft_strnstr(envp[i], "PATH", ft_strlen(envp[i])))
		i++;
	data->env_path = ft_split(envp[i], ':');
	if (!data->env_path)
		return ((t_pipexdata *)pipex_exit(data, NULL, NO_MEMORY, NULL));
	return (data);
}

void	*pipex_child(t_pipexdata *data, int fd[2], char **envp)
{
	int			pid;
	t_pipexcmd	*cmd;
	t_pipexcmd	*cmd2;

	cmd = (struct s_pipexcmd *)data->cmds->content;
	cmd2 = (struct s_pipexcmd *)data->cmds->next->content;
	pid = fork();
	if (!pid)
	{
		close(fd[READ_END]);
		if (dup2(fd[WRITE_END], STDOUT_FILENO) == -1)
			return (pipex_exit(data, NULL, DUP_ERR, NULL));
		execve(cmd->full_path, cmd->cmd, envp);
		close(fd[WRITE_END]);
		return (pipex_exit(data, NULL, END, NULL));
	}
	close(fd[WRITE_END]);
	waitpid(pid, NULL, 0);
	if (dup2(fd[READ_END], STDIN_FILENO) == -1)
		return (pipex_exit(data, NULL, DUP_ERR, NULL));
	if (dup2(data->out_fd, STDOUT_FILENO) == -1)
		return (pipex_exit(data, NULL, DUP_ERR, NULL));
	execve(cmd2->full_path, cmd2->cmd, envp);
	close(fd[READ_END]);
	return (pipex_exit(data, NULL, END, NULL));
}

void	*pipex(t_pipexdata *data, char **envp)
{
	pid_t		pid;
	int			fd[2];

	if (dup2(data->in_fd, STDIN_FILENO) == -1)
		return (pipex_exit(data, NULL, DUP_ERR, NULL));
	if (pipe(fd) == -1)
		return (pipex_exit(data, NULL, PIPE_ERR, NULL));
	pid = fork();
	if (pid == -1)
		return (pipex_exit(data, NULL, FORK_ERR, NULL));
	if (!pid)
		return (pipex_child(data, fd, envp));
	close(fd[WRITE_END]);
	waitpid(pid, NULL, 0);
	close(fd[READ_END]);
	return (pipex_exit(data, NULL, END, NULL));
}
