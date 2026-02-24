#include "cub3d.h"

int	error_exit(const char *msg)
{
	if (msg)
	{
		write(2, "Error\n", 6);
		write(2, msg, __builtin_strlen(msg));
		write(2, "\n", 1);
	}
	return (1);
}
