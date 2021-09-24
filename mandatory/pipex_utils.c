/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <aperez-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 14:06:21 by aperez-b          #+#    #+#             */
/*   Updated: 2021/09/24 14:08:19 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/pipex.h"

void	pipex_freecmd(void *node)
{
	t_pipexcmd	*temp;

	temp = (struct s_pipexcmd *)node;
	free(temp->cmd);
	free(temp->file);
	free(temp);
}

static t_pipexcmd	*pipxex_newcmd(char *cmd, char *file)
{
	t_pipexcmd	*newcmd;

	newcmd = malloc(sizeof(t_pipexcmd));
	if (!newcmd)
		return (NULL);
	newcmd->cmd = ft_strdup(cmd);
	if (cmd && !newcmd->cmd)
	{
		free(newcmd);
		return (NULL);
	}
	newcmd->file = ft_strdup(file);
	if (file && !newcmd->file)
	{
		free(newcmd->cmd);
		free(newcmd);
		return (NULL);
	}
	return (newcmd);
}

t_list	*pipex_lstnew(char *cmd, char *file)
{
	t_list	*newnode;

	newnode = malloc(sizeof(t_list));
	if (!newnode)
		return (NULL);
	newnode->content = pipxex_newcmd(cmd, file);
	if (!newnode->content)
	{
		free(newnode);
		return (NULL);
	}
	newnode->next = NULL;
	return (newnode);
}

void	pipex_printlist(t_list *cmds)
{
	t_pipexcmd	*cmd;

	while (cmds)
	{
		cmd = (struct s_pipexcmd *)cmds->content;
		ft_putstr_fd("Command: ", 1);
		if (cmd->cmd)
			ft_putstr_fd(cmd->cmd, 1);
		else
			ft_putstr_fd("(null)", 1);
		ft_putstr_fd("\n", 1);
		ft_putstr_fd("File: ", 1);
		if (cmd->file)
			ft_putstr_fd(cmd->file, 1);
		else
			ft_putstr_fd("(null)", 1);
		ft_putstr_fd("\n", 1);
		ft_putstr_fd("\n", 1);
		cmds = cmds->next;
	}
}
