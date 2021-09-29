/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_tools.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/05 17:32:34 by aperez-b          #+#    #+#             */
/*   Updated: 2021/09/29 18:27:38 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_TOOLS_H

# define LIBFT_TOOLS_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <ctype.h>

/* Implementation of linked list */
typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;

/* Returns length of a string */
size_t		ft_strlen(const char *s);

/* Concatenates two strings ensuring it ends with \0 */
size_t		ft_strlcat(char *dst, const char *src, size_t size);

/* Copies n - 1 bytes from a source string to a destination string */
size_t		ft_strlcpy(char *dst, const char *src, size_t size);

/* Prints string to the given file descriptor */
int			ft_putstr_fd(char *s, int fd);

/* Splits a string according to a given separator character */
char		**ft_split(char const *s, char c);

/* Concatenates two strings allocating enough space first */
char		*ft_strjoin(char const *s1, char const *s2);

/* Copies from the n-th char of a string */
char		*ft_substr(char const *s, unsigned int start, size_t len);

/* Saves enoug space and duplicates a string */
char		*ft_strdup(const char *s);

/* Tries to find a substring (needle) in a second string (haystack)
 * before the n-th char is reached */
char		*ft_strnstr(const char *big, const char *little, size_t len);

/* Adds new node at the end of the linked list */
void		ft_lstadd_back(t_list **lst, t_list *newnode);

/* Deletes a given element and every element after that */
void		ft_lstclear(t_list **lst, void (*del)(void*));

/* Deletes, frees, and applies function del to content of a given node */
void		ft_lstdelone(t_list *lst, void (*del)(void*));

/* Creates new node allocating with malloc */
t_list		*ft_lstnew(void *content);

/* Returns number of elements of linked list */
int			ft_lstsize(t_list *lst);

/* Returns the node at the given index or null */
t_list		*ft_lstget_at(t_list *lst, int i);

/* Frees all allocated memory from a matrix (array of arrays) */
void		ft_free_matrix(char ***m);

#endif
