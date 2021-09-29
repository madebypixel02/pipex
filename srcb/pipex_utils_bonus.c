/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <aperez-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 14:06:21 by aperez-b          #+#    #+#             */
/*   Updated: 2021/09/29 23:58:13 by aperez-b         ###   ########.fr       */
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
		close(data->in_fd);
		if (data->here_doc)
			unlink(".test");
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
	ft_putstr_fd(RED, 2);
	ft_putstr_fd("pipex: ", 2);
	if (err == CMD_NOT_FOUND)
		ft_putstr_fd("command not found: ", 2);
	if (err == NO_FILE)
		ft_putstr_fd("no such file or directory: ", 2);
	if (err == NO_PERM)
		ft_putstr_fd("permission denied: ", 2);
	if (err == INV_ARGS)
		ft_putstr_fd("invalid number of arguments", 2);
	if (err == NO_MEMORY)
		ft_putstr_fd("no memory left on device", 2);
	if (err == DUP_ERR)
		ft_putstr_fd("error: could not create fd with dup2", 2);
	if (err == PIPE_ERR)
		ft_putstr_fd("error: could not create pipe", 2);
	if (err == FORK_ERR)
		ft_putstr_fd("error: could not fork process", 2);
	if (param && (err == CMD_NOT_FOUND || err == NO_FILE || err == NO_PERM))
		ft_putstr_fd(param, 2);
	ft_putstr_fd("\n", 2);
	ft_putstr_fd(DEFAULT, 2);
}
