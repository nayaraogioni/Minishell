/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayara <nayara@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 17:46:12 by nayara            #+#    #+#             */
/*   Updated: 2025/07/08 13:24:43 by nayara           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_last_bg_pid(t_lexer *lexer, pid_t pid)
{
	lexer->last_bg_pid = pid;
}

char	*get_special_var(char *var_name, t_lexer *lexer)
{
	if (ft_strcmp(var_name, "?") == 0)
		return (ft_itoa(lexer->exit_status));
	else if (ft_strcmp(var_name, "$") == 0)
		return (ft_itoa(getppid()));
	else if (ft_strcmp(var_name, "0") == 0)
		return (ft_strdup("minishell"));
	else if (ft_strcmp(var_name, "#") == 0)
		return (ft_strdup("0"));
	else if (ft_strcmp(var_name, "*") == 0)
		return (ft_strdup(""));
	else if (ft_strcmp(var_name, "@") == 0)
		return (ft_strdup(""));
	else if (ft_strcmp(var_name, "!") == 0)
	{
		 if (lexer->last_bg_pid > 0)
		 	return (ft_itoa(lexer->last_bg_pid));
		else
			return (ft_strdup(""));
	}
	return (NULL);
}
int	expand_variables(t_lexer *lexer)
{
	int	i;
	char	*var_name;
	char	*var_value;
	char	*old_text;

	i = 0;
	while (i < lexer->token_count)
	{
		if (lexer->tokens[i].type == T_VAR)
		{
			var_name = lexer->tokens[i].text + 1; // pula o $
			var_value = get_special_var((char *)var_name, lexer);
			if (!var_value)
				var_value = getenv(var_name);
			if (!var_value)
				var_value = ft_strdup(""); // Variável não encontrada
			if (!var_value)
				return (-1);
			old_text = lexer->tokens[i].text;
			lexer->tokens[i].text = ft_strdup(var_value);
			free(old_text);
			lexer->tokens[i].type = T_WORD;
		}
		i++;
	}
	return (0);
}