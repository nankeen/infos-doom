/* SPDX-License-Identifier: MIT */
/**
 * MIT License
 * 
 * Copyright (c) 2018 Tom Spink
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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
