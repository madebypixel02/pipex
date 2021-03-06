/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <aperez-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/22 18:53:59 by aperez-b          #+#    #+#             */
/*   Updated: 2021/10/06 15:43:46 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H

# define PIPEX_H

# include <sys/wait.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include "../inc/libft_tools.h"

# define READ_END 0
# define WRITE_END 1

/* Enum for error handling */
enum e_pipex_error
{
	END = 1,
	CMD_NOT_FOUND = 0,
	NO_FILE = -1,
	NO_PERM = -2,
	INV_ARGS = -3,
	NO_MEMORY = -4,
	PIPE_ERR = -5,
	DUP_ERR = -6,
	FORK_ERR = -7,
	NO_PATH = -8,
	CMD_FAIL = -9
};

/* Struct to store fds and linked list */
typedef struct s_pipexdata
{
	int		in_fd;
	int		out_fd;
	char	**env_path;
	int		here_doc;
	t_list	*cmds;
}				t_pipexdata;

/* Struct to store every command with its corresponding file */
typedef struct s_pipexcmd
{
	char	*full_path;
	char	**cmd;
}				t_pipexcmd;

/* Creates new node storing the command and the file */
t_list		*pipex_lstnew(char *full_path, char **cmd);

/* Frees all stuf inside pipexcmd struct */
void		pipex_freecmd(void *node);

/* Finds correct path for a shell command and returns it as a string */
int			find_command(t_pipexdata *data, char *cmd, char **full_path);

/* Fills linked list with the command and full path */
t_list		*parse_commands(int argc, char **argv, t_pipexdata *data);

/* Fills up data struct */
t_pipexdata	*pipex_get_data(int argc, char **argv, int here_doc, char **envp);

/* Prints "command not found" for given command to sderr  */
void		pipex_perror(char *param, int err);

/* Closes open file descriptors and frees struct */
void		*pipex_exit(t_pipexdata *data, char *param, int err, char ***cmd);

/* Creates a set of fds and forks to pass the output to a new command */
void		*pipex(t_pipexdata *data, char **envp);

#endif
