/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 14:47:56 by dopereir          #+#    #+#             */
/*   Updated: 2025/09/02 16:37:52 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdarg.h>
# include <stddef.h>
# include <unistd.h>
# include <limits.h>
# include <stdio.h>
# include <stdlib.h>
# include <ctype.h>
# include <string.h>
# include <fcntl.h>
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 100
# endif

typedef struct s_flags
{
	int		width;
	int		precision;
	int		zero_pad;
	char	specifier;
	char	padding;
	char	left_align;
}		t_flags;

typedef struct s_list
{
	struct s_list	*next;
	void			*content;
	size_t			i;
	size_t			count;
	va_list			ap;
}		t_list;

int		ft_isalpha(int i);
int		ft_isdigit(int i);
int		ft_isalnum(int i);
int		ft_isascii(int i);
int		ft_isprint(int i);
int		ft_isspace(int c);
size_t	ft_strlen(const char *str);
void	*ft_memset(void *str, int c, size_t n);
void	ft_bzero(void *str, size_t n);
void	*ft_memcpy(void *dest, const void *src, size_t n);
void	*ft_memmove(void *dest, const void *src, size_t n);
size_t	ft_strlcpy(char *dest, const char *src, size_t n);
char	*ft_strcpy(char *dst, char *src);
char	*ft_strcat(char *dest, const char *src);
size_t	ft_strlcat(char *dest, const char *src, size_t n);
char	*ft_strncpy(char *dest, char *src, unsigned int n);
int		ft_toupper(int c);
int		ft_tolower(int c);
char	*ft_strchr(const char *s, int c);
char	*ft_strrchr(const char *s, int c);
int		ft_strcmp(char *s1, char *s2);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
void	*ft_memchr(const void *str, int c, size_t n);
int		ft_memcmp(const void *s1, const void *s2, size_t n);
char	*ft_strnstr(const char *haystack, const char *needle, size_t len);
int		ft_atoi(const char *str);
void	*ft_calloc(size_t nmeb, size_t size);
char	*ft_strdup(const char *s);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strjoin_free(char const *s1, char const *s2, char flag);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strtrim(char const *s1, char const *set);
char	**ft_split(char const *s, char c);
char	*ft_strtok(char *str, char *sep);
long	check_overflow(long *result, int digit, int base, int sign);
long	ft_strtol(const char *nptr, char **endptr, int base);
size_t	ft_strcspn(char *s1, char *s2);
size_t	ft_strspn(char *s1, char *s2);
char	*ft_strndup(char *s, size_t n);
char	*ft_itoa(int n);
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void	ft_putchar_fd(char c, int fd);
void	ft_putstr_fd(char *s, int fd);
void	ft_putendl_fd(char *s, int fd);
void	ft_putnbr_fd(int n, int fd);
t_list	*ft_lstnew(void *content);
void	ft_lstadd_front(t_list **lst, t_list *nw);
int		ft_lstsize(t_list *lst);
t_list	*ft_lstlast(t_list *lst);
void	ft_lstadd_back(t_list **lst, t_list *nw);
void	ft_lstdelone(t_list *lst, void (*del)(void *));
void	ft_lstclear(t_list **lst, void (*del)(void *));
void	ft_lstiter(t_list *lst, void (*f)(void *));
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));

// Helper functions
void	print_padding(int length, char pad_char, t_list *op);
char	*ft_itoa_custombase(unsigned long value, int base);
void	parse_flags(t_flags *flags, const char *format, size_t *i);
void	ft_putchar(char c);
char	*ft_strstr(const char *haystack, const char *needle);

//Bonus functions
void	print_sign_bonus(t_flags *flags, int value, t_list *op);
void	print_integer_str_bonus(const char *str, t_list *op, t_flags *s_flags);
int		calculate_padding_bonus(int value, t_flags *flags, int len, int sign);
void	handle_padding_bonus(t_flags *flags, int padding, size_t value,
			t_list *op);
int		print_prefix_bonus(int prefix_len, t_flags *flags, t_list *op);

// Main functions
void	print_int(t_flags *flags, t_list *op);
void	print_char(t_flags *flags, t_list *op);
void	print_string(t_flags *flags, t_list *op);
void	print_ptr(t_flags *flags, t_list *op);
void	print_hex(t_flags *flags, t_list *op);
void	print_unsigned(t_flags *flags, t_list *op);
int		ft_printf(const char *format, ...);

char	*get_next_line(int fd);
void	*ft_realloc(void *ptr, size_t oldsize, size_t size);
char	*ft_strncat(char *dest, char *src, unsigned int nb);

#endif
