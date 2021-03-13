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

unsigned long syscall(uint32_t nr)
{
    unsigned long ret;
    
    asm volatile("int $0x81" : "=a"(ret) : "a"((unsigned long)nr) : "rcx", "r11");
    
    return ret;
}

unsigned long syscall(uint32_t nr, unsigned long a1)
{
    unsigned long ret;
    
    asm volatile("int $0x81" : "=a"(ret) : "a"((unsigned long)nr), "D"(a1) : "rcx", "r11");
    
    return ret;
}

unsigned long syscall(uint32_t nr, unsigned long a1, unsigned long a2)
{
    unsigned long ret;
    
    asm volatile("int $0x81" : "=a"(ret) : "a"((unsigned long)nr), "D"(a1), "S"(a2) : "rcx", "r11");
    
    return ret;
}

unsigned long syscall(uint32_t nr, unsigned long a1, unsigned long a2, unsigned long a3)
{
    unsigned long ret;
    
    asm volatile("int $0x81" : "=a"(ret) : "a"((unsigned long)nr), "D"(a1), "S"(a2), "d"(a3) : "rcx", "r11");
    
    return ret;
}

unsigned long syscall(uint32_t nr, unsigned long a1, unsigned long a2, unsigned long a3, unsigned long a4, unsigned long a5)
{
    unsigned long ret;

    register unsigned long r8 asm("r8") = a5;
    
    asm volatile("int $0x81" : "=a"(ret) : "a"((unsigned long)nr), "D"(a1), "S"(a2), "d"(a3), "c"(a4), "r"(r8) : "r11");
    
    return ret;
}
