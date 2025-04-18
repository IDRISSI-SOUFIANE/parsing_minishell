/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sidrissi <sidrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 23:54:18 by sidrissi          #+#    #+#             */
/*   Updated: 2025/04/18 14:42:42 by sidrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/**/
int sp(char c)
{
	return (c == ' ' || c == '>' || c == '<' || c == '|');
}

static void handle_quotes(t_quote_data *data, t_initalize *init)
{
    init->quote = data->line[*(data->i)];
    init->j = *(data->i);
    (*(data->i))++;
    while (data->line[*(data->i)] && data->line[*(data->i)] != init->quote)
        (*(data->i))++;
    if (data->line[*(data->i)] == init->quote)
    {
        init->res = ft_substr(data->line, init->j, *(data->i) - init->j + 1);
        if (!init->res)
            return ;
        if (init->j == 0)
        {
            free(*(data-> buffer));
            *(data->buffer) = init->res;
        }
        else
        {
            init->new_buffer = ft_strjoin(*(data->buffer), init->res);
            free(init->res);
            free(*(data->buffer));
            *(data->buffer) = init->new_buffer;
        }
        (*(data->i))++;
    }
}



static void handle_words(char *line, int *i, char **buffer)
{
	int	j;
    char    *new_word;
    char    *new_buffer;
	
	j = *i;
	while (line[*i] && !sp(line[*i]) && line[*i] != '\'' && line[*i] != '\"')
		(*i)++;
    new_word = ft_substr(line, j, (*i - j));
    if (!new_word)
        return ;
    if (*buffer == NULL)
        *buffer = new_word;
    else
    {
        new_buffer = ft_strjoin(*buffer, new_word);
        free(new_word);
        free(*buffer);
        *buffer = new_buffer;
    }
}

static	t_initalize	*ft_initialize()
{
	t_initalize	*init;

	init = malloc(sizeof(t_initalize));
	if (NULL == init)
		return (NULL);
	init->j = 0;
	init->new_buffer = NULL;
	init->res = NULL;
	init->quote = '\0';
	return (init);
}

static void handle_mixed_words(char *line, int *i, t_token **head)
{
	char *buffer;
	t_initalize	*init;

    buffer = NULL;
	init = ft_initialize();
	while (line[*i] && !sp(line[*i]))
	{
		if (line[*i] == '\'' || line[*i] == '\"')
			handle_quotes(&(t_quote_data){line, i, &buffer}, init);
		else
			handle_words(line, i, &buffer);
	}
    if (buffer)
		ft_lstadd_back(head, ft_lstnew(buffer, WORD));
	free(init);
}

static void handle_special_chars(char *line, int *i, t_token **head)
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
	else
		free(sp_char);
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

/**/

/*						check again				

#include "../include/minishell.h"

int sp(char c)
{
    return (c == ' ' || c == '>' || c == '<' || c == '|');
}

static void handle_quotes(t_quote_data *data, t_initalize *init)
{
    init->quote = data->line[*(data->i)];
    init->j = *(data->i);
    (*(data->i))++;
    while (data->line[*(data->i)] && data->line[*(data->i)] != init->quote)
        (*(data->i))++;
    if (data->line[*(data->i)] == init->quote)
    {
        init->res = ft_substr(data->line, init->j, *(data->i) - init->j + 1);
        if (!init->res)
            return;
        if (init->j == 0)
        {
            free(*(data->buffer));
            *(data->buffer) = init->res;
        }
        else
        {
            init->new_buffer = ft_strjoin(*(data->buffer), init->res);
            free(init->res);
            free(*(data->buffer));
            *(data->buffer) = init->new_buffer;
        }
        (*(data->i))++;
    }
}

static void handle_words(char *line, int *i, char **buffer)
{
    int j = *i;
    char *new_word;
    char *new_buffer;

    while (line[*i] && !sp(line[*i]) && line[*i] != '\'' && line[*i] != '\"')
        (*i)++;
    new_word = ft_substr(line, j, *i - j);
    if (!new_word)
        return;
    if (*buffer == NULL)
        *buffer = new_word;
    else
    {
        new_buffer = ft_strjoin(*buffer, new_word);
        free(new_word);
        free(*buffer);
        *buffer = new_buffer;
    }
}

static t_initalize *ft_initialize(void)
{
    t_initalize *init = malloc(sizeof(t_initalize));
    if (!init)
        return NULL;
    init->res = NULL;
    init->new_buffer = NULL;
    init->quote = '\0';
    init->j = 0;
    return init;
}

static void handle_mixed_words(char *line, int *i, t_token **head)
{
    char *buffer = NULL;
    t_initalize *init = ft_initialize();
    
    if (!init)
        return;

    while (line[*i] && !sp(line[*i]))
    {
        if (line[*i] == '\'' || line[*i] == '\"')
            handle_quotes(&(t_quote_data){line, i, &buffer}, init);
        else
            handle_words(line, i, &buffer);
    }
    
    if (buffer)
        ft_lstadd_back(head, ft_lstnew(buffer, WORD));
    
    free(init);
}

static void handle_special_chars(char *line, int *i, t_token **head)
{
    char *sp_char;
    int j = *i;

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
    else
        free(sp_char);
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
*/




/*
int sp(char c)
{
    return (c == ' ' || c == '>' || c == '<' || c == '|');
}

void handle_quotes(t_quote_data *data)
{
    char quote;
    char *res;
    char *new_buffer;
    int j;

    quote = data->line[*(data->i)];
    j = *(data->i);
    (*(data->i))++;

    while (data->line[*(data->i)] && data->line[*(data->i)] != quote)
        (*(data->i))++;

    if (data->line[*(data->i)] == quote)
    {
        res = ft_substr(data->line, j, *(data->i) - j + 1);
        if (!res)
            return;

        if (j == 0)
        {
            // free(data->buffer);  // Free previous buffer if exists
            data->buffer = res;
        }
        else
        {
            new_buffer = ft_strjoin(data->buffer, res);
            // free(res);
            // free(data->buffer);
            data->buffer = new_buffer;
        }
        (*(data->i))++;
    }
}

void handle_words(char *line, int *i, char **buffer)
{
    int j = *i;
    char *new_word;
    char *new_buffer;

    while (line[*i] && !sp(line[*i]) && line[*i] != '\'' && line[*i] != '\"')
        (*i)++;

    new_word = ft_substr(line, j, *i - j);
    if (!new_word)
        return;

    if (*buffer == NULL)
    {
        *buffer = new_word;
    }
    else
    {
        new_buffer = ft_strjoin(*buffer, new_word);
        // free(new_word);
        // free(*buffer);
        *buffer = new_buffer;
    }
}

void handle_mixed_words(char *line, int *i, t_token **head)
{
    char *buffer = NULL;

    while (line[*i] && !sp(line[*i]))
    {
        if (line[*i] == '\'' || line[*i] == '\"')
            handle_quotes(&(t_quote_data){line, i, buffer, NULL});
        else
            handle_words(line, i, &buffer);
    }

    if (buffer)
    {
        ft_lstadd_back(head, ft_lstnew(buffer, WORD));
        // buffer is now owned by the token, don't free here
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
    else
        free(sp_char);  // Free if not used
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
*/






/*
		res = NULL;
		if (j == 0)
		{
			res = ft_substr(data->line, j, *(data->i));
			data->buffer = res;
			printf("\n!!==> enter here\n");
		}
		else
		{
			printf("\n!!==> enter here\n");
			printf("{res: %s}\n", res);
			while (1);
			data->buffer = ft_strjoin(res, data->buffer);
		}
*/


/*
		res = ft_substr(data->line, j, (*(data->i)) + 1);
		// printf("[ %d ]", j);
		if (j == 0)
		{
			
			printf("==!=> %s\n", res);
			res = ft_substr(res, j, (*(data->i)) + 1); //  ft_substr(data->line, j, (*(data->i)) + 1);
			data->buffer = res;
			// free(res);
		}
		else
		{
			printf("\n!!==> enter here\n");
			printf("{res: %s}\n", res);
			// while (1);
			data->buffer = ft_strjoin(res, data->buffer);
		}
*/