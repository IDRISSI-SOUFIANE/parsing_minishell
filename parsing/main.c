/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sidrissi <sidrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 11:15:46 by sidrissi          #+#    #+#             */
/*   Updated: 2025/04/18 18:44:09 by sidrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int check_quotes(char *line, int i, int count_quote)
{
	char quote;

	while (line[i])
	{
		if (line[i] == '\'' || line[i] == '\"')
		{
			quote = line[i];
			count_quote++;
			i++;
			while (line[i] && line[i] != quote)
				i++;
			if (line[i] == quote)
				count_quote++;
			else
				break;
			i++;
		}
		else
			i++;
	}
	if (count_quote % 2)
		return (1);
	return (0);
}

t_token *lexing(char *line)
{
	int		i;
	int		count_quote;
	t_token	*tokens;

	i = 0;
	count_quote = 0;
	if (check_quotes(line, i, count_quote))
		return (ft_putstr_fd("missing quotation\n", 2), NULL);

	tokens = tokenization(line, i);
	if (tokens == NULL)
		return (NULL);
	tokens->fd = 0;
	error(tokens);
	ft_rename(tokens);
	ft_expand(tokens);
	ft_herdoc(tokens);
	return (tokens);
}

void	f()
{
	system("leaks minishell");
}

int main(int ac, char **av, char **env)
{
	// atexit(f);
	(void)ac;
	(void)av;
	(void)env; // I am voiding env cause in expand i am using function getenv()

	char	*line;
	t_token	*tokens;
	t_data	*data;

	while (1)
	{
		line = readline("Minishell: ");
		if (line == NULL)
		{
			// free_data(data);
			// free_tokens(tokens);
			break;
		}
		tokens = lexing(line);
		data = parsing(&tokens);

		// lexing(line);

		//
			// excution();
		//
		/*		printe cause leaks				*/
			// PRINT
			t_data *tmp = data;
			while (tmp)
			{

				printf("\n\n");
				printf("Command: %s\n", tmp->cmd ? tmp->cmd : "(no command)");

				// Print arguments
				printf("Arguments:");
				if (tmp->args)
				{
					for (int i = 0; tmp->args[i]; i++)
						printf(" |%s|", tmp->args[i]);
				}
				printf("\n");
				if (tmp->file)
				{
					while (tmp->file)
					{
						printf("[fname: %d | ftype: %d]\n",  *(tmp->file->fd), tmp->file->type);
						tmp->file = tmp->file->next;
					}
				}

				/*
				
					if (tmp->file)
				{
					while (tmp->file)
					{
						printf("[fd: %s | ftype: %d]\n", 
						(tmp->file->fd) ? *(tmp->file->fd) : "(null)", 
						tmp->file->type);
						tmp->file = tmp->file->next;
					}
				}
				*/

				printf("\n");
				tmp = tmp->next;
			}
		/*				*/
		//
		// t_data *tmp = data;

		// tmp = data;
		// int fd = open (tmp->file->name, O_RDONLY);
		// if(fd < 0)
		// 	printf("soufiane fail\n");
		if (line[0] != '\0')
			add_history(line);
		free(line);
		free_data(data);
		free_tokens(tokens);
	}
	return (0);
}









// void	lexing(char *line)
// {
// 	int		i;
// 	int		count_quote;
// 	t_token	*tokens;

// 	i = 0;
// 	count_quote = 0;
// 	if (check_quotes(line, i, count_quote))
// 		return (ft_putstr_fd("missing quotation\n", 2));

// 	tokens = tokenization(line, i);
// 	if (tokens == NULL)
// 		return ;
// 	// error(tokens);
// 	// ft_rename(tokens);
// 	// ft_expand(tokens);
// 	// ft_herdoc(tokens);

// 	t_token *temp;

// 	temp = tokens;

// 	// printf("@==> %s\n", temp->value[0]);

// 	while (temp)
// 	{
// 		int	i = 0;
// 		if (temp->value)
// 		{
// 			while (temp->value[i])
// 			{
// 					printf("\n | %s |\n", temp->value[i]);
// 					i++;
// 			}
// 		}
// 		temp = temp->next;
// 	}

// }



/*************************************/
//  handle this cas '' '' '' ''  
// add `command` as argument also, cause execve need it


/* problem
	syntax error ;;
*/

/*
there still a problem when i write '           ' => the problem is not print l espace
exept when somethig in single_quote like this => '        .'
so should fix this issue when i have a space in single quote should keep the space '      '
*/

/* 
	problem in herdoc "ok" => khaso irooj bi ok bla ""
	check this in your bash and compare it with real bash
*/

/* problem
<< ok |||| << d
check this in your bash and compare it with real bash
*/

/* problem
	close(fd) => if I close it fd in parsing the executer can't open it again will lose so be carefule and find a solution 
*/


/* problem
	in expand should handle numberes
*/

