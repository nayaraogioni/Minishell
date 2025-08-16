/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 01:10:10 by dopereir          #+#    #+#             */
/*   Updated: 2025/07/19 01:48:11 by dopereir         ###   ########.fr       */
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
# include <sys/ioctl.h>
# include <termios.h>
# include <fcntl.h>
# include <errno.h> //to use errno extern
# include <asm-generic/errno-base.h>
# include "libft/libft.h"
# include "lexer.h"
# include "parser.h"

typedef struct s_parsephase_data
{
	t_command		*commands[MAX_ARGS];
	//t_command		*root;//
	int				n_cmds;
	int				n_spawn_pids;
	int				pd_exit_status;
}			t_parse_data;

//helper structure to helper the exec_parsed_cmds context
typedef struct s_exec_data
{
	int		pipe[2];
	int		fd;
	int		i;
	int		rc;
}			t_exec_data;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}			t_env;

static volatile sig_atomic_t	g_heredoc_sig = 0;
//tokenizer.c
void			print_tokens(t_lexer *shell); //FUNCTION FOR TESTING
void			clear_token(t_token *tokens, int token_count);
int				token_counter(char *str, char delim);
t_token			*split_tokens(char *str, char delim, t_lexer *lexer);
void			lexing_input(t_lexer *lexer, char delim);
char	*join_words(char *a, char *b);
int		is_wordish(t_token *t);
//parser.c
t_command		*init_command(void);
t_command		*parse_function(t_lexer *lexer, t_env *my_env);
t_command		*parse_sequence(t_lexer *lexer, t_env *my_env);
t_command		*parse_pipeline(t_lexer *lexer, t_env *my_env);
t_command		*parse_simple_command(t_lexer *lexer, t_env *env_list);
//parser_utils.c
int				has_pipes(t_lexer *lexer);
int				has_variables(t_lexer *lexer);
int				has_logical_operators(t_lexer *lexer);
int				count_args(t_lexer *lexer);
void			free_command(t_command *cmd);
int				find_next_pipe(t_lexer *lexer, int start);
int				find_next_logical_operator(t_lexer *lexer, int start);
t_lexer			*create_sublexer(t_lexer *lexer, int start, int end);
void			free_sublexer(t_lexer *sublexer);
//collect_commands.c
void			free_parsed_data(t_parse_data *parsed_data);
t_parse_data	format_parsed_data(t_lexer *lexer, t_env *my_env);
void			print_parsed_data(const t_parse_data *pd);
//execute_helpers.c
char			*cmd_path_generator(char *cmd_name, t_env *env);//TESTING
void			replace_env_value(t_env **env, char *key, char *value);
//heredoc_utils.c
void			heredoc_sig_handler(int ignore);
int				handle_all_heredocs(t_parse_data *pd, t_env *env);
//redirections_utils.c
int				set_output(t_command *cmd);
int				set_input(t_command *cmd);
int				set_pipe(int *read_fd, int *write_fd);
//environment_functions.c
void			env_add(t_env **head, char *key, char *value);
void			env_init(t_env **my_env, char **envp);
char			*ft_getenv(t_env *env, char *key);
void			ft_setenv(t_env **env, char *key, char *value);
//enviroment_functions_utils.c
void			ft_unsetenv(t_env **env, char *key);
void			ft_env(t_env *env);
int				ft_unset(char **argv, t_env **env);
int				ft_export(char **argv, t_env **env);
//cleanup_env_list.c
int		count_env(t_env *head); //FOR TESTING
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
//signal_handlers.c
void			sigint_handler(int signo);
//expand_var.c
int				expand_variables(t_lexer *lexer, t_env *my_env);
char			*get_special_var(char *var_name, t_lexer *lexer);
void			update_last_bg_pid(t_lexer *lexer, pid_t pid);
char			*expand_heredoc_line(char *line, t_env *env);
//expand_var_heredoc_helper.c -> exist to helpe the expand_var function
// to expand env vars in the heredoc interactive mode
char			*hd_helper_extract_key(char *line, int *i);
char			*hd_helper_getvalue(char *key, t_env *env);
char			*hd_helper_exp_varname(char *out, char *key, t_env *env);
char			*hd_helper_append_char(char *out, char c);
//exec_commands.c
int				child_run(t_command *cmd, int fd, t_env **env, int c_pipe[2]);
void			parent_run(t_command *cmd, int *fd, int pipe_var[2]);
void			exec_parsed_cmds(t_parse_data *pd, t_env **myenv);
//exec_refactoring.c
int				pre_exec_setups(t_command *cmd, int prev_fd);
int				pre_exec_setups_2(t_command *cmd, int c_pipe[2], int has_pipe);
int				pos_exec_error_codes(char *cmd_name, int errno_code);
int				pre_exec_prep(t_command *cmd, t_env **env, int n, int cp[2]);
void			exit_code(t_parse_data *pd, t_env **env, pid_t pids[MAX_ARGS]);
//expand_var_helpers.c
int		append_char(char **dst, char c);
char	*expand_word_text(const char *src, int quot, t_env *env, int last_status);

#endif
