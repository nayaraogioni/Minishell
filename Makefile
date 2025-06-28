NAME = minishell

CC = cc
FLAGS = -Wall -Wextra -Werror
INCLUDES = -I. -Ilibft

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

SOURCES = main.c tokenizer.c exec_commands.c execute_helpers.c redirections_utils.c \
		redirections.c parser.c parser_utils.c \

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
