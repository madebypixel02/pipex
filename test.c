/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <aperez-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/29 20:27:15 by aperez-b          #+#    #+#             */
/*   Updated: 2021/09/29 21:48:14 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/libft_tools.h"
#include "inc/pipex.h"
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
	char	c[1];
	char	*buf;
	char	*temp;
	char	*final;

	final = NULL;
	buf = NULL;
	while (!buf || ft_strncmp("a", buf, ft_strlen(buf) - 1))
	{
		temp = final;
		final = ft_strjoin(final, buf);
		free(temp);
		free(buf);
		buf = NULL;
		*c = 0;
		ft_putstr_fd("heredoc>", 1);
		while(*c != '\n')
		{
			read(0, c, 1);
			temp = buf;
			buf = ft_strjoin(buf, c);
			free(temp);
		}
	}
	dup2(STDIN_FILENO, STDIN_FILENO);
	write(STDIN_FILENO, final, ft_strlen(final));
	free(final);
}
