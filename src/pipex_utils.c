/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <aperez-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 14:06:21 by aperez-b          #+#    #+#             */
/*   Updated: 2021/09/27 16:00:21 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

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

void	*pipex_exit(t_pipexdata *data, char *param, int err, char ***cmd)
{
	if (err < 1 || param)
		pipex_perror(param, err);
	if (cmd)
		ft_free_matrix(cmd);
	if (data)
	{
		close(data->input_fd);
		close(data->output_fd);
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
	if (param && err != INV_ARGS && err != NO_MEMORY)
		ft_putstr_fd(param, 2);
	ft_putchar_fd('\n', 2);
	ft_putstr_fd(DEFAULT, 2);
}
