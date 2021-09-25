/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <aperez-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/22 18:53:59 by aperez-b          #+#    #+#             */
/*   Updated: 2021/09/25 09:19:14 by aperez-b         ###   ########.fr       */
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
	char	*full_path;
	char	**cmd;
}				t_pipexcmd;

/* Creates new node storing the command and the file */
t_list	*pipex_lstnew(char *full_path, char **cmd);

/* Frees all stuf inside pipexcmd struct */
void	pipex_freecmd(void *node);

/* Prints linked list of commands */
void	pipex_printlist(t_list *cmds);

/* Finds correct path for a shell command and returns it as a string */
char	*find_command(char *cmd, char **env_path);

/* Creates a set of fds and forks to pass the output to a new command */
void	*pipex(int fd[2], int io_fd[2], t_list *cmds);

#endif
