/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <aperez-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/22 18:52:57 by aperez-b          #+#    #+#             */
/*   Updated: 2021/09/22 21:45:55 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/pipex.h"

int	main(int argc, char **argv)
{
	t_list	*cmds;
	int		i;

	i = 0;
	cmds = NULL;
	if (argc < 5)
		return (ft_putstr_fd("Incorrect number of arguments!\n", 2));
	while (argv[++i] && argv[i + 1])
	{
		if (i == 1)
		{
			ft_lstadd_back(&cmds, pipex_lstnew(argv[i + 1], argv[i]));
			i++;
		}
		else if (i == argc - 2)
		{
			ft_lstadd_back(&cmds, pipex_lstnew(argv[i], argv[i + 1]));
			i++;
		}
		else
			ft_lstadd_back(&cmds, pipex_lstnew(argv[i], NULL));
	}
	pipex_printlist(cmds);
	ft_lstclear(&cmds, pipex_freecmd);
	return (0);
}
