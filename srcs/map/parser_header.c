#include "parser_internal.h"

static int	starts_with_token(const char *line, const char *token)
{
	int	i;

	i = 0;
	while (token[i])
	{
		if (line[i] != token[i])
			return (0);
		i++;
	}
	if (line[i] && !p_is_space(line[i]))
		return (0);
	return (1);
}

static char	*extract_value_after_key(const char *line, int key_len)
{
	int	start;
	int	end;

	start = key_len;
	while (line[start] && p_is_space(line[start]))
		start++;
	end = (int)p_strlen(line);
	while (end > start && p_is_space(line[end - 1]))
		end--;
	if (end <= start)
		return (NULL);
	return (p_dup_range(line, start, end));
}

static int	parse_rgb_value(const char *line, int rgb[3])
{
	int	i;
	int	j;
	int	value;

	i = 0;
	j = 0;
	while (j < 3)
	{
		while (line[i] && p_is_space(line[i]))
			i++;
		if (!p_is_digit(line[i]))
			return (1);
		value = 0;
		while (p_is_digit(line[i]))
		{
			value = value * 10 + (line[i++] - '0');
			if (value > 255)
				return (1);
		}
		if (value < 0 || value > 255)
			return (1);
		rgb[j++] = value;
		while (line[i] && p_is_space(line[i]))
			i++;
		if (j < 3)
		{
			if (line[i] != ',')
				return (1);
			i++;
		}
	}
	while (line[i])
	{
		if (!p_is_space(line[i]))
			return (1);
		i++;
	}
	return (0);
}

static int	set_header_value(char **dst, const char *line, int key_len)
{
	char	*value;

	if (*dst)
		return (1);
	value = extract_value_after_key(line, key_len);
	if (!value)
		return (1);
	*dst = value;
	return (0);
}

int	p_parse_header_line(t_game *game, const char *line)
{
	if (starts_with_token(line, "NO"))
		return (set_header_value(&game->map.tex_no, line, 2));
	if (starts_with_token(line, "SO"))
		return (set_header_value(&game->map.tex_so, line, 2));
	if (starts_with_token(line, "WE"))
		return (set_header_value(&game->map.tex_we, line, 2));
	if (starts_with_token(line, "EA"))
		return (set_header_value(&game->map.tex_ea, line, 2));
	if (starts_with_token(line, "F"))
	{
		if (game->map.floor_rgb[0] != -1)
			return (1);
		return (parse_rgb_value(line + 1, game->map.floor_rgb));
	}
	if (starts_with_token(line, "C"))
	{
		if (game->map.ceiling_rgb[0] != -1)
			return (1);
		return (parse_rgb_value(line + 1, game->map.ceiling_rgb));
	}
	return (1);
}
