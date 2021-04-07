#include "doomkeys.h"
#include "doomgeneric.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>

uint32_t *DG_ScreenBuffer = 0;
static uint64_t start_tick = 0;
static uint32_t *frame_buffer = 0;

typedef unsigned long HANDLE;
typedef HANDLE HFILE;
typedef HANDLE HTHREAD;
typedef HANDLE HPROC;

#define SYS_MMAP            0x15
#define SYS_WAIT_PROC       0x0b
#define SYS_CREATE_THREAD   0x0c
#define SYS_STOP_THREAD     0x0d
#define SYS_JOIN_THREAD     0x0e

#define V_WIDTH     1280
#define V_HEIGHT    768
#define V_STRIDE    V_WIDTH * 4
#define GRAPHICS_START_X ((V_WIDTH - DOOMGENERIC_RESX) / 2)
#define GRAPHICS_START_Y ((V_HEIGHT - DOOMGENERIC_RESY) / 2)

#define KEYQUEUE_SIZE 32

static unsigned short s_KeyQueue[KEYQUEUE_SIZE];
static unsigned int s_KeyQueueWriteIndex = 0;
static unsigned int s_KeyQueueReadIndex = 0;

static uint8_t scancode_map[] = {
    0, KEY_ESCAPE,
    '1', '2', '3', '4', '5', '6',
    '7', '8', '9', '0',

    KEY_MINUS, KEY_EQUALS, KEY_BACKSPACE, KEY_TAB,

    'Q', 'W', 'E', 'R', 'T', 'Y',
    'U', 'I', 'O', 'P', '[', ']',

    KEY_ENTER,
    KEY_FIRE,

    'A', 'S', 'D', 'F', 'G', 'H',
    'J', 'K', 'L', ';', '\'', '`',

    KEY_RSHIFT,
    '\\', 'Z', 'X', 'C', 'V', 'B',
    'N', 'M', KEY_STRAFE_L, KEY_STRAFE_R, '/',

    KEY_RSHIFT,
    '*',
    KEY_LALT,
    KEY_USE,
    KEY_CAPSLOCK,

    KEY_F1,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
};

static char getch(HFILE fd)
{
    char buffer;
    
    int r = 0;
    while (r < 1) {
        r = read(fd, &buffer, 1);
    }
    
    return buffer;
}

static unsigned char convertToDoomKey(unsigned int key)
{
    key = tolower(key);
    switch (key)
    {
        case 'a':
            key = KEY_LEFTARROW;
            break;
        case 'd':
            key = KEY_RIGHTARROW;
            break;
        case 'w':
            key = KEY_UPARROW;
            break;
        case 's':
            key = KEY_DOWNARROW;
            break;
    }
    return key;
}

static uint8_t scanCodeToKey(uint8_t scancode)
{
    uint8_t key_scancode = ((uint8_t)scancode) & ~0x80;

    if (key_scancode > sizeof(scancode_map) / sizeof(*scancode_map)) {
        return 0x00;
    }

    return scancode_map[key_scancode];
}

static void addKeyToQueue(int pressed, unsigned int keyCode)
{
    unsigned char key = convertToDoomKey(keyCode);

    unsigned short keyData = (pressed << 8) | key;

    s_KeyQueue[s_KeyQueueWriteIndex] = keyData;
    s_KeyQueueWriteIndex++;
    s_KeyQueueWriteIndex %= KEYQUEUE_SIZE;
}

static void *mmap(void *addr, size_t len, int flags, HFILE fd, size_t offset)
{
    void *ret;
    
    asm volatile("int $0x81" : "=a"(ret) : "a"(SYS_MMAP), "D"(addr), "S"(len), "d"(flags), "c"(fd), "r"(offset));
    
    return ret;
}

static HTHREAD create_thread(void *tp, void *arg)
{
    HTHREAD ret;
    
    asm volatile("int $0x81" : "=a"(ret) : "a"(SYS_CREATE_THREAD), "D"(tp), "S"(arg));
    
    return ret;
}

static void stop_thread(HTHREAD thread)
{
    asm volatile("int $0x81" : : "a"(SYS_STOP_THREAD), "D"(thread));
}

static void handleKeyInput()
{
    char c;
    HFILE kbd_fd = open("/dev/kbd0", 0);
    if (kbd_fd == (HFILE)-1) {
        printf("Can't open keyboard device!\n");
        exit(1);
    }
    while (!should_exit) {
        c = getch(kbd_fd);
        if (c & 0x80) {
            addKeyToQueue(0, scanCodeToKey(c));
        } else {
            addKeyToQueue(1, scanCodeToKey(c));
        }
    }
    stop_thread(-1);
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

    create_thread((void *)handleKeyInput, NULL);
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
    if (s_KeyQueueReadIndex == s_KeyQueueWriteIndex){
        //key queue is empty
        return 0;
    } else {
        unsigned short keyData = s_KeyQueue[s_KeyQueueReadIndex];
        s_KeyQueueReadIndex++;
        s_KeyQueueReadIndex %= KEYQUEUE_SIZE;

        *pressed = keyData >> 8;
        *doom_key = keyData & 0xFF;

        return 1;
    }

    return 0;
}

void DG_SetWindowTitle(const char *title)
{
    printf("Title: %s\n", title);
}
