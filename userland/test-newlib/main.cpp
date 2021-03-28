#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int, const char *)
{
	char *allocated = (char *)malloc(0x20000);
	printf("Allocated buffer at: %p\n", allocated);
	fflush(stdout);
	*(allocated+0x20000) = 0;
    return 0;
}
