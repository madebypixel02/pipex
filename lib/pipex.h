/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <aperez-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/22 18:53:59 by aperez-b          #+#    #+#             */
/*   Updated: 2021/09/22 21:46:45 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H

# define PIPEX_H

# include <sys/wait.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include "../libft/lib/libft.h"

/* Struct to store every command with its corresponding file */
typedef struct s_pipexcmd
{
	char	*cmd;
	char	*file;
}				t_pipexcmd;

/* Creates new node storing the command and the file */
t_list	*pipex_lstnew(char *cmd, char *file);

/* Frees all stuf inside pipexcmd struct */
void	pipex_freecmd(void *node);

/* Prints linked list of commands */
void	pipex_printlist(t_list *cmds);

#endif
