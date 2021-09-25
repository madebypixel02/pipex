/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <aperez-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/22 18:53:21 by aperez-b          #+#    #+#             */
/*   Updated: 2021/09/25 10:15:44 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/pipex.h"
#include <bits/types/struct_iovec.h>
#include <unistd.h>

void	*pipex(int io_fd[2], t_list *cmds, char **envp)
{
	t_pipexcmd	*cmd;

	cmd = (struct s_pipexcmd *)cmds->content;
	if (io_fd[0] != -1)
	{
		if (dup2(io_fd[0], STDIN_FILENO) == 1)
			return (NULL);
		execve(cmd->full_path, cmd->cmd, envp);
	}
	return (NULL);
}
