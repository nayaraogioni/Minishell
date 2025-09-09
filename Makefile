NAME = minishell

CC = cc
FLAGS = -Wall -Wextra -Werror -g -O0
INCLUDES = -I. -Ilibft

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

SOURCES =	main.c \
			built_ins_2.c \
			built_ins.c \
			cleanup_env_list.c \
			cleanup_utils.c \
			collect_commands.c \
			environment_functions_utils.c \
			environment_functions_utils2.c \
			environment_functions_utils_3.c \
			environment_functions.c \
			error_handlers.c \
			exec_commands.c \
			exec_commands_helper.c \
			exec_refactoring.c \
			execute_helpers.c \
			expand_var_helpers.c \
			expand_var_helpers2.c \
			expand_var_heredoc_helper.c \
			expand_var.c \
			export_utils.c \
			export_utils_2.c \
			heredoc_utils.c \
			parser_helper1.c \
			parser_helper2.c \
			parser_helper3.c \
			parser_helper4.c \
			parser_utils.c \
			parser_utils2.c \
			parser_utils3.c \
			parser.c \
			redirections_utils.c \
			signal_handlers.c \
			split_token.c \
			split_token_helper.c \
			split_token_helper_2.c \
			split_token_helper_3.c \
			tokenizer_helper.c \
			tokenizer_helper_2.c \
			tokenizer.c \
			write_error.c \

OBJ = $(SOURCES:.c=.o)

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	@$(CC) $(FLAGS) $(OBJ) -Llibft -lft -lreadline -o $(NAME)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

%.o: %.c minishell.h lexer.h parser.h
	$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJ)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
