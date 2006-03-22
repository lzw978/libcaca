/*
 *  libcaca       Colour ASCII-Art library
 *  Copyright (c) 2002-2006 Sam Hocevar <sam@zoy.org>
 *                All Rights Reserved
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the Do What The Fuck You Want To
 *  Public License, Version 2, as published by Sam Hocevar. See
 *  http://sam.zoy.org/wtfpl/COPYING for more details.
 */

/** \file driver_win32.c
 *  \version \$Id$
 *  \author Sam Hocevar <sam@zoy.org>
 *  \brief Win32 driver
 *
 *  This file contains the libcaca Win32 input and output driver
 */

#include "config.h"

#if defined(USE_WIN32)

#include <windows.h>

#include <stdlib.h>
#include <stdio.h>

#include "caca.h"
#include "caca_internals.h"
#include "cucul.h"
#include "cucul_internals.h"

/*
 * Global variables
 */

static int const win32_fg_palette[] =
{
    0,
    FOREGROUND_BLUE,
    FOREGROUND_GREEN,
    FOREGROUND_GREEN | FOREGROUND_BLUE,
    FOREGROUND_RED,
    FOREGROUND_RED | FOREGROUND_BLUE,
    FOREGROUND_RED | FOREGROUND_GREEN,
    FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
    FOREGROUND_INTENSITY,
    FOREGROUND_INTENSITY | FOREGROUND_BLUE,
    FOREGROUND_INTENSITY | FOREGROUND_GREEN,
    FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
    FOREGROUND_INTENSITY | FOREGROUND_RED,
    FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
    FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
    FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
};

static int const win32_bg_palette[] =
{
    0,
    BACKGROUND_BLUE,
    BACKGROUND_GREEN,
    BACKGROUND_GREEN | BACKGROUND_BLUE,
    BACKGROUND_RED,
    BACKGROUND_RED | BACKGROUND_BLUE,
    BACKGROUND_RED | BACKGROUND_GREEN,
    BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
    BACKGROUND_INTENSITY,
    BACKGROUND_INTENSITY | BACKGROUND_BLUE,
    BACKGROUND_INTENSITY | BACKGROUND_GREEN,
    BACKGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_BLUE,
    BACKGROUND_INTENSITY | BACKGROUND_RED,
    BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE,
    BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN,
    BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE
};

struct driver_private
{
    HANDLE hin, hout, screen;
    CHAR_INFO *buffer;
    CONSOLE_CURSOR_INFO cci;
};

static int win32_init_graphics(caca_t *kk)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    SMALL_RECT rect;
    COORD size;

    kk->drv.p = malloc(sizeof(struct driver_private));

    /* This call is allowed to fail in case we already have a console */
    AllocConsole();

    kk->drv.p->hin = GetStdHandle(STD_INPUT_HANDLE);
    kk->drv.p->hout = CreateFile("CONOUT$", GENERIC_READ | GENERIC_WRITE,
                                 FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
                                 OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if(kk->drv.p->hout == INVALID_HANDLE_VALUE)
        return -1;

    GetConsoleCursorInfo(kk->drv.p->hout, &kk->drv.p->cci);
    kk->drv.p->cci.bVisible = FALSE;
    SetConsoleCursorInfo(kk->drv.p->hout, &kk->drv.p->cci);

    SetConsoleMode(kk->drv.p->hout, ENABLE_MOUSE_INPUT);

    kk->drv.p->screen =
        CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,
                                  0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    if(!kk->drv.p->screen || kk->drv.p->screen == INVALID_HANDLE_VALUE)
        return -1;

    /* Set the new console size */
    size.X = kk->qq->width;
    size.Y = kk->qq->height;
    SetConsoleScreenBufferSize(kk->drv.p->screen, size);

    rect.Left = rect.Top = 0;
    rect.Right = kk->qq->width - 1;
    rect.Bottom = kk->qq->height - 1;
    SetConsoleWindowInfo(kk->drv.p->screen, TRUE, &rect);

    /* Report our new size to libcucul */
    if(!GetConsoleScreenBufferInfo(kk->drv.p->screen, &csbi))
        return -1;

    _cucul_set_size(kk->qq, csbi.srWindow.Right - csbi.srWindow.Left + 1,
                            csbi.srWindow.Bottom - csbi.srWindow.Top + 1);

    SetConsoleMode(kk->drv.p->screen, 0);

    GetConsoleCursorInfo(kk->drv.p->screen, &kk->drv.p->cci);
    kk->drv.p->cci.dwSize = 0;
    kk->drv.p->cci.bVisible = FALSE;
    SetConsoleCursorInfo(kk->drv.p->screen, &kk->drv.p->cci);

    SetConsoleActiveScreenBuffer(kk->drv.p->screen);

    kk->drv.p->buffer = malloc(kk->qq->width * kk->qq->height
                               * sizeof(CHAR_INFO));
    if(kk->drv.p->buffer == NULL)
        return -1;

    return 0;
}

static int win32_end_graphics(caca_t *kk)
{
    SetConsoleActiveScreenBuffer(kk->drv.p->hout);
    CloseHandle(kk->drv.p->screen);

    SetConsoleTextAttribute(kk->drv.p->hout, FOREGROUND_INTENSITY
                                             | FOREGROUND_RED
                                             | FOREGROUND_GREEN
                                             | FOREGROUND_BLUE);
    kk->drv.p->cci.bVisible = TRUE;
    SetConsoleCursorInfo(kk->drv.p->hout, &kk->drv.p->cci);
    CloseHandle(kk->drv.p->hout);

    free(kk->drv.p);

    return 0;
}

static int win32_set_window_title(caca_t *kk, char const *title)
{
    SetConsoleTitle(title);
    return 0;
}

static unsigned int win32_get_window_width(caca_t *kk)
{
    /* FIXME */

    /* Fallback to a 6x10 font */
    return kk->qq->width * 6;
}

static unsigned int win32_get_window_height(caca_t *kk)
{
    /* FIXME */

    /* Fallback to a 6x10 font */
    return kk->qq->height * 10;
}

static void win32_display(caca_t *kk)
{
    COORD size, pos;
    SMALL_RECT rect;
    unsigned int i;

    /* Render everything to our screen buffer */
    for(i = 0; i < kk->qq->width * kk->qq->height; i++)
    {
        uint32_t c = kk->qq->chars[i];

#if 0
        if(c > 0x00000020 && c < 0x00000080)
            kk->drv.p->buffer[i].Char.AsciiChar = (uint8_t)c;
        else
            kk->drv.p->buffer[i].Char.AsciiChar = ' ';
#else
        if(c > 0x00000020 && c < 0x00010000)
            kk->drv.p->buffer[i].Char.UnicodeChar = (uint16_t)c;
        else
            kk->drv.p->buffer[i].Char.UnicodeChar = (uint16_t)' ';
#endif

        kk->drv.p->buffer[i].Attributes =
                win32_fg_palette[kk->qq->attr[i] & 0xf]
                 | win32_bg_palette[kk->qq->attr[i] >> 4];
    }

    /* Blit the screen buffer */
    size.X = kk->qq->width;
    size.Y = kk->qq->height;
    pos.X = pos.Y = 0;
    rect.Left = rect.Top = 0;
    rect.Right = kk->qq->width - 1;
    rect.Bottom = kk->qq->height - 1;
#if 0
    WriteConsoleOutput(kk->drv.p->screen, kk->drv.p->buffer, size, pos, &rect);
#else
    WriteConsoleOutputW(kk->drv.p->screen, kk->drv.p->buffer, size, pos, &rect);
#endif
}

static void win32_handle_resize(caca_t *kk)
{
    /* FIXME: I don't know what to do here. */
    kk->resize.w = kk->qq->width;
    kk->resize.h = kk->qq->height;
}

static unsigned int win32_get_event(caca_t *kk)
{
    INPUT_RECORD rec;
    DWORD num;

    for( ; ; )
    {
        GetNumberOfConsoleInputEvents(kk->drv.p->hin, &num);
        if(num == 0)
            break;

        ReadConsoleInput(kk->drv.p->hin, &rec, 1, &num);
        if(rec.EventType == KEY_EVENT)
        {
            unsigned int event;

            if(rec.Event.KeyEvent.bKeyDown)
                event = CACA_EVENT_KEY_PRESS;
            else
                event = CACA_EVENT_KEY_RELEASE;

            if(rec.Event.KeyEvent.uChar.AsciiChar)
                return event | rec.Event.KeyEvent.uChar.AsciiChar;
        }

        if(rec.EventType == MOUSE_EVENT)
        {
            if(rec.Event.MouseEvent.dwEventFlags == 0)
            {
                if(rec.Event.MouseEvent.dwButtonState & 0x01)
                    return CACA_EVENT_MOUSE_PRESS | 0x000001;

                if(rec.Event.MouseEvent.dwButtonState & 0x02)
                    return CACA_EVENT_MOUSE_PRESS | 0x000002;
            }
            else if(rec.Event.MouseEvent.dwEventFlags == MOUSE_MOVED)
            {
                COORD pos = rec.Event.MouseEvent.dwMousePosition;

                if(kk->mouse.x == (unsigned int)pos.X &&
                   kk->mouse.y == (unsigned int)pos.Y)
                    continue;

                kk->mouse.x = pos.X;
                kk->mouse.y = pos.Y;

                return CACA_EVENT_MOUSE_MOTION
                        | (kk->mouse.x << 12) | kk->mouse.y;
            }
#if 0
            else if(rec.Event.MouseEvent.dwEventFlags == DOUBLE_CLICK)
            {
                cout << rec.Event.MouseEvent.dwMousePosition.X << "," <<
                        rec.Event.MouseEvent.dwMousePosition.Y << "  " << flush;
            }
            else if(rec.Event.MouseEvent.dwEventFlags == MOUSE_WHEELED)
            {
                SetConsoleCursorPosition(hOut,
                                         WheelWhere);
                if(rec.Event.MouseEvent.dwButtonState & 0xFF000000)
                    cout << "Down" << flush;
                else
                    cout << "Up  " << flush;
            }
#endif
        }

        /* Unknown event */
        return CACA_EVENT_NONE;
    }

    /* No event */
    return CACA_EVENT_NONE;
}

/*
 * Driver initialisation
 */

void win32_init_driver(caca_t *kk)
{
    kk->drv.driver = CACA_DRIVER_WIN32;

    kk->drv.init_graphics = win32_init_graphics;
    kk->drv.end_graphics = win32_end_graphics;
    kk->drv.set_window_title = win32_set_window_title;
    kk->drv.get_window_width = win32_get_window_width;
    kk->drv.get_window_height = win32_get_window_height;
    kk->drv.display = win32_display;
    kk->drv.handle_resize = win32_handle_resize;
    kk->drv.get_event = win32_get_event;
}

#endif /* USE_WIN32 */

