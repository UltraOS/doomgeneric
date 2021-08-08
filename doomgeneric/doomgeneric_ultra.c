#include "doomkeys.h"
#include "doomgeneric.h"

#include <Ultra/Ultra.h>

static uint32_t window_id = 0;
static uint32_t* framebuffer = NULL;
static uint32_t pitch = 0;

void DG_Init()
{
    debug_log("Initializing UltraOS stuff...");

    WindowCreateCommand wc = {
        .command = WM_CREATE_WINDOW,
        .width = DOOMGENERIC_RESX,
        .height = DOOMGENERIC_RESY,
        .title = "DOOM",
    };

    execute_window_command(&wc);

    if (!wc.framebuffer) {
        debug_log("Failed to create window!");
        exit(-1);
    }

    window_id = wc.window_id;
    framebuffer = (uint32_t*)wc.framebuffer;
    pitch = wc.pitch;
}

int DG_GetKey(int* pressed, unsigned char* doomKey)
{
    PopEventCommand pe = {
        pe.command = WM_POP_EVENT,
        pe.window_id = window_id
    };

    for (;;) {
        execute_window_command(&pe);

        switch (pe.event.type)
        {
        case ET_EMPTY:
            return 0;
        case ET_KEY_STATE:
            switch (pe.event.vk_state.vkey)
            {
            case VK_ENTER:
                *doomKey = KEY_ENTER;
                break;
            case VK_ESC:
                *doomKey = KEY_ESCAPE;
                break;
            case VK_ARROW_LEFT:
                *doomKey = KEY_LEFTARROW;
                break;
            case VK_ARROW_RIGHT:
                *doomKey = KEY_RIGHTARROW;
                break;
            case VK_ARROW_UP:
                *doomKey = KEY_UPARROW;
                break;
            case VK_ARROW_DOWN:
                *doomKey = KEY_DOWNARROW;
                break;
            case VK_LEFT_CTRL:
                *doomKey = KEY_FIRE;
                break;
            case VK_SPACE:
                *doomKey = KEY_USE;
                break;
            }
            *pressed = pe.event.vk_state.state == VKS_PRESSED;
            return 1;
        case ET_WINDOW_SHOULD_CLOSE: {
            WindowDestroyCommand wd = {
                .command = WM_DESTROY_WINDOW,
                .window_id = window_id
            };
            execute_window_command(&wd);

            exit(0);
        }
        default:
            continue;
        }
    }
}

void DG_DrawFrame()
{
    for (size_t y = 0; y < DOOMGENERIC_RESY; ++y) {
        size_t offset = y * pitch;
        uint32_t* fb = (uint32_t*)(((uint8_t*)framebuffer) + offset);

        size_t doom_offset_y = y * DOOMGENERIC_RESX;

        for (size_t x = 0; x < DOOMGENERIC_RESX; ++x)
            fb[x] = DG_ScreenBuffer[doom_offset_y + x];
    }

    InvalidateRectCommand ir = {
        .command = WM_INVALIDATE_RECT,
        .window_id = window_id
    };

    execute_window_command(&ir);
}

void DG_SetWindowTitle(const char* title)
{
    SetTitleCommand sc = {
        .command = WM_SET_TITLE,
        .window_id = window_id,
        .title = title
    };

    execute_window_command(&sc);
}

void DG_SleepMs(uint32_t ms)
{
    sleep(ms * 1000 * 1000);
}

uint32_t DG_GetTicksMs()
{
    return ticks_since_boot();
}
