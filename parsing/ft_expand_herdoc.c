/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_herdoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sidrissi <sidrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 17:54:07 by sidrissi          #+#    #+#             */
/*   Updated: 2025/04/21 19:48:17 by sidrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void append_char(t_expand *ex, char c)
{
	char *new_res;
	char tmp[2];

	tmp[0] = c;
	tmp[1] = '\0';
	new_res = ft_strjoin(ex->res, tmp);
	free(ex->res);
	ex->res = new_res;
}

static void handle_even_dollars(t_expand *ex)
{
	int num;
	char *dollars;
	char *new_res;

	num = ex->dollar_count / 2;
	dollars = malloc(num + 1);
	if (!dollars)
		return;
	ft_memset(dollars, '$', num);
	dollars[num] = '\0';
	new_res = ft_strjoin(ex->res, dollars);
	free(ex->res);
	free(dollars);
	ex->res = new_res;
}

static void extract_var(t_expand *ex, char *str)
{
	int start;

	ex->var_len = 0;
	start = ex->i;
	while (ft_isalnum(str[ex->i]) || str[ex->i] == '_')
	{
		ex->i++;
		ex->var_len++;
	}
	ex->var_name = ft_substr(str, start, ex->var_len);
}

static void handle_odd_dollars(t_expand *ex, char *str)
{
	char *val;
	char *new_res;

	if (ft_isalnum(str[ex->i]) || str[ex->i] == '_')
	{
		extract_var(ex, str);
		val = getenv(ex->var_name);
		if (!val)
			val = "";
		new_res = ft_strjoin(ex->res, val);
		free(ex->res);
		free(ex->var_name);
		ex->res = new_res;
	}
}

static void process_dollar(t_expand *ex, char *str)
{
	ex->dollar_count = 0;
	while (str[ex->i] == '$')
	{
		ex->dollar_count++;
		ex->i++;
	}
	if (ex->dollar_count % 2)
		handle_odd_dollars(ex, str);
	else
		handle_even_dollars(ex);
}


static void handle(t_expand *ex, char *str)
{
	// ex->i++;
	// printf("d_str[ex->i]: %c\n", str[ex->i] );

	while (str[ex->i])
	{
		if (str[ex->i] == '$')
		{
			process_dollar(ex, str);
		}
		else
		{
			append_char(ex, str[ex->i]);
			ex->i++;
		}	
	}
	if (str[ex->i] == '"')
		ex->i++;
}

static char **split(t_expand *ex, int *flag)
{
	char **result;

	if (*flag == 0)
	{
		result = ft_split(ex->res, ' ');
		free(ex->res);
	}
	else
	{
		result = malloc(2 * sizeof(char *));
		if (!result)
		{
			free(ex->res);
			return (NULL);
		}
		result[0] = ex->res;
		result[1] = NULL;
	}
	return (result);
}

char **ft_expand_herdoc(char *str, int *flag)
{
	t_expand ex;

	ft_memset(&ex, 0, sizeof(ex));
	ex.res = ft_strdup("");
	if (!ex.res)
		return (NULL);
	while (str[ex.i])
	{
		
		handle(&ex, str);
		if (str[ex.i] == '$')
			process_dollar(&ex, str);
		else
		{
			append_char(&ex, str[ex.i]);
			ex.i++;
		}
	}
	return (split(&ex, flag));
}
