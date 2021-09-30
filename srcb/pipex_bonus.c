/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <aperez-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/22 18:53:21 by aperez-b          #+#    #+#             */
/*   Updated: 2021/09/30 14:52:32 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"
#include <stdlib.h>
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
	if (!here_doc)
		data->out_fd = open(argv[argc - 1], O_CREAT | O_RDWR | O_TRUNC, 0666);
	else
		data->out_fd = open(argv[argc - 1], O_CREAT | O_RDWR | O_APPEND, 0666);
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
	close(fd[READ_END]);
	if (dup2(fd[WRITE_END], STDOUT_FILENO) == -1)
		return (pipex_exit(data, NULL, DUP_ERR, NULL));
	close(fd[READ_END]);
	data->last_cmd_i--;
	pipex(data, envp);
	return (pipex_exit(data, NULL, END, NULL));
}

void	*pipex_parent(t_pipexdata *data, int fd[2], pid_t pid, char **envp)
{
	t_pipexcmd	*cmd;

	cmd = (t_pipexcmd *)ft_lstget_at(data->cmds, data->last_cmd_i)->content;
	waitpid(pid, NULL, 0);
	close(fd[WRITE_END]);
	if (!data->last_cmd_i && dup2(data->in_fd, STDIN_FILENO) == -1)
		return (pipex_exit(data, NULL, DUP_ERR, NULL));
	else if (data->last_cmd_i && dup2(fd[READ_END], STDIN_FILENO) == -1)
		return (pipex_exit(data, NULL, DUP_ERR, NULL));
	if (data->last_cmd_i == ft_lstsize(data->cmds) - 1 && \
			dup2(data->out_fd, STDOUT_FILENO) == -1)
		return (pipex_exit(data, NULL, DUP_ERR, NULL));
	close(fd[READ_END]);
	execve(cmd->full_path, cmd->cmd, envp);
	return (pipex_exit(data, NULL, END, NULL));
}

void	*pipex(t_pipexdata *data, char **envp)
{
	pid_t		pid;
	int			fd[2];

	if (pipe(fd) == -1)
		return (pipex_exit(data, NULL, PIPE_ERR, NULL));
	pid = 1;
	if (data->last_cmd_i >= 0)
		pid = fork();
	if (pid == -1)
		return (pipex_exit(data, NULL, FORK_ERR, NULL));
	if (!pid)
		return (pipex_child(data, fd, envp));
	return (pipex_parent(data, fd, pid, envp));
}
