#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

typedef unsigned long HANDLE;
typedef HANDLE HFILE;
typedef HANDLE HDIR;
typedef HANDLE HPROC;
typedef HANDLE HTHREAD;

int main(int, const char *)
{
    puts("Hello world\n");
    return 0;
}
