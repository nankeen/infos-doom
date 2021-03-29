#include "doomgeneric.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

uint32_t *DG_ScreenBuffer = 0;
static uint64_t start_tick = 0;
static uint32_t *frame_buffer = 0;

typedef unsigned long HANDLE;
typedef HANDLE HFILE;
#ifndef SYS_MMAP
#define SYS_MMAP 0x15
#endif

#define V_WIDTH     1280
#define V_HEIGHT    768
#define V_STRIDE    V_WIDTH * 4
#define GRAPHICS_START_X ((V_WIDTH - DOOMGENERIC_RESX) / 2)
#define GRAPHICS_START_Y ((V_HEIGHT - DOOMGENERIC_RESY) / 2)

static void *mmap(void *addr, size_t len, int flags, HFILE fd, size_t offset)
{
    void *ret;
    
    asm volatile("int $0x81" : "=a"(ret) : "a"(SYS_MMAP), "D"(addr), "S"(len), "d"(flags), "c"(fd), "r"(offset));
    
    return ret;
}

void dg_Create()
{
    DG_ScreenBuffer = (uint32_t *)malloc(DOOMGENERIC_RESX * DOOMGENERIC_RESY * 4);

    DG_Init();
}

void DG_Init()
{
    // Open video device
    HFILE fd = open("/dev/video0", 0);
    if (fd < 0) {
        printf("Could not open /dev/video0\n");
        exit(1);
    }
    frame_buffer = (uint32_t *) mmap((void *)0xdeadbeef, V_STRIDE * V_HEIGHT, 1, fd, 0);
    start_tick = clock();
}

void DG_DrawFrame()
{
    for (size_t y = 0; y < DOOMGENERIC_RESY; y++) {
        for (size_t x = 0; x < DOOMGENERIC_RESX; x++) {
            frame_buffer[(y + GRAPHICS_START_Y) * V_WIDTH + (x + GRAPHICS_START_X)] = DG_ScreenBuffer[y * DOOMGENERIC_RESX + x];
        }
    }
}

void DG_SleepMs(uint32_t ms)
{
    usleep(ms * 1000);
}

uint32_t DG_GetTicksMs()
{
    return (clock() - start_tick) / 1e6;
}

int DG_GetKey(int* pressed, unsigned char* doom_key)
{
    return 0;
}

void DG_SetWindowTitle(const char *title)
{
    printf("Title: %s\n", title);
}
