NAME = minishell

CC = cc
FLAGS = -Wall -Wextra -Werror
INCLUDES = -I. -Ilibft

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

SOURCES = main.c tokenizer.c parser.c parser_utils.c built_ins.c error_handlers.c \
		collect_commands.c exec_commands.c environment_functions.c cleanup_env_list.c \
		execute_helpers.c redirections_utils.c environment_functions_utils.c \
		heredoc_utils.c expand_var.c expand_var_heredoc_helper.c signal_handlers.c \
		exec_refactoring.c expand_var_helpers.c \

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
