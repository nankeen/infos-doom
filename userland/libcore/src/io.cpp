/* SPDX-License-Identifier: MIT */

#include <libcore.h>

HFILE open(const char *file, int flags)
{
	return (HFILE)syscall(SYS_OPEN, (unsigned long)file, (unsigned long)flags);
}

void close(HFILE file)
{
	syscall(SYS_CLOSE, (unsigned long)file);
}

int read(HFILE file, char *buffer, size_t size)
{
	return (int)syscall(SYS_READ, (unsigned long)file, (unsigned long)buffer, (unsigned long)size);
}

int write(HFILE file, const char *buffer, size_t size)
{
	return (int)syscall(SYS_WRITE, (unsigned long)file, (unsigned long)buffer, (unsigned long)size);
}

HDIR opendir(const char *path, int flags)
{
	return (HDIR)syscall(SYS_OPENDIR, (unsigned long)path, (unsigned long)flags);
}

int readdir(HDIR dir, struct dirent *de)
{
	return (int)syscall(SYS_READDIR, (unsigned long)dir, (unsigned long)de);
}

void closedir(HDIR dir)
{
	syscall(SYS_CLOSEDIR, (unsigned long)dir);
}

int get_time_of_day(struct tod *t)
{
	return (int)syscall(SYS_GET_TOD, (unsigned long)t);
}
