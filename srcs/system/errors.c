#include "cub3d.h"

static size_t	msg_len(const char *s)
{
	size_t	len;

	len = 0;
	while (s[len])
		len++;
	return (len);
}

static void	safe_write(int fd, const char *buf, size_t len)
{
	ssize_t	ret;

	ret = write(fd, buf, len);
	if (ret < 0)
		return ;
}

int	error_exit(const char *msg)
{
	if (msg)
	{
		safe_write(2, "Error\n", 6);
		safe_write(2, msg, msg_len(msg));
		safe_write(2, "\n", 1);
	}
	return (1);
}
