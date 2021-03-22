#include <libcore.h>

#define V_WIDTH     1280
#define V_HEIGHT    768
#define V_STRIDE    V_WIDTH * 4

void *init_framebuffer()
{
    // Open video device
    HFILE fd = open("/dev/video0", 0);
    if (is_error(fd)) {
        printf("Could not open /dev/video0\n");
        exit(1);
    }

    return mmap((void *)0xdeadbeef, V_STRIDE * V_HEIGHT, 1, fd, 0);
}

int main(const char *)
{
    // Test userland frame graphics
    uint32_t *frame_buffer = (uint32_t *)init_framebuffer();
    if ((intptr_t)frame_buffer == -1) {
        printf("MMAP error: %p\n", frame_buffer);
        exit(1);
    }
    printf("Mapped frame buffer at %p\n", frame_buffer);
    while (1) {
        for (size_t i = 0; i < V_WIDTH * V_HEIGHT; i++) {
            frame_buffer[i] = -1;
        }
    }
    return 0;
}
