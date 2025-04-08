#include "../include/minishell.h"

// echo 'Hello'$SHELL'World'
/*******************************************************my_logic****************************************************

static char	*expand_string(char *str)
{
	char	*res;
	int		i;
	// int		flag_s;
	int		flag_sp;
	int		dollar_count;
	int		va_start;
	int		var_len;
	char	*var_name;
	char	*var_value;

	var_len = 0;
	res = NULL;
	// flag_s = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
		{
			flag_sp = i;
			// flag_s = 1;
			while (str[i] && str[i] != '\'')
				i++;
			if (str[i] == '\'')
			{
				res = ft_substr(str, (flag_sp + 1), (i - 1));
				// flag_s = 0;
				i++;
			}
		}
		if (str[i] && str[i] != '\'')
		{
			if (str[i] == '$')
			{
				dollar_count = 0;
				while (str[i] == '$')
				{
					dollar_count++;
					i++;
				}
				if (dollar_count % 2 == 1)
				{
					if (ft_isalnum(str[i]) || str[i] == '_')
						va_start = i;
					while (ft_isalnum(str[i]) || str[i] == '_')
					{
						var_len++;
						i++;
					}
					// var_name = malloc(var_len + 1);
					var_name = ft_substr(str, va_start, var_len);
					if (!var_name)
						break ;
					var_value = getenv(var_name);
					if(!var_value)
						var_value = "";
					res = ft_strjoin(res, ft_strncpy(ft_allocate(&res, var_len), str, var_len));
				}
				else
					res = ft_strjoin(res, ft_strdup("$"));
			}
		}
	}
	return (res);
}



void	expand_1(t_token *tokens)
{
	char	*expanded;

	while (tokens)
	{
		expanded = expand_string(tokens->value);
		free(tokens->value);
		tokens->value = expanded;
		tokens = tokens->next;
	}
}

*/

/************************************************************************************************************************







/************************************************************************************************************************
// #include "../include/minishell.h"
// #include <stdlib.h>
// #include <stdio.h>

// /*
// ** Updated expand_string function that:
// **  - Skips expansion within single quotes.
// **  - Removes both single and double quotes from the output.
// **  - Handles consecutive '$' signs: even dollars cancel out (no expansion)
// **    and an odd extra dollar triggers variable expansion.
// */
// static char	*expand_string(char *str)
// {
// 	char	*result;
// 	int		res_index;
// 	int		i;
// 	int		dollar_count;

// 	/* Allocate a buffer for the expanded string.
// 	   For production, consider dynamic reallocation. */
// 	result = malloc(4096);
// 	if (!result)
// 		return (NULL);
// 	res_index = 0;
// 	i = 0;
// 	while (str[i])
// 	{
// 		/* Handle single quotes: no expansion inside, remove quotes */
// 		if (str[i] == '\'')
// 		{
// 			i++;  // skip opening single quote
// 			while (str[i] && str[i] != '\'')
// 				result[res_index++] = str[i++];
// 			if (str[i] == '\'')
// 				i++;  // skip closing single quote
// 		}
// 		/* Handle double quotes: expand variables, but remove the quotes */
// 		else if (str[i] == '"')
// 		{
// 			i++;  // skip opening double quote
// 			while (str[i] && str[i] != '"')
// 			{
// 				if (str[i] == '$')
// 				{
// 					dollar_count = 0;
// 					while (str[i] == '$')
// 					{
// 						dollar_count++;
// 						i++;
// 					}
// 					if (dollar_count % 2 == 1)
// 					{
// 						if ((str[i] >= 'A' && str[i] <= 'Z')
// 							|| (str[i] >= 'a' && str[i] <= 'z')
// 							|| (str[i] == '_'))
// 						{
// 							int var_start = i;
// 							int var_len = 0;
// 							while ((str[i] >= 'A' && str[i] <= 'Z')
// 								|| (str[i] >= 'a' && str[i] <= 'z')
// 								|| (str[i] >= '0' && str[i] <= '9')
// 								|| (str[i] == '_'))
// 							{
// 								var_len++;
// 								i++;
// 							}
// 							char *var_name = malloc(var_len + 1);
// 							if (!var_name)
// 								break;
// 							for (int j = 0; j < var_len; j++)
// 								var_name[j] = str[var_start + j];
// 							var_name[var_len] = '\0';
// 							char *var_value = getenv(var_name);
// 							free(var_name);
// 							if (!var_value)
// 								var_value = "";
// 							for (int j = 0; var_value[j]; j++)
// 								result[res_index++] = var_value[j];
// 						}
// 						else
// 						{
// 							result[res_index++] = '$';
// 						}
// 					}
// 					/* Even number of '$' cancels out (skip them) */
// 				}
// 				else
// 					result[res_index++] = str[i++];
// 			}
// 			if (str[i] == '"')
// 				i++;  // skip closing double quote
// 		}
// 		/* Handle expansion outside any quotes */
// 		else if (str[i] == '$')
// 		{
// 			dollar_count = 0;
// 			while (str[i] == '$')
// 			{
// 				dollar_count++;
// 				i++;
// 			}
// 			if (dollar_count % 2 == 1)
// 			{
// 				if ((str[i] >= 'A' && str[i] <= 'Z')
// 					|| (str[i] >= 'a' && str[i] <= 'z')
// 					|| (str[i] == '_'))
// 				{
// 					int var_start = i;
// 					int var_len = 0;
// 					while ((str[i] >= 'A' && str[i] <= 'Z')
// 						|| (str[i] >= 'a' && str[i] <= 'z')
// 						|| (str[i] >= '0' && str[i] <= '9')
// 						|| (str[i] == '_'))
// 					{
// 						var_len++;
// 						i++;
// 					}
// 					char *var_name = malloc(var_len + 1);
// 					if (!var_name)
// 						break;
// 					for (int j = 0; j < var_len; j++)
// 						var_name[j] = str[var_start + j];
// 					var_name[var_len] = '\0';
// 					char *var_value = getenv(var_name);
// 					free(var_name);
// 					if (!var_value)
// 						var_value = "";
// 					for (int j = 0; var_value[j]; j++)
// 						result[res_index++] = var_value[j];
// 				}
// 				else
// 				{
// 					result[res_index++] = '$';
// 				}
// 			}
// 			/* Even dollar count: do nothing (skip them) */
// 		}
// 		else
// 		{
// 			result[res_index++] = str[i++];
// 		}
// 	}
// 	result[res_index] = '\0';
// 	return (result);
// }

// /*
// ** The expand function iterates over the token list and replaces each token's
// ** value with its expanded version.
// */
// void	expand_1(t_token *tokens)
// {
// 	char	*expanded;

// 	while (tokens)
// 	{
// 		expanded = expand_string(tokens->value);
// 		free(tokens->value);
// 		tokens->value = expanded;
// 		tokens = tokens->next;
// 	}
// }
/************************************************************************************************************************













/************************************************************************************************************************
// #include "../include/minishell.h"


// /* Forward declaration */
// static void handle_dollar(t_expander *exp);

// static void handle_single_quote(t_expander *exp)
// {
//     exp->i++; /* Skip the opening single quote */
//     while (exp->src[exp->i] && exp->src[exp->i] != '\'')
//         exp->dest[exp->j++] = exp->src[exp->i++];
//     if (exp->src[exp->i] == '\'')
//         exp->i++; /* Skip the closing single quote */
// }

// static void handle_double_quote(t_expander *exp)
// {
//     exp->i++; /* Skip the opening double quote */
//     while (exp->src[exp->i] && exp->src[exp->i] != '"')
//     {
//         if (exp->src[exp->i] == '$')
//             handle_dollar(exp);
//         else
//             exp->dest[exp->j++] = exp->src[exp->i++];
//     }
//     if (exp->src[exp->i] == '"')
//         exp->i++; /* Skip the closing double quote */
// }

// static void handle_dollar(t_expander *exp)
// {
//     exp->count = 0;
//     while (exp->src[exp->i] == '$')
//     {
//         exp->count++;
//         exp->i++;
//     }
//     if (exp->count % 2 == 1)
//     {
//         char c = exp->src[exp->i];
//         if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c == '_'))
//         {
//             exp->start = exp->i;
//             exp->var_len = 0;
//             while (exp->src[exp->i] && ((exp->src[exp->i] >= 'A' && exp->src[exp->i] <= 'Z') ||
//                    (exp->src[exp->i] >= 'a' && exp->src[exp->i] <= 'z') ||
//                    (exp->src[exp->i] >= '0' && exp->src[exp->i] <= '9') ||
//                    (exp->src[exp->i] == '_')))
//             {
//                 exp->var_len++;
//                 exp->i++;
//             }
//             char *var_name = malloc(exp->var_len + 1);
//             if (!var_name)
//                 return;
//             for (exp->k = 0; exp->k < exp->var_len; exp->k++)
//                 var_name[exp->k] = exp->src[exp->start + exp->k];
//             var_name[exp->var_len] = '\0';
//             char *var_value = getenv(var_name);
//             free(var_name);
//             if (!var_value)
//                 var_value = "";
//             for (exp->k = 0; var_value[exp->k]; exp->k++)
//                 exp->dest[exp->j++] = var_value[exp->k];
//         }
//         else
//             exp->dest[exp->j++] = '$';
//     }
//     /* Even count: dollars cancel out (do nothing) */
// }

// static char *expand_string(char *str)
// {
//     t_expander exp;

//     exp.src = str;
//     exp.dest = malloc(4096);
//     if (!exp.dest)
//         return (NULL);
//     exp.i = 0;
//     exp.j = 0;
//     exp.count = 0;
//     exp.start = 0;
//     exp.var_len = 0;
//     exp.k = 0;
//     while (exp.src[exp.i])
//     {
//         if (exp.src[exp.i] == '\'')
//             handle_single_quote(&exp);
//         else if (exp.src[exp.i] == '"')
//             handle_double_quote(&exp);
//         else if (exp.src[exp.i] == '$')
//             handle_dollar(&exp);
//         else
//             exp.dest[exp.j++] = exp.src[exp.i++];
//     }
//     exp.dest[exp.j] = '\0';
//     return (exp.dest);
// }

// void expand_1(t_token *tokens)
// {
//     char *expanded;
//     while (tokens)
//     {
//         expanded = expand_string(tokens->value);
//         free(tokens->value);
//         tokens->value = expanded;
//         tokens = tokens->next;
//     }
// }

/************************************************************************************************************************









/************************************************************************************************************************

// #include "../include/minishell.h"
// #include <stdlib.h>
// #include <stdio.h>

// /* This struct holds all the working variables for expansion */
// typedef struct s_expander {
//     char    *src;      /* input string */
//     char    *dest;     /* output buffer */
//     int     i;         /* index in src */
//     int     j;         /* index in dest */
//     int     count;     /* for counting '$' characters */
//     int     start;     /* start index for variable name */
//     int     var_len;   /* length of variable name */
//     int     k;         /* loop index for various loops */
// } t_expander;



// static void handle_dollar(t_expander *exp);
// static char *copy_variable_name(t_expander *exp)
// {
//     int start;
//     int len;
//     char *name;
//     int k;

//     start = exp->i;
//     len = 0;
//     while (exp->src[exp->i] &&
//            ((exp->src[exp->i] >= 'A' && exp->src[exp->i] <= 'Z') ||
//             (exp->src[exp->i] >= 'a' && exp->src[exp->i] <= 'z') ||
//             (exp->src[exp->i] >= '0' && exp->src[exp->i] <= '9') ||
//             (exp->src[exp->i] == '_')))
//     {
//         len++;
//         exp->i++;
//     }
//     name = malloc(len + 1);
//     if (name == NULL)
//     {
//         return (NULL);
//     }
//     k = 0;
//     while (k < len)
//     {
//         name[k] = exp->src[start + k];
//         k++;
//     }
//     name[len] = '\0';
//     return (name);
// }

// static void handle_single_quote(t_expander *exp)
// {
//     exp->i++;  /* Skip opening single quote */
//     while (exp->src[exp->i] && exp->src[exp->i] != '\'')
//     {
//         exp->dest[exp->j] = exp->src[exp->i];
//         exp->j++;
//         exp->i++;
//     }
//     if (exp->src[exp->i] == '\'')
//     {
//         exp->i++;  /* Skip closing single quote */
//     }
// }

// static void handle_double_quote(t_expander *exp)
// {
//     exp->i++;  /* Skip opening double quote */
//     while (exp->src[exp->i] && exp->src[exp->i] != '"')
//     {
//         if (exp->src[exp->i] == '$')
//         {
//             handle_dollar(exp);
//         }
//         else
//         {
//             exp->dest[exp->j] = exp->src[exp->i];
//             exp->j++;
//             exp->i++;
//         }
//     }
//     if (exp->src[exp->i] == '"')
//     {
//         exp->i++;  /* Skip closing double quote */
//     }
// }

// static void handle_dollar(t_expander *exp)
// {
//     char c;
//     char *var_name;
//     char *var_value;
//     int k;

//     exp->count = 0;
//     while (exp->src[exp->i] == '$')
//     {
//         exp->count++;
//         exp->i++;
//     }
//     if (exp->count % 2 == 1)
//     {
//         c = exp->src[exp->i];
//         if (((c >= 'A' && c <= 'Z') ||
//              (c >= 'a' && c <= 'z') ||
//              (c == '_')))
//         {
//             var_name = copy_variable_name(exp);
//             if (var_name == NULL)
//             {
//                 return;
//             }
//             var_value = getenv(var_name);
//             free(var_name);
//             if (var_value == NULL)
//             {
//                 var_value = "";
//             }
//             k = 0;
//             while (var_value[k])
//             {
//                 exp->dest[exp->j] = var_value[k];
//                 exp->j++;
//                 k++;
//             }
//         }
//         else
//         {
//             exp->dest[exp->j] = '$';
//             exp->j++;
//         }
//     }
//     /* Even number of '$' cancel out: do nothing */
// }

// static char *expand_string(char *str)
// {
//     t_expander exp;

//     exp.src = str;
//     exp.dest = malloc(4096);
//     if (exp.dest == NULL)
//     {
//         return (NULL);
//     }
//     exp.i = 0;
//     exp.j = 0;
//     exp.count = 0;
//     while (exp.src[exp.i])
//     {
//         if (exp.src[exp.i] == '\'')
//         {
//             handle_single_quote(&exp);
//         }
//         else if (exp.src[exp.i] == '"')
//         {
//             handle_double_quote(&exp);
//         }
//         else if (exp.src[exp.i] == '$')
//         {
//             handle_dollar(&exp);
//         }
//         else
//         {
//             exp.dest[exp.j] = exp.src[exp.i];
//             exp.j++;
//             exp.i++;
//         }
//     }
//     exp.dest[exp.j] = '\0';
//     return (exp.dest);
// }

// void expand_1(t_token *tokens)
// {
//     char *expanded;

//     while (tokens)
//     {
//         expanded = expand_string(tokens->value);
//         free(tokens->value);
//         tokens->value = expanded;
//         tokens = tokens->next;
//     }
// }

/************************************************************************************************************************/









/************************************************************************************************************************/

// #include "../include/minishell.h"

// typedef struct s_expand {
//     char    *result;
//     int     res_index;
//     int     i;
//     int     dollar_count;
//     int     var_start;
//     int     var_len;
//     char    *var_name;
//     char    *var_value;
// } t_expand;


// static void handle_even_dollars(t_expand *ex)
// {
//     int num_dollars;
//     int j;

//     num_dollars = ex->dollar_count / 2;
//     j = 0;
//     while (j < num_dollars) {
//         ex->result[ex->res_index++] = '$';
//         j++;
//     }
// }

// static void extract_var_name(t_expand *ex, char *str)
// {
//     ex->var_start = ex->i;
//     ex->var_len = 0;
//     while ((ft_isalnum(str[ex->i]) || str[ex->i] == '_'))
// 	{
//         ex->var_len++;
//         ex->i++;
//     }
//     ex->var_name = ft_substr(str, ex->var_start, ex->var_len);
// }

// static void append_var_value(t_expand *ex)
// {
//     int j;

//     j = 0;
//     while (ex->var_value[j]) {
//         ex->result[ex->res_index++] = ex->var_value[j++];
//     }
//     free(ex->var_name);
// }

// static void handle_odd_dollars(t_expand *ex, char *str)
// {
//     if (ft_isalpha(str[ex->i]))
// 	{
//         extract_var_name(ex, str);
//         ex->var_value = getenv(ex->var_name);
//         if (!ex->var_value)
//             ex->var_value = "";
//         append_var_value(ex);
//     }
// 	else
// 	{
//         ex->result[ex->res_index++] = '$';
//     }
// }

// static void process_dollar(t_expand *ex, char *str)
// {
//     ex->dollar_count = 0;
//     while (str[ex->i] == '$') {
//         ex->dollar_count++;
//         ex->i++;
//     }
//     if (ex->dollar_count % 2 == 1)
//         handle_odd_dollars(ex, str);
//     else
//         handle_even_dollars(ex);
// }

// static void handle_single_quote(t_expand *ex, char *str)
// {
//     ex->i++;
//     while (str[ex->i] && str[ex->i] != '\'')
//         ex->result[ex->res_index++] = str[ex->i++];
//     if (str[ex->i] == '\'')
//         ex->i++;
// }

// static void handle_double_quote(t_expand *ex, char *str)
// {
//     ex->i++;
//     while (str[ex->i] && str[ex->i] != '"') {
//         if (str[ex->i] == '$')
//             process_dollar(ex, str);
//         else
//             ex->result[ex->res_index++] = str[ex->i++];
//     }
//     if (str[ex->i] == '"')
//         ex->i++;
// }

// static char *expand_string(char *str)
// {
//     t_expand ex;

//     ex.result = malloc(4096);
//     ex.res_index = 0;
//     ex.i = 0;
//     if (!ex.result)
//         return (NULL);
//     while (str[ex.i]) {
//         if (str[ex.i] == '\'')
//             handle_single_quote(&ex, str);
//         else if (str[ex.i] == '"')
//             handle_double_quote(&ex, str);
//         else if (str[ex.i] == '$')
//             process_dollar(&ex, str);
//         else
//             ex.result[ex.res_index++] = str[ex.i++];
//     }
//     ex.result[ex.res_index] = '\0';
//     return (ex.result);
// }

// void    expand_1(t_token *tokens)
// {
//     char    *expanded;

//     while (tokens) {
//         expanded = expand_string(tokens->value);
//         free(tokens->value);
//         tokens->value = expanded;
//         tokens = tokens->next;
//     }
// }

/************************************************************************************************************************/














/**************************************************25line********************************************************************** */

// #include "../include/minishell.h"
// #include <stdlib.h>

// typedef struct s_expand {
//     char    *res;
//     int     i;
//     int     dollar_count;
//     int     var_len;
//     char    *var_name;
// } t_expand;

// static void append_char(t_expand *ex, char c) {
//     char tmp[2] = {c, '\0'};
//     char *new_res = ft_strjoin(ex->res, tmp);
//     free(ex->res);
//     ex->res = new_res;
// }

// static void handle_even_dollars(t_expand *ex) {
//     int num = ex->dollar_count / 2;
//     char *dollars = malloc(num + 1);
//     if (!dollars) return;
//     memset(dollars, '$', num);
//     dollars[num] = '\0';
//     char *new_res = ft_strjoin(ex->res, dollars);
//     free(ex->res);
//     free(dollars);
//     ex->res = new_res;
// }

// static void extract_var(t_expand *ex, char *str) {
//     ex->var_len = 0;
//     int start = ex->i;
//     while (ft_isalnum(str[ex->i]))
//     {
//         ex->i++;
//         ex->var_len++;
//     }
//     ex->var_name = ft_substr(str, start, ex->var_len);
// }

// static void handle_odd_dollars(t_expand *ex, char *str) {
//     if (ft_isalpha(str[ex->i]))
//     {
//         extract_var(ex, str);
//         char *val = getenv(ex->var_name);
//         char *new_res = ft_strjoin(ex->res, val ? val : "");
//         free(ex->res);
//         free(ex->var_name);
//         ex->res = new_res;
//     } else {
//         append_char(ex, '$');
//     }
// }

// static void process_dollar(t_expand *ex, char *str) {
//     ex->dollar_count = 0;
//     while (str[ex->i] == '$') {
//         ex->dollar_count++;
//         ex->i++;
//     }
//     if (ex->dollar_count % 2)
//         handle_odd_dollars(ex, str);
//     else
//         handle_even_dollars(ex);
// }

// static void handle_single_quote(t_expand *ex, char *str) {
//     ex->i++;
//     while (str[ex->i] && str[ex->i] != '\'') {
//         append_char(ex, str[ex->i]);
//         ex->i++;
//     }
//     if (str[ex->i] == '\'') ex->i++;
// }

// static void handle_double_quote(t_expand *ex, char *str) {
//     ex->i++;
//     while (str[ex->i] && str[ex->i] != '"') {
//         if (str[ex->i] == '$') {
//             process_dollar(ex, str);
//         } else {
//             append_char(ex, str[ex->i]);
//             ex->i++;
//         }
//     }
//     if (str[ex->i] == '"') ex->i++;
// }

// static char *expand_string(char *str)
// {
//     t_expand ex = {0};

//     ex.res = ft_strdup("");
//     if (!ex.res) return NULL;
    
//     while (str[ex.i])
//     {
//         if (str[ex.i] == '\'') handle_single_quote(&ex, str);
//         else if (str[ex.i] == '"') handle_double_quote(&ex, str);
//         else if (str[ex.i] == '$') process_dollar(&ex, str);
//         else {
//             append_char(&ex, str[ex.i]);
//             ex.i++;
//         }
//     }
//     return ex.res;
// }

// void expand_1(t_token *tokens)
// {
//     while (tokens) {
//         char *expanded = expand_string(tokens->value);
//         free(tokens->value);
//         tokens->value = expanded;
//         tokens = tokens->next;
//     }
// }
/************************************************************************************************************************/