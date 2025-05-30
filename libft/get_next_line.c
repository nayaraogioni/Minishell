/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 21:08:43 by dopereir          #+#    #+#             */
/*   Updated: 2025/01/08 21:28:36 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	find_newline_eof(char *buffer)
{
	int	i;

	i = 0;
	if (!buffer)
		return (-1);
	while (buffer[i])
	{
		if (buffer[i] == '\n')
			return (i);
		i++;
	}
	return (-1);
}

static int	update_remainder_buffer(char **buffer, int newline_pos)
{
	int	remaining_len;

	remaining_len = ft_strlen(*buffer + newline_pos);
	if (remaining_len > 0)
		ft_memmove(*buffer, *buffer + newline_pos, remaining_len + 1);
	else
		(*buffer)[0] = '\0';
	*buffer = ft_realloc(*buffer, ft_strlen(*buffer), remaining_len + 1);
	if (!*buffer)
		return (0);
	return (1);
}

char	*extract_line(char **buffer, int newline_pos)
{
	char	*line;
	int		line_len;

	if ((*buffer)[0] == '\n' && newline_pos >= 0)
	{
		line = malloc(2);
		line[0] = '\n';
		line[1] = '\0';
		if (!update_remainder_buffer(buffer, 1))
			return (free(line), NULL);
		return (line);
	}
	if (newline_pos == -1)
		newline_pos = ft_strlen(*buffer);
	line_len = newline_pos;
	if ((*buffer)[newline_pos] == '\n')
		line_len = newline_pos + 1;
	line = malloc(line_len + 1);
	if (!line)
		return (NULL);
	ft_memcpy(line, *buffer, line_len);
	line[line_len] = '\0';
	if (!update_remainder_buffer(buffer, line_len))
		return (free(line), NULL);
	return (line);
}

int	read_newpiece(int fd, char **buffer)
{
	int			bytes_readed;
	char		current_buffer[BUFFER_SIZE + 1];
	char		*new_buffer;

	bytes_readed = read(fd, current_buffer, BUFFER_SIZE);
	if (bytes_readed <= 0)
		return (bytes_readed);
	current_buffer[bytes_readed] = '\0';
	if (!*buffer)
	{
		*buffer = malloc(1);
		if (!*buffer)
			return (-1);
		(*buffer)[0] = '\0';
	}
	new_buffer = ft_realloc(*buffer, ft_strlen(*buffer),
			ft_strlen(*buffer) + bytes_readed + 1);
	if (!new_buffer)
		return (-1);
	*buffer = new_buffer;
	ft_strncat(*buffer, current_buffer, bytes_readed);
	return (bytes_readed);
}

char	*get_next_line(int fd)
{
	static char	*buffer = NULL;
	char		*line;
	int			newline_index;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!buffer)
	{
		buffer = malloc(1);
		if (!buffer)
			return (NULL);
		buffer[0] = '\0';
	}
	newline_index = find_newline_eof(buffer);
	while (newline_index == -1 && read_newpiece(fd, &buffer) > 0)
		newline_index = find_newline_eof(buffer);
	if (newline_index == -1 && (!buffer || buffer[0] == '\0'))
	{
		free (buffer);
		buffer = NULL;
		return (NULL);
	}
	line = extract_line(&buffer, newline_index);
	return (line);
}

/*int main(int ac, char **av)
{
    int     fd;
    char    *line;

    if (ac < 2)
        return (write(1, "Usage: ./a.out <filename>\n", 27));
    fd = open(av[1], O_RDONLY);
    if (fd <= -1)
        return (write(1, "error opening file\n", 20));

    while ((line = get_next_line(fd)) != NULL)
    {
        printf("()%s", line);
        free (line);
    }
    close (fd);
    return (0);
}*/
