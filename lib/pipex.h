/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <aperez-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/22 18:53:59 by aperez-b          #+#    #+#             */
/*   Updated: 2021/09/24 15:32:59 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H

# define PIPEX_H

# include <sys/wait.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include "../libft/lib/libft.h"

# define READ_FD 0
# define WRITE_FD 1

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

/* Finds correct path for a shell command and returns it as a string */
char	*find_command(char *argv, char **env_path);

#endif
