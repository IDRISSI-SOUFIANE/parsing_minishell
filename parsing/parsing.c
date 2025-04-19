/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sidrissi <sidrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 22:43:23 by sidrissi          #+#    #+#             */
/*   Updated: 2025/04/19 20:48:34 by sidrissi         ###   ########.fr       */
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

// void add_redirection(t_data *current, t_token **tokens)
// {
// 	t_redir *new_redir;
// 	t_redir *last;
// 	t_token *token;

// 	token = *tokens;
// 	new_redir = malloc(sizeof(t_redir));
// 	if (!new_redir)
// 		return ;
// 	if (token->type == F_HERDOC)
// 		new_redir->fd = token->fd;
// 	else
// 	{
// 		new_redir->name = ft_strdup(token->value[0]);
// 		new_redir->fd  = -101; // should do handle line 25
// 	}
// 	new_redir->type = token->type;
// 	new_redir->next = NULL;

// 	if (!current->file)
// 		current->file = new_redir;
// 	else
// 	{
// 		last = current->file;
// 		while (last->next)
// 			last = last->next;
// 		last->next = new_redir;
// 	}
// }



// void add_redirection(t_data *current, t_token **tokens)
// {
//     t_redir *new_redir;
//     t_redir *existing = NULL;
//     t_token *token = *tokens;

//     new_redir = malloc(sizeof(t_redir));
//     if (!new_redir)
//         return;

//     // Initialize new_redir based on token type
//     if (token->type == F_HERDOC) {
//         new_redir->fd = token->fd;  // Heredoc uses fd
//         new_redir->name = NULL;
//     } else {
//         new_redir->name = ft_strdup(token->value[0]);  // Other redirs use name
//         new_redir->fd = -1;  // Default invalid fd
//     }
//     new_redir->type = token->type;
//     new_redir->next = NULL;

//     // Check if a redir of the same type already exists
//     t_redir *current_redir = current->file;
//     while (current_redir) {
//         if (current_redir->type == new_redir->type) {
//             existing = current_redir;
//             break;
//         }
//         current_redir = current_redir->next;
//     }

//     if (existing) {
//         // Replace existing redirection
//         if (existing->type == F_HERDOC) {
//             existing->fd = new_redir->fd;  // Update fd for heredoc
//         } else {
//             free(existing->name);          // Free old name
//             existing->name = new_redir->name;  // Assign new name
//         }
//         free(new_redir);  // No longer needed
//     } else {
//         // Append new redirection to the list
//         if (!current->file) {
//             current->file = new_redir;
//         } else {
//             t_redir *last = current->file;
//             while (last->next) last = last->next;
//             last->next = new_redir;
//         }
//     }
// }



// // Helper 1: Create new redirection node
// static t_redir *create_new_redir(t_token *token)
// {
//     t_redir *new_redir = malloc(sizeof(t_redir));
//     if (!new_redir) return NULL;
//     new_redir->type = token->type;
//     new_redir->name = (token->type == F_HERDOC) ? NULL : ft_strdup(token->value[0]);
//     new_redir->fd = (token->type == F_HERDOC) ? token->fd : -1;
//     new_redir->next = NULL;
//     return new_redir;
// }


static t_redir *create_new_redir(t_token *token)
{
    t_redir *new_redir = malloc(sizeof(t_redir));
    if (!new_redir)
        return NULL;

    new_redir->type = token->type;
    new_redir->next = NULL;

    if (token->type == F_HERDOC)
	{
        new_redir->name = NULL;
        new_redir->fd = token->fd;
    }
	else
	{
        new_redir->name = ft_strdup(token->value[0]);
        new_redir->fd = -1;
    }

    return (new_redir);
}

// Helper 2: Find last occurrence of redirection type
static t_redir **find_last_redir_of_type(t_redir **head, t_keyword type)
{
    t_redir **last = NULL;
    while (*head)
	{
        if ((*head)->type == type)
			last = head;
        head = &(*head)->next;
    }
    return (last);
}

// Updated replace_or_append_redir
// static void replace_or_append_redir(t_redir **last, t_redir **head, t_redir *new_redir)
// {
//     if (last && *last)
// 	{
//         // Replace existing redirection
//         if ((*last)->type == F_HERDOC)
// 		{
//             close((*last)->fd);
//             (*last)->fd = new_redir->fd;
//         }
// 		else
// 		{
//             free((*last)->name);
//             (*last)->name = new_redir->name;
//         }
//         free (new_redir);
//     }
// 	else
// 	{
//         // Append to end of list
//         t_redir **p = head;
//         while (*p) p = &(*p)->next;
//         *p = new_redir;
//     }
// }
static void replace_or_append_redir(t_redir **last, t_redir **head, t_redir *new_redir)
{
    if (last != NULL && *last != NULL) {
        // Close existing heredoc FD before replacing
        if ((*last)->type == F_HERDOC && (*last)->fd >= 0) {
            close((*last)->fd);
        }
        (*last)->fd = new_redir->fd;
        free(new_redir);
    } else {
        // Append new redirection
        t_redir **current = head;
        while (*current != NULL) {
            current = &(*current)->next;
        }
        *current = new_redir;
    }
}

// Simplified add_redirection
void add_redirection(t_data *current, t_token **tokens)
{
    t_redir *new_redir = create_new_redir(*tokens);
    if (!new_redir) return;
    
    t_redir **head = &current->file;
    t_redir **last_match = find_last_redir_of_type(head, new_redir->type);
    
    replace_or_append_redir(last_match, head, new_redir);
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
	i = 0;
	new_args[count] = ft_strdup(arg);
	new_args[count + 1] = NULL;
	free(current->args);
	current->args = new_args;
}

void	ft_check(t_data *current, t_token *temp)
{
	int	j;
	int i;

	i = 0;
	while (temp->value[i])
		i++ ;
	if (i > 1)
	{
		j = 0;
		while (j < i)
		{
			add_argument(current, temp->value[j]);
			j++;
		}
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
		{
			current->cmd = ft_strdup(temp->value[0]);
			add_argument(current, temp->value[0]);
		}
		else if (temp->type == WORD)
			ft_check(current, temp);
		else if (temp->type >= FREAD_IN && temp->type <= F_APPEND)
			add_redirection(current, &temp);
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



/*



*/