/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_herdoc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sidrissi <sidrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 05:40:07 by sidrissi          #+#    #+#             */
/*   Updated: 2025/04/19 20:49:41 by sidrissi         ###   ########.fr       */
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
	int fd = open("/dev/random", O_RDONLY);
	if (fd < 0)
		return (perror("failed to open /dev/random"), NULL);
	return (get_word(fd));
}
/**/
static int	open_herdoc(char *delimter)
{
	int write_fd;
	char *random_fd;
	char *line;

	random_fd = generate_name();
	write_fd = open(random_fd, O_RDWR | O_CREAT, 0777);

	if (unlink(random_fd) || (write_fd < 0))
		return (perror("faile"), close(write_fd), free(random_fd), -1);
	while (1)
	{
		line = readline("> ");
		if (!line)
			break;
		if (ft_strcmp(line, delimter) == 0)
		{
			free(line);
			break;
		}
		write(write_fd, line, ft_strlen(line));
		write(write_fd, "\n", 1);
		free(line);
	}
	lseek(write_fd, 0, SEEK_SET);
	free(random_fd);
	return (write_fd);// jloul surprise  || should close the prev file descriptor
}
/**/



// static int open_herdoc(char *delimter)
// {
//     int write_fd;
//     char *random_fd;
//     char *line;

//     random_fd = generate_name();
//     write_fd = open(random_fd, O_RDWR | O_CREAT | O_TRUNC, 0600); // Add O_TRUNC and fix permissions
//     if (write_fd < 0)
//     {
//         perror("minishell: open");
//         free(random_fd);
//         return -1;
//     }

//     while (1)
//     {
//         line = readline("> ");
//         if (!line || ft_strcmp(line, delimter) == 0)
//         {
//             free(line);
//             break;
//         }
//         write(write_fd, line, ft_strlen(line));
//         write(write_fd, "\n", 1);
//         free(line);
//     }

//     // Reset file pointer to the start
//     lseek(write_fd, 0, SEEK_SET);
//     return write_fd;
// }



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



























/**************************-******************-****************/

// #include "../include/minishell.h"

// #define NAME_LENGTH 9

// static char *get_word(int fd)
// {
//     char *name = malloc(NAME_LENGTH + 1);
//     if (!name)
//         return (perror("minishell: malloc"), NULL);
    
//     int count = 0;
//     char buffer;
//     while (count < NAME_LENGTH)
//     {
//         if (read(fd, &buffer, 1) < 0)
//         {
//             free(name);
//             return (perror("minishell: read"), NULL);
//         }
//         if (ft_isalnum(buffer))
//             name[count++] = buffer;
//     }
//     name[NAME_LENGTH] = '\0';
//     return name;
// }

// static char *generate_name(void)
// {
//     int fd = open("/dev/urandom", O_RDONLY);
//     if (fd < 0)
//     {
//         perror("minishell: open");
//         return NULL;
//     }
//     char *name = get_word(fd);
//     close(fd);
//     return name;
// }

// static int open_herdoc(char *delimiter)
// {
//     char *random_name = generate_name();
//     if (!random_name)
//         return -1;

//     int fd = open(random_name, O_RDWR | O_CREAT | O_TRUNC, 0600);
//     if (fd < 0)
//     {
//         perror("minishell: open");
//         free(random_name);
//         return -1;
//     }
//     unlink(random_name);
//     free(random_name);

//     char *line;
//     while (1)
//     {
//         line = readline("> ");
//         if (!line || ft_strcmp(line, delimiter) == 0)
//         {
//             free(line);
//             break;
//         }
//         write(fd, line, ft_strlen(line));
//         write(fd, "\n", 1);
//         free(line);
//     }
//     lseek(fd, 0, SEEK_SET);
//     return fd;
// }

// void ft_herdoc(t_token *tokens)
// {
//     t_token *current = tokens;
//     while (current) {
//         if (current->type == HERDOC && current->next && current->next->type == F_HERDOC) {
//             if (!current->fd) {
//                 current->fd = malloc(sizeof(int));
//                 if (!current->fd) {
//                     perror("minishell: malloc");
//                     return;
//                 }
//                 *(current->fd) = -1; // Initialize to -1
//             }
//             int fd = open_herdoc(current->next->value[0]);
//             if (fd != -1)
//             {
//                 *(current->fd) = fd;
//                 printf("===>> current->fd: %d | fd: %d\n", *current->fd, fd);
//                 free(current->fd);
//             } else {
//                 *(current->fd) = -1; // Explicitly mark as invalid
//             }
//         }
//         current = current->next;
//     }
// }




/************-***********-**************-**************************/
// void ft_herdoc(t_token *tokens)
// {
// 	t_token *current;
// 	char *name;

// 	current = tokens;
// 	while (current)
// 	{
// 		if (current->type == HERDOC
// 			&& current->next && current->next->type == F_HERDOC)
// 		{
// 			name = open_herdoc(current->next->value[0]);
// 			if (name)
// 			{
// 				free(current->next->value[0]);
// 				current->next->value[0] = name;
// 			}
// 		}
// 		current = current->next;
// 	}
// }



