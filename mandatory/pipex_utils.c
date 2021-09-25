/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <aperez-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 14:06:21 by aperez-b          #+#    #+#             */
/*   Updated: 2021/09/25 14:14:55 by aperez-b         ###   ########.fr       */
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

void	*pipex_exit(t_pipexdata *data, char *msg, int fd)
{
	if (data)
	{
		close(data->input_fd);
		close(data->output_fd);
		if (data->cmds)
			ft_lstclear(&data->cmds, pipex_freecmd);
		ft_free_matrix(&data->env_path);
		free(data);
	}
	ft_putstr_fd(RED, 2);
	if (msg)
		ft_putstr_fd(msg, fd);
	ft_putstr_fd(DEFAULT, 2);
	exit(0);
	return (0);
}

void	pipex_cmd_not_found(char *cmd)
{
	ft_putstr_fd(RED, 2);
	ft_putstr_fd("pipex: command not found: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putchar_fd('\n', 2);
	ft_putstr_fd(DEFAULT, 2);
}
