/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sidrissi <sidrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 22:43:23 by sidrissi          #+#    #+#             */
/*   Updated: 2025/04/08 21:07:25 by sidrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_data *ft_lstnew_p(void)
{
	t_data *new_node = malloc(sizeof(t_data));
	if (!new_node)
		return (NULL);
	new_node->cmd = NULL;
	new_node->args = NULL;
	new_node->file = NULL;
	new_node->next = NULL;
	return (new_node);
}

void add_redirection(t_data *current, t_token *token)
{
	t_redir *new_redir;
	t_redir *last;

	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return ;
	new_redir->name = ft_strdup(token->value[0]);
	new_redir->type = token->type;
	new_redir->next = NULL;

	if (!current->file)
		current->file = new_redir;
	else
	{
		last = current->file;
		while (last->next)
			last = last->next;
		last->next = new_redir;
	}
}

/**/
void add_argument(t_data *current, char *arg)
{
	int count;
	int i;
	char **new_args;

	count = 0;
	i = 0;
	if (current->args)
		while (current->args[count])
			count++;
	new_args = malloc((count + 2) * sizeof(char *));
	if (!new_args)
		return ;
	while (i < count)
	{
		new_args[i] = current->args[i];
		i++;
	}
	new_args[count] = ft_strdup(arg);
	new_args[count + 1] = NULL;
	free(current->args); // ?? ?? ?? ?? ?? ?? ?? ?? 
	current->args = new_args;
}

int	ft_count(char **s)
{
	int	i;

	i = 0;
	while (NULL != s[i])
		i++;
	return (i);
}

char	**duplicate_value_array(char **s)
{
	int		count;
	int		i;
	char	**str;

	if (!s)
		return (NULL);
	count = ft_count(s);
	str = malloc(count + 1);
	if(NULL == str)
		return (NULL);
	i = 0;
	while (i < count)
	{
		str[i] = ft_strdup(s[i]);
		if (!str[i])
		{
			while (i-- > 0)
				free(str[i]);
			free(str);
			return (NULL);
		}
		i++;
	}
	str[count] = NULL;
	return (str);
}

void	ft_check(t_data *current, t_token *temp)
{
	// int i;

	// i = 0;
	// while (temp->value[i])
	// 	i++;
	// if (i > 1)
	// 	current->args = duplicate_value_array(temp->value); //malloc((i + 1) * sizeof(char *));
	// else
	// 	add_argument(current, temp->value[0]);


	int i = 0;
    	while (temp->value[i])
        	i++;
    	if (i > 1)
 	  	{
   	    	// Allocate new args array
   	    	current->args = malloc((i + 1) * sizeof(char *));
   	    	if (!current->args)
   	    		return ; // Handle error
   	     	// Duplicate each string
   	     	for (int j = 0; j < i; j++)
   	     	{
            	current->args[j] = ft_strdup(temp->value[j]);
   		        	if (!current->args[j])
           		{
   	            	// Free allocated memory on failure
   	            	while (j-- > 0)
   	                	free(current->args[j]);
   	            	free(current->args);
   		            	return ;
   	         	}
   	     	}
    	    	current->args[i] = NULL;
    	}
   	 	else
    	    add_argument(current, temp->value[0]);

	
}

t_data *parsing(t_token **tokens)
{
	t_token *temp;
	t_data *lst;
	t_data *current;

	temp = *tokens;
	lst = ft_lstnew_p();
	current = lst;
	while (temp)
	{
		if (temp->type == PIPE)
		{
			current->next = ft_lstnew_p();
			current = current->next;
		}
		else if (temp->type == CMD)
			current->cmd = ft_strdup(temp->value[0]);

		else if (temp->type == WORD)
			ft_check(current, temp);
		else if (temp->type >= FREAD_IN && temp->type <= F_APPEND)
			add_redirection(current, temp);
		temp = temp->next;
	}
	return (lst);
}






/*****************************************/


/*

#include "../include/minishell.h"

t_data *ft_lstnew_p(void)
{
	t_data *new_node = malloc(sizeof(t_data));
	if (!new_node)
		return (NULL);
	new_node->cmd = NULL;
	new_node->args = NULL;
	new_node->file = NULL;
	new_node->next = NULL;
	return (new_node);
}

void add_redirection(t_data *current, t_token *token)
{
	t_redir *new_redir;
	t_redir *last;

	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return ;
	new_redir->name = ft_strdup(token->value[0]);
	new_redir->type = token->type;
	new_redir->next = NULL;

	if (!current->file)
		current->file = new_redir;
	else
	{
		last = current->file;
		while (last->next)
			last = last->next;
		last->next = new_redir;
	}
}

void add_argument(t_data *current, char *arg)
{
	int count;
	int i;
	char **new_args;

	count = 0;
	i = 0;
	if (current->args)
		while (current->args[count])
			count++;
	new_args = malloc((count + 2) * sizeof(char *));
	if (!new_args)
		return ;
	while (i < count)
	{
		new_args[i] = current->args[i];
		i++;
	}
	new_args[count] = ft_strdup(arg);
	new_args[count + 1] = NULL;
	free(current->args); // ?? ?? ?? ?? ?? ?? ?? ?? 
	current->args = new_args;
}

int	ft_count(char **s)
{
	int	i;

	i = 0;
	while (NULL != s[i])
		i++;
	return (i);
}

// char	**duplicate_value_array(char **s)
// {
// 	int		count;
// 	int		i;
// 	char	**str;

// 	if (!s)
// 		return (NULL);
// 	count = ft_count(s);
// 	str = malloc(count + 1);
// 	if(NULL == str)
// 		return (NULL);
// 	i = 0;
// 	while (i < count)
// 	{
// 		str[i] = ft_strdup(s[i]);
// 		if (!str[i])
// 		{
// 			while (i-- > 0)
// 				free(str[i]);
// 			free(str);
// 			return (NULL);
// 		}
// 		i++;
// 	}
// 	str[count] = NULL;
// 	return (str);
// }

// void	ft_check(t_data *current, t_token *temp)
// {
// 	int i;

// 	i = 0;
// 	while (temp->value[i])
// 		i++;
// 	if (i > 1)
// 		current->args = duplicate_value_array(temp->value); //malloc((i + 1) * sizeof(char *));
// 	else
// 		add_argument(current, temp->value[0]);
// }

t_data *parsing(t_token **tokens)
{
	t_token *temp;
	t_data *lst;
	t_data *current;

	temp = *tokens;
	lst = ft_lstnew_p();
	current = lst;
	while (temp)
	{
		if (temp->type == PIPE)
		{
			current->next = ft_lstnew_p();
			current = current->next;
		}
		else if (temp->type == CMD)
			current->cmd = ft_strdup(temp->value[0]);

		// In parsing.c, update the WORD handling:
	else if (temp->type == WORD)
	{
    	int i = 0;
    	while (temp->value[i])
        	i++;
    	if (i > 1)
 	  	{
   	    	// Allocate new args array
   	    	current->args = malloc((i + 1) * sizeof(char *));
   	    	if (!current->args)
   	    		return (NULL); // Handle error
   	     	// Duplicate each string
   	     	for (int j = 0; j < i; j++)
   	     	{
            	current->args[j] = ft_strdup(temp->value[j]);
   		        	if (!current->args[j])
           		{
   	            	// Free allocated memory on failure
   	            	while (j-- > 0)
   	                	free(current->args[j]);
   	            	free(current->args);
   		            	return (NULL);
   	         	}
   	     	}
    	    	current->args[i] = NULL;
    	}
   	 	else
    	    add_argument(current, temp->value[0]);
}


		else if (temp->type >= FREAD_IN && temp->type <= F_APPEND)
			add_redirection(current, temp);
		temp = temp->next;
	}
	return (lst);
}


*/