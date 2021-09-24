/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <aperez-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 14:06:21 by aperez-b          #+#    #+#             */
/*   Updated: 2021/09/24 20:58:55 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/pipex.h"

void	pipex_freecmd(void *node)
{
	t_pipexcmd	*temp;

	temp = (struct s_pipexcmd *)node;
	free(temp->full_path);
	ft_free_matrix(&temp->cmd);
	free(temp);
}

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

void	pipex_printlist(t_list *cmds)
{
	t_pipexcmd	*cmd;
	char		**temp;

	while (cmds)
	{
		cmd = (struct s_pipexcmd *)cmds->content;
		ft_putstr_fd("Path: ", 1);
		if (cmd->full_path)
			ft_putstr_fd(cmd->full_path, 1);
		ft_putstr_fd("\n", 1);
		ft_putstr_fd("Command: ", 1);
		temp = cmd->cmd;
		while (*temp)
		{
			ft_putstr_fd(*temp, 1);
			ft_putchar_fd(' ', 1);
			temp++;
		}
		ft_putstr_fd("\n\n", 1);
		cmds = cmds->next;
	}
}
