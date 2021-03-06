/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <aperez-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 14:06:21 by aperez-b          #+#    #+#             */
/*   Updated: 2021/10/02 20:03:33 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

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

void	pipex_freecmd(void *node)
{
	t_pipexcmd	*temp;

	temp = (struct s_pipexcmd *)node;
	free(temp->full_path);
	ft_free_matrix(&temp->cmd);
	free(temp);
}

void	*pipex_exit(t_pipexdata *data, char *param, int err, char ***cmd)
{
	if (err < 1 || param)
		pipex_perror(param, err);
	if (cmd)
		ft_free_matrix(cmd);
	if (data)
	{
		close(STDIN_FILENO);
		close(data->in_fd);
		close(data->out_fd);
		if (data->cmds)
			ft_lstclear(&data->cmds, pipex_freecmd);
		if (data->env_path)
			ft_free_matrix(&data->env_path);
		free(data);
	}
	exit(0);
	return (0);
}

void	pipex_perror(char *param, int err)
{
	ft_putstr_fd("pipex: ", 2);
	if (err == CMD_NOT_FOUND)
		ft_putstr_fd("command not found: ", 2);
	if (err == NO_FILE)
		ft_putstr_fd("no such file or directory: ", 2);
	if (err == NO_PERM)
		ft_putstr_fd("permission denied: ", 2);
	if (err == CMD_FAIL)
		ft_putstr_fd("command failed: ", 2);
	if (err == INV_ARGS)
		ft_putstr_fd("invalid number of arguments", 2);
	if (err == NO_MEMORY)
		ft_putstr_fd("no memory left on device", 2);
	if (err == DUP_ERR)
		ft_putstr_fd("could not dup fd", 2);
	if (err == PIPE_ERR)
		ft_putstr_fd("could not create pipe", 2);
	if (err == FORK_ERR)
		ft_putstr_fd("could not fork process", 2);
	if (err == NO_PATH)
		ft_putstr_fd("PATH variable is not set", 2);
	if (param && (err == CMD_NOT_FOUND || err == NO_FILE || err == NO_PERM || \
			err == CMD_FAIL))
		ft_putstr_fd(param, 2);
	ft_putstr_fd("\n", 2);
}
