/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <aperez-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/22 18:52:57 by aperez-b          #+#    #+#             */
/*   Updated: 2021/12/17 14:27:54 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

int	find_command(t_pipexdata *data, char *cmd, char **full_path)
{
	char	*temp;
	int		i;

	i = -1;
	*full_path = NULL;
	while (data->env_path && data->env_path[++i])
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
	if (!data->env_path || !data->env_path[i])
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
	int		temp;

	cmds = NULL;
	i = 1;
	full_path = NULL;
	while (++i < argc - 1)
	{
		data->cmds = cmds;
		cmd = ft_split(argv[i], ' ');
		if (!cmd || !*cmd)
			return ((t_list *)pipex_exit(data, NULL, CMD_NOT_FOUND, &cmd));
		temp = find_command(data, *cmd, &full_path);
		if (temp == -1)
			return (pipex_exit(data, *cmd, CMD_NOT_FOUND, &cmd));
		if (temp == -2)
			return (pipex_exit(data, NULL, NO_MEMORY, &cmd));
		ft_lstadd_back(&cmds, pipex_lstnew(full_path, cmd));
		free(full_path);
	}
	return (cmds);
}

char	*pipex_here_str(char *limit, char *buf, char *final)
{
	char	c[2];
	char	*temp;

	c[1] = '\0';
	while (!buf || ft_strlen(buf) - 1 != ft_strlen(limit) || \
			ft_strncmp(limit, buf, ft_strlen(buf) - 1))
	{
		temp = final;
		final = ft_strjoin(final, buf);
		free(temp);
		free(buf);
		buf = NULL;
		*c = 0;
		ft_putstr_fd("> ", 1);
		while (*c != '\n')
		{
			read(0, c, 1);
			temp = buf;
			buf = ft_strjoin(buf, c);
			free(temp);
		}
	}
	free(buf);
	return (final);
}

int	pipex_here_fd(t_pipexdata *d, char *hdoc_str)
{
	int		fd[2];
	pid_t	pid;

	if (pipe(fd) == -1)
		pipex_exit(d, NULL, PIPE_ERR, NULL);
	pid = fork();
	if (pid == -1)
		pipex_exit(d, NULL, FORK_ERR, NULL);
	if (!pid)
	{
		close(fd[READ_END]);
		write(fd[WRITE_END], hdoc_str, ft_strlen(hdoc_str));
		d->in_fd = fd[READ_END];
		close(fd[WRITE_END]);
		free(hdoc_str);
		return (*(int *)pipex_exit(d, NULL, END, NULL));
	}	
	close(fd[WRITE_END]);
	free(hdoc_str);
	return (fd[READ_END]);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipexdata	*d;
	int			here_doc;

	if (argc < 5)
		return (*(int *)pipex_exit(NULL, NULL, INV_ARGS, NULL));
	here_doc = !ft_strncmp(argv[1], "here_doc", ft_strlen(argv[1]));
	if (!here_doc && access(argv[1], F_OK) == -1)
		return (*(int *)pipex_exit(NULL, argv[1], NO_FILE, NULL));
	if (!here_doc && access(argv[1], R_OK) == -1)
		return (*(int *)pipex_exit(NULL, argv[1], NO_PERM, NULL));
	d = pipex_get_data(argc, argv, here_doc, envp);
	d->here_doc = here_doc;
	if (here_doc)
	{
		if (argc-- < 6)
			return (*(int *)pipex_exit(d, NULL, INV_ARGS, NULL));
		d->in_fd = pipex_here_fd(d, pipex_here_str(argv[2], NULL, NULL));
		argv++;
	}
	else
		d->in_fd = open(argv[1], O_RDONLY);
	d->cmds = parse_commands(argc, argv, d);
	pipex(d, envp);
	return (*(int *)pipex_exit(d, NULL, END, NULL));
}
