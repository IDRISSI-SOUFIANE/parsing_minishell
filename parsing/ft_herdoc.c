/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_herdoc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sidrissi <sidrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 05:40:07 by sidrissi          #+#    #+#             */
/*   Updated: 2025/04/21 22:22:47 by sidrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char *get_word(int fd)
{
	char *name;
	int count;
	char buffer;

	name = malloc(name_length + 1);
	if (!name)
		return (close(fd), perror("failed malloc"), NULL);
	count = 0;
	while (count < name_length)
	{
		if (read(fd, &buffer, 1) < 0)
		{
			close(fd);
			perror("read failed\n");
			return (NULL);
		}
		if (ft_isalnum(buffer))
			name[count++] = buffer;
	}
	name[name_length] = '\0';
	close(fd);
	return (name);
}

static char *generate_name()
{
	int fd = open("/dev/urandom", O_RDONLY);
	if (fd < 0)
		return (perror("failed to open /dev/random"), NULL);
	return (get_word(fd));
}

 int	open_herdoc(char *delimter)
{
	int fd;
	char *random_name;
	char *line;
	char	**exp;
	char	*new_line;
	int	n;


	n = 8;

	random_name = generate_name();
	fd = open(random_name, O_RDWR | O_CREAT, 0777);

	if (unlink(random_name) || (fd < 0))
		return (perror("faile"), close(fd), free(random_name), -1);

	while (1)
	{
		line = readline("> ");
		if ((!line) || ft_strcmp(line, delimter) == 0)
		{
			free(line);
			break;
		}
		exp = ft_expand_herdoc(line, &n);
		new_line = exp[0];
		(write(fd, new_line, ft_strlen(new_line)), write(fd, "\n", 1));
		(free(line), free(new_line), free(exp));
	}
	lseek(fd, 0, SEEK_SET);
	return (free(random_name), fd);// jloul surprise  || should close the prev file descriptor
}


void ft_herdoc(t_token **tokens)
{
	t_token *current;
	int		fd_;

	fd_ = -1;
	current = *tokens;
	while (current)
	{
		if (current->type == HERDOC
			&& current->next && current->next->type == F_HERDOC)
		{
			fd_ = open_herdoc(current->next->value[0]);
			if (fd_ != -1)
				current->next->fd = fd_;
		}
		// close(fd_);
		current = current->next;
	}
}

