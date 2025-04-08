/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sidrissi <sidrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 23:54:18 by sidrissi          #+#    #+#             */
/*   Updated: 2025/04/06 12:21:51 by sidrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int sp(char c)
{
	return (c == ' ' || c == '>' || c == '<' || c == '|');
}

void handle_quotes(t_quote_data *data)
{
	char quote;
	int j;

	quote = data->line[*(data->i)];
	j = *(data->i);
	(*(data->i))++;

	while (data->line[*(data->i)] && data->line[*(data->i)] != quote)
		(*(data->i))++;

	if (data->line[*(data->i)] == quote)
	{
		while (j <= *(data->i))
			data->buffer[(*(data->buffer_index))++] = data->line[j++];

		(*(data->i))++;
	}
}
void handle_words(char *line, int *i, char *buffer, int *buffer_index)
{
	while (line[*i] && !sp(line[*i]) && line[*i] != '\'' && line[*i] != '\"')
	{
		buffer[(*(buffer_index))++] = line[*i];
		(*i)++;
	}
}

void handle_mixed_words(char *line, int *i, t_token **head)
{
	char buffer[2025];
	int buffer_index;

	buffer_index = 0;

	while (line[*i] && !sp(line[*i]))
	{
		if (line[*i] == '\'' || line[*i] == '\"')
			handle_quotes(
				&(t_quote_data){line, i, buffer, &buffer_index});
		else
			handle_words(line, i, buffer, &buffer_index);
	}
	if (buffer_index > 0)
	{
		buffer[buffer_index] = '\0';
		ft_lstadd_back(head, ft_lstnew(ft_strdup(buffer), WORD));
	}
}

void handle_special_chars(char *line, int *i, t_token **head)
{
	char *sp_char;
	int j;

	j = *i;
	while (line[*i] && line[*i] == line[j])
		(*i)++;
	sp_char = ft_substr(line, j, *i - j);

	if (ft_cmp(sp_char) == PIPE)
		ft_lstadd_back(head, ft_lstnew(sp_char, PIPE));
	else if (ft_cmp(sp_char) == PIPES)
		ft_lstadd_back(head, ft_lstnew(sp_char, PIPES));
	else if (ft_cmp(sp_char) == WRITE_OUT)
		ft_lstadd_back(head, ft_lstnew(sp_char, WRITE_OUT));
	else if (ft_cmp(sp_char) == APPEND)
		ft_lstadd_back(head, ft_lstnew(sp_char, APPEND));
	else if (ft_cmp(sp_char) == READ_IN)
		ft_lstadd_back(head, ft_lstnew(sp_char, READ_IN));
	else if (ft_cmp(sp_char) == HERDOC)
		ft_lstadd_back(head, ft_lstnew(sp_char, HERDOC));
}

t_token *tokenization(char *line, int i)
{
	t_token *head;

	head = ft_lstnew(ft_strdup("."), START);
	if (!head)
		return (NULL);
	while (line[i])
	{
		if (line[i] == '\'' || line[i] == '\"')
			handle_mixed_words(line, &i, &head);
		else if (sp(line[i]))
		{
			if (line[i] == ' ')
				i++;
			else
				handle_special_chars(line, &i, &head);
		}
		else
			handle_mixed_words(line, &i, &head);
	}
	head = deldum(&head);
	return (head);
}
