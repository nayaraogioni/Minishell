/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 01:10:10 by dopereir          #+#    #+#             */
/*   Updated: 2025/09/03 02:16:06 by dopereir         ###   ########.fr       */
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
	int		seq_mode;
	t_lexer	*lexer_ref;
}			t_exec_data;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}			t_env;

static volatile sig_atomic_t	g_heredoc_sig = 0;
//main.c
void			cleanup_iter(t_lexer *lexer, t_parse_data *pd);
//tokenizer.c
t_token_type	determine_type(char *token_text, int qt_flag);
int				token_counter(char *str, char delim);
int				validate_quotes(char *str);
//tokenizer_helper.c
void			remove_quotes_from_token(t_token *token);
int				add_token(t_token **tokens, int index, t_proc_token *pt);
void			free_tokens_partial(t_token *tokens, int count);
void			lexing_input(t_lexer *lexer, char delim);
char			*join_words(char *a, char *b);
//tokenizer_helper_2.c
int				is_wordish(t_token *t);
void			print_tokens(t_lexer *lexer);//FUNCTION FOR TESTING
bool			bool_changer(bool key);
void			clear_token(t_token *tokens, int token_count);
t_token_type	determine_type_helper(char *token_text);
//split_token.c
int				init_split_tokens(char *str, t_lexer *lexer,
					char **s, t_token **tokens);
char			*handle_variable_token(char *s, int *len);
char			*handle_double_quotes(char *s, int *len);
char			*handle_single_quotes(char *s, char quote_char, int *len);
char			*handle_quoted_token(char *s, int *len, int *qt_flag);
//split_token_helper.c
char			*handle_double_operators(char *s, int *len);
char			*handle_single_operators(char *s, int *len);
char			*handle_regular_token(char *s, char delim, int *len);
int				is_double_operator(char *s);
int				is_single_operator(char *s);
//split_token_helper_2.c
char			*parse_variable_or_quote(char *s, char **start,
					int *len, int *qt_flag);
char			*parse_operators_or_regular(char *s, char delim,
					char **start, int *len);
char			*parse_token(char *s, char delim, t_proc_token *pt);
char			*skip_delimiters(char *s, char delim);
int				should_join_prev(char *tok_begin, char *str, char delim);
//split_token_helper_3.c
t_token			*handle_add_token_error(t_token *tokens, int i,
					t_lexer *lexer);
int				process_single_token(t_token_loop *loop, char *str, char delim,
					t_token **tokens);
t_token			*tokenize_loop(char *str, char delim, t_lexer *lexer,
					t_token *tokens);
t_token			*split_tokens(char *str, char delim, t_lexer *lexer);
int				is_escaped_special_char(char next);
//collect_commands.c
void			free_parsed_data(t_parse_data *parsed_data);
t_parse_data	format_parsed_data(t_lexer *lexer, t_env *my_env);
void			print_parsed_data(const t_parse_data *pd);
//execute_helpers.c
char			*cmd_path_generator(char *cmd_name, t_env *env);//TESTING
int				replace_env_value(t_env **env, char *key, char *value);
//heredoc_utils.c
void			heredoc_sig_handler(int ignore);
int				handle_all_heredocs(t_parse_data *pd, t_env *env);
//redirections_utils.c
int				set_output(t_command *cmd);
int				set_input(t_command *cmd);
int				set_pipe(int *read_fd, int *write_fd);
//environment_functions.c
int				env_add(t_env **head, char *key, char *value);
int				env_init(t_env **my_env, char **envp);
char			*ft_getenv(t_env *env, char *key);
void			ft_setenv(t_env **env, char *key, char *value);
//enviroment_functions_utils.c
void			ft_unsetenv(t_env **env, char *key);
void			ft_env(t_env *env);
int				ft_unset(char **argv, t_env **env);
int				add_export(char *arg, t_env **env, char *value);
int				ft_export(char **argv, t_env **env, t_parse_data *pd);
//cleanup_env_list.c
void			clean_env_list(t_env **env_list);
void			free_env_array(char **arr, int count);
char			**env_to_array(t_env *env);
int				list_lenght(t_env *env_list);
//cleanup_utils.c
void			cleanup_iter(t_lexer *lexer, t_parse_data *pd);
void			free_lexer_tokens(t_lexer *lexer);
//built_ins.c
int				ft_cd(char **argv, t_env **env_list);
int				ft_exit(char *input);
int				run_parent_built(t_command *cmd, t_env **env_list,
					t_parse_data *pd);
bool			is_parent_builtin(char *name);
int				ft_pwd(t_env **env, t_parse_data *pd, t_command *cmd);
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
int				child_run(t_command *cmd, t_exec_data *ctx, t_env **env);
void			parent_run(t_command *cmd, int *fd, int pipe_var[2]);
void			exec_parsed_cmds(t_parse_data *pd, t_env **myenv,
					t_lexer *lexer);
void			handle_child_process(t_command *cmd, t_exec_data *ctx,
					t_env **env);
void			handle_parent_process(t_command *cmd, int *fd, int pipe[2]);
//exec_commands_helper.c
int				spawn_processes(t_parse_data *pd, t_env **env, pid_t *pids,
					t_lexer *lexer);
int				exit_code_helper(t_parse_data *pd, t_env **env);
//exec_refactoring.c
int				pre_exec_setups(t_command *cmd, int prev_fd);
int				pre_exec_setups_2(t_command *cmd, int c_pipe[2], int has_pipe);
int				pos_exec_error_codes(char *cmd_name, int errno_code);
int				pre_exec_prep(t_command *cmd, t_env **env,
					t_parse_data *pd, int cp[2]);
int				exit_code(t_parse_data *pd, t_env **env, pid_t pids[MAX_ARGS]);
//expand_var_helpers.c
int				append_char(char **dst, char c);
char			*expand_word_text(t_env *env, t_token *t);
//expand_var_helpers2.c
int				backslash_helper(int i, char **out, char *src, int quot);
int				process_backslash(int i, char **out, char *src, int quot);
char			*last_exit_expander(int last_status, char **out);
//enviroment_functions_utils2.c
char			*literal_argv_expander(char *eq, char **argv, int *i);
int				export_exception_flag(t_lexer *lexer);
int				export_helper(char *eq, char **argv, t_env **env,
					t_parse_data *pd);
//built_ins_2.c
int				ft_echo(t_parse_data *pd, t_command *cmd);
int				ft_exit_helper(char *ptr);
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
int				find_next_pipe(t_lexer *lexer, int start);
//parser_helper1.c
int				is_wordish_module(t_command *cmd, t_lexer *lexer,
					t_pbuilder *pb);
char			*parse_next_argument(t_lexer *lexer, int *i);
int				redir_in_module(t_command *cmd, t_lexer *lexer, t_pbuilder *pb);
int				redir_out_module(t_command *cmd, t_lexer *lexer,
					t_pbuilder *pb);
//parser_helper2.c
int				redir_heredoc_module(t_command *cmd, t_lexer *lexer,
					t_pbuilder *pb);
int				acc_module(t_command *cmd, t_pbuilder *pb);
int				parse_section1(t_command *cmd, t_lexer *lexer, t_pbuilder *pb);
int				finalize_command(t_command *cmd, t_pbuilder *pb);
//parser_helper3.c
t_command		*finalize_and_return(t_command *cmd, t_pbuilder *pb);
int				pre_pipe_parse(t_pipe_data *pipe_data,
					t_command **pipeline_cmd);
int				process_sublexer(t_pipe_data *pd, t_lexer *lexer,
					t_command *pipeline_cmd);
int				process_leaf(t_pipe_data *pp, t_command *pipeline_cmd,
					t_env *env, t_lexer *lexer);
int				pre_seq_parse(t_seq_data *sd, t_command **sequence_cmd);
//parser_helper4.c
int				process_sequence_segment(t_seq_data *sd,
					t_command *sequence_cmd, t_lexer *lexer, t_env *env);
//parser_utils2.c
void			free_and_null(char **ptr);
void			free_string_array(char **arr, int max);
void			free_command_array(t_command **arr, int count, int max);
void			free_command_ptr(t_command **ptr);
void			free_command(t_command *cmd);
//parser_utils3.c
void			free_sublexer(t_lexer *sublexer);
int				find_next_logical_operator(t_lexer *lexer, int start);
t_lexer			*create_sublexer(t_lexer *lexer, int start, int end);
void			setup_command_defaults(t_command *cmd);

#endif
