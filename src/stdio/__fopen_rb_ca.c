#include "stdio_impl.h"
#include <fcntl.h>
#include <string.h>

extern __typeof(open) __libc_open;
extern __typeof(fcntl) __libc_fcntl;

FILE *__fopen_rb_ca(const char *filename, FILE *f, unsigned char *buf, size_t len)
{
	memset(f, 0, sizeof *f);

	f->fd = __libc_open(filename, O_RDONLY|O_CLOEXEC);
	if (f->fd < 0) return 0;
	__libc_fcntl(f->fd, F_SETFD, FD_CLOEXEC);

	f->flags = F_NOWR | F_PERM;
	f->buf = buf + UNGET;
	f->buf_size = len - UNGET;
	f->read = __stdio_read;
	f->seek = __stdio_seek;
	f->close = __stdio_close;
	f->lock = -1;

	return f;
}
