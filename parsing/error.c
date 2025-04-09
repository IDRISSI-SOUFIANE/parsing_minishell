/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sidrissi <sidrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 20:50:45 by sidrissi          #+#    #+#             */
/*   Updated: 2025/04/09 12:17:34 by sidrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*

ls >| a should handle this

<>a

ls | > a



<< ls <<

mzal kynin MCHAKILL FI ERROR.C
*/

/*									*/
int	is_redirect(t_keyword type)
{
	return (type == READ_IN || type == WRITE_OUT
			|| type == APPEND || type == HERDOC);
}

int	is_pipe(t_keyword type)
{
	return (type == PIPE || type == PIPES);
}


int	pipe_check(t_token *prev, t_token *next)
{
	if (!prev || !next)
		return (1);
	if (next->type == PIPE)
		return (1);
	if (is_redirect(prev->type))
		return (1);
	return (0);
}

int	redierct_check(t_token *prev, t_token *next)
{
	if (!next || next->type != WORD)
		return (1);
	else if (next->type == WORD && prev && is_pipe(prev->type));
	else if (prev && (is_redirect(prev->type) || is_pipe(prev->type))
			&& next->type != WORD)
		return (1);
	// else if (prev && (!is_redirect(prev->type) || !is_pipe(prev->type || (prev->value != ".")))
	// 		&& next->type == WORD)
	// 	return (syntax_error());  // ls | /> a
	return (0);
}

int	is_invalide(t_token *token, char **value)
{
	int	count;

	count = 0;
	count = ft_strlen(value[0]);
	if (token->type == PIPES)
		return (1);
	else if (count > 2)
		return (1);
	return (0);
}

int	error(t_token *tokens)
{
	t_token	*current;
	t_token	*prev;
	t_token	*next;

	current = tokens;
	prev = NULL;
	if (is_pipe(current->type))
		return (ft_putstr_fd(ERROR, STDERR_FILENO), 1);
	while (current)
	{
		next = current->next;
		if (is_pipe(current->type))
			if (pipe_check(prev, next) || is_invalide(current, current->value))
				return (ft_putstr_fd(ERROR, STDERR_FILENO), 1);
		if (is_redirect(current->type))
			if (redierct_check(prev, next) || is_invalide(current, current->value))
			return (ft_putstr_fd(ERROR, STDERR_FILENO), 1);
		prev = current;
		current = next;
	}
	if (prev && (is_redirect(prev->type) || is_pipe(prev->type)))
	return (ft_putstr_fd(ERROR, STDERR_FILENO), 1);
	return (0);
}
/*					*/