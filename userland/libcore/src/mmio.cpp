#include <libcore.h>


void *mmap(void *addr, size_t len, int flags, HFILE fd, off_t offset)
{
    return (void *)syscall(SYS_MMAP, (unsigned long)addr, len, (unsigned long)flags, fd, offset);
}
