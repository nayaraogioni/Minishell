# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/10 20:59:32 by dopereir          #+#    #+#              #
#    Updated: 2025/08/22 22:03:18 by dopereir         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = libft.a

Sources = calculate_padding_bonus.c \
		ft_atoi.c \
		ft_bzero.c \
		ft_calloc.c \
		ft_isalnum.c \
		ft_isalpha.c \
		ft_isascii.c \
		ft_isdigit.c \
		ft_isprint.c \
		ft_isspace.c \
		ft_itoa.c \
		ft_itoa_custombase.c \
		ft_lstadd_back.c \
		ft_lstadd_front.c \
		ft_lstclear.c \
		ft_lstdelone.c \
		ft_lstiter.c \
		ft_lstlast.c \
		ft_lstmap.c \
		ft_lstnew.c \
		ft_lstsize.c \
		ft_memchr.c \
		ft_memcmp.c \
		ft_memcpy.c \
		ft_memmove.c \
		ft_memset.c \
		ft_printf.c \
		ft_putchar.c \
		ft_putchar_fd.c \
		ft_putendl_fd.c \
		ft_putnbr_fd.c \
		ft_putstr_fd.c \
		ft_realloc.c \
		ft_split.c \
		ft_strcat.c \
		ft_strchr.c \
		ft_strcmp.c \
		ft_strcpy.c \
		ft_strcspn.c \
		ft_strdup.c \
		ft_striteri.c \
		ft_strjoin_free.c \
		ft_strjoin.c \
		ft_strlcat.c \
		ft_strlcpy.c \
		ft_strlen.c \
		ft_strmapi.c \
		ft_strncat.c \
		ft_strncmp.c \
		ft_strndup.c \
		ft_strnstr.c \
		ft_strncpy.c \
		ft_strrchr.c \
		ft_strspn.c \
		ft_strtok.c \
		ft_strtol.c \
		ft_strtrim.c \
		ft_substr.c \
		ft_tolower.c \
		ft_toupper.c \
		get_next_line.c \
		handle_padding_bonus.c \
		parse_flags.c \
		print_char.c \
		print_hex.c \
		print_int.c \
		print_integer_str_bonus.c \
		print_padding.c \
		print_prefix_bonus.c \
		print_ptr.c \
		print_sign_bonus.c \
		print_string.c \
		print_unsigned.c

Compiler = cc $(CFLAGS) -c

CFLAGS ?= -Wall -Wextra -Werror -fPIE

mylib = ar rc $(NAME)

ranlib = ranlib $(NAME)

remove = rm -f

O_FILES = $(Sources:.c=.o)
O_BONUS = $(Bonus:.c=.o)

GREEN = \033[0;32m
RESET = \033[0m

all: compile_msg $(NAME)

$(NAME): $(O_FILES)
	@$(mylib) $(O_FILES)
	@$(ranlib)

compile_msg:
	@echo "$(GREEN)Compiling libft...$(RESET)"

$(O_FILES): %.o: %.c
	@$(Compiler) -o $@ $<

bonus: $(O_BONUS)
	@$(mylib) $(O_BONUS)
	@$(ranlib)

$(O_BONUS): %.o: %.c
	@$(Compiler) -o $@ $<

clean:
	@$(remove) $(O_FILES) $(O_BONUS)

fclean:	clean
	@$(remove) $(NAME)

re:	fclean	all

.PHONY: clean fclean re bonus
