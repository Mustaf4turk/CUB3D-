#include "parser_internal.h"

static int	count_lines(const char *all)
{
	int	count;
	int	i;

	count = 1;
	i = -1;
	while (all[++i])
		if (all[i] == '\n')
			count++;
	return (count);
}

static int	fill_lines(char **lines, const char *all)
{
	int	i;
	int	start;
	int	idx;

	idx = 0;
	start = 0;
	i = 0;
	while (1)
	{
		if (all[i] == '\n' || all[i] == '\0')
		{
			lines[idx] = p_dup_range(all, start, i);
			if (!lines[idx])
				return (-1);
			idx++;
			start = i + 1;
			if (all[i] == '\0')
				break ;
		}
		i++;
	}
	return (idx);
}

char	**p_split_lines(const char *all, int *line_count)
{
	char	**lines;
	int		count;
	int		idx;

	count = count_lines(all);
	lines = malloc(sizeof(char *) * (count + 1));
	if (!lines)
		return (NULL);
	idx = fill_lines(lines, all);
	if (idx < 0)
		return (p_free_lines(lines), NULL);
	lines[idx] = NULL;
	*line_count = idx;
	return (lines);
}

int	p_is_blank_line(const char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (!p_is_space(line[i]))
			return (0);
		i++;
	}
	return (1);
}
