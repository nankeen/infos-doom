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

HPROC exec(const char *program, const char *args)
{
    return (HPROC)syscall(SYS_EXEC, (unsigned long)program, (unsigned long)args);
}

void wait_proc(HPROC proc)
{
    syscall(SYS_WAIT_PROC, proc);
}

HTHREAD create_thread(ThreadProc tp, void *arg)
{
    return (HTHREAD)syscall(SYS_CREATE_THREAD, (unsigned long)tp, (unsigned long)arg);
}

void stop_thread(HTHREAD thread)
{
    syscall(SYS_STOP_THREAD, thread);
}

void join_thread(HTHREAD thread)
{
    syscall(SYS_JOIN_THREAD, thread);
}

void usleep(unsigned long us)
{
    syscall(SYS_USLEEP, us);
}

void system(const char *cmd)
{
    char prog[64];
    int n = 0;
    while (*cmd && *cmd != ' ' && n < 63) {
        prog[n++] = *cmd++;
    }
    
    prog[n] = 0;
    
    if (*cmd) cmd++;
    
    HPROC pcmd = exec(prog, cmd);
    if (is_error(pcmd)) {
        printf("error: unable to run command %s '%s'\n", prog, cmd);
    } else {
        wait_proc(pcmd);
    }
}
