#include "doomgeneric.h"

int should_exit = 0;

void dg_Create()
{
    DG_ScreenBuffer = (uint32_t *)malloc(DOOMGENERIC_RESX * DOOMGENERIC_RESY * 4);

    DG_Init();
}
