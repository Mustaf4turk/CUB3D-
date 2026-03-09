/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muturk <muturk@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 19:08:22 by muturk            #+#    #+#             */
/*   Updated: 2026/03/09 19:08:23 by muturk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_internal.h"

size_t	p_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (s[len])
		len++;
	return (len);
}

int	p_is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\r' || c == '\v' || c == '\f');
}

int	p_is_digit(char c)
{
	return (c >= '0' && c <= '9');
}

char	*p_dup_range(const char *s, int start, int end)
{
	char	*dup;
	int		i;

	if (end < start)
		end = start;
	dup = malloc((size_t)(end - start + 1));
	if (!dup)
		return (NULL);
	i = 0;
	while (start + i < end)
	{
		dup[i] = s[start + i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

void	p_free_lines(char **lines)
{
	int	i;

	if (!lines)
		return ;
	i = 0;
	while (lines[i])
	{
		free(lines[i]);
		i++;
	}
	free(lines);
}
