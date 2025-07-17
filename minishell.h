/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayara <nayara@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 01:10:10 by dopereir          #+#    #+#             */
/*   Updated: 2025/07/06 22:07:33 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# define MAX_ARGS 1024

# include <signal.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <string.h>
# include <time.h>
# include <stdbool.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include "libft/libft.h"
# include "lexer.h"
# include "parser.h"

typedef struct s_parsephase_data
{
	t_command		*commands[MAX_ARGS];
	t_command		*root;
	int				n_cmds;
}			t_parse_data;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}			t_env;

static volatile sig_atomic_t	g_heredoc_sig = 0;

void			print_tokens(t_lexer *shell); //TEMP FUNCTION FOR TESTING
void			clear_token(t_token *tokens, int token_count);
int				token_counter(char *str, char delim);
t_token			*split_tokens(char *str, char delim, t_lexer *lexer);
void			lexing_input(t_lexer *lexer, char delim);
//parser.c
t_command		*init_command(void);
t_command		*parse_function(t_lexer *lexer);
t_command		*parse_sequence(t_lexer *lexer);
t_command		*parse_pipeline(t_lexer *lexer);
t_command		*parse_simple_command(t_lexer *lexer);
int				has_pipes(t_lexer *lexer);
int				has_logical_operators(t_lexer *lexer);
int				count_args(t_lexer *lexer);
void			free_command(t_command *cmd);
int				find_next_pipe(t_lexer *lexer, int start);
int				find_next_logical_operator(t_lexer *lexer, int start);
t_lexer			*create_sublexer(t_lexer *lexer, int start, int end);
void			free_sublexer(t_lexer *sublexer);

t_command	*init_command(void);
t_command	*parse_function(t_lexer *lexer);
t_command	*parse_sequence(t_lexer *lexer);
t_command	*parse_pipeline(t_lexer *lexer);
t_command	*parse_simple_command(t_lexer *lexer);
int	has_pipes(t_lexer *lexer);
int	has_logical_operators(t_lexer *lexer);
int	has_variables(t_lexer *lexer);
int	count_args(t_lexer *lexer);
void	free_command(t_command *cmd);
int	find_next_pipe(t_lexer *lexer, int start);
int	find_next_logical_operator(t_lexer *lexer, int start);
t_lexer	*create_sublexer(t_lexer *lexer, int start, int end);
void	free_sublexer(t_lexer *sublexer);

//collect_commands.c
void			free_parsed_data(t_parse_data *parsed_data);
t_parse_data	format_parsed_data(t_lexer *lexer);
void			print_parsed_data(const t_parse_data *pd);
//execute_helpers.c
char			*cmd_path_generator(char	*cmd_name);
//exec_commands.c
void			exec_parsed_cmds(t_parse_data *pd, t_env **env_list);
//heredoc_handler.c
int				set_heredoc(char *delim);
//redirections_utils.c
int				set_output(t_command *cmd);
int				set_input(t_command *cmd);
int				set_pipe(int *read_fd, int *write_fd);
void			heredoc_sig_handler(int ignore);
//environment_functions.c
void			env_add(t_env **head, char *key, char *value);
void			env_init(t_env **my_env, char **envp);
char			*ft_getenv(t_env *env, char *key);
void			ft_setenv(t_env **env, char *key, char *value);
//enviroment_functions_utils.c
void			ft_unsetenv(t_env **env, char *key);
void			ft_env(t_env *env); // for env command
int				ft_unset(char **argv, t_env **env); //for unset command
int				ft_export(char **argv, t_env **env); //for export command
//cleanup_env_list.c
void			clean_env_list(t_env **env_list);
void			free_env_array(char **arr, int count);
char			**env_to_array(t_env *env);
int				list_lenght(t_env *env_list);
//built_ins.c
int				ft_cd(char **argv, t_env **env_list);
int				run_parent_built(t_command *cmd, t_env **env_list);
bool			is_parent_builtin(char *name);
bool			is_any_builtin(char *name);
//error_handlers.c
char			*cmd_type_str(t_token_type type);
void			argument_redirs_error(t_token_type type);

int				set_heredoc(char *delim);


//expand_var.c
int	expand_variables(t_lexer *lexer);
char	*get_special_var(char *var_name, t_lexer *lexer);
void	update_last_bg_pid(t_lexer *lexer, pid_t pid);

#endif
