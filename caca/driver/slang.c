/*
 *  libcaca       Colour ASCII-Art library
 *  Copyright (c) 2002-2009 Sam Hocevar <sam@hocevar.net>
 *                All Rights Reserved
 *
 *  $Id$
 *
 *  This library is free software. It comes without any warranty, to
 *  the extent permitted by applicable law. You can redistribute it
 *  and/or modify it under the terms of the Do What The Fuck You Want
 *  To Public License, Version 2, as published by Sam Hocevar. See
 *  http://sam.zoy.org/wtfpl/COPYING for more details.
 */

/*
 *  This file contains the libcaca SLang input and output driver
 */

#include "config.h"

#if defined(USE_SLANG)

#if defined(HAVE_SLANG_SLANG_H)
#   include <slang/slang.h>
#else
#   include <slang.h>
#endif

#include <stdlib.h>
#include <string.h>

#if defined(HAVE_SIGNAL_H)
#   include <signal.h>
#endif

#include "caca.h"
#include "caca_internals.h"

/*
 * Global variables
 */

/* Tables generated by tools/optipal.c */
static int const slang_palette[2*16*16] =
{
     1,  0,   2,  0,   3,  0,   4,  0,   5,  0,   6,  0,   7,  0,   8,  0,
     9,  0,  10,  0,  11,  0,  12,  0,  13,  0,  14,  0,  15,  0,   0,  8,
     8,  7,   7,  8,  15,  7,   7, 15,  15,  9,   9, 15,   1,  9,   9,  1,
     7,  9,   9,  7,   8,  1,   1,  8,   0,  1,  15, 10,  10, 15,   2, 10,
    10,  2,   7, 10,  10,  7,   8,  2,   2,  8,   0,  2,  15, 11,  11, 15,
     3, 11,  11,  3,   7, 11,  11,  7,   8,  3,   3,  8,   0,  3,  15, 12,
    12, 15,   4, 12,  12,  4,   7, 12,  12,  7,   8,  4,   4,  8,   0,  4,
    15, 13,  13, 15,   5, 13,  13,  5,   7, 13,  13,  7,   8,  5,   5,  8,
     0,  5,  15, 14,  14, 15,   6, 14,  14,  6,   7, 14,  14,  7,   8,  6,
     6,  8,   0,  6,   4,  6,   6,  4,  12, 14,  14, 12,   6,  2,   2,  6,
    14, 10,  10, 14,   2,  3,   3,  2,  10, 11,  11, 10,   3,  1,   1,  3,
    11,  9,   9, 11,   1,  5,   5,  1,   9, 13,  13,  9,   5,  4,   4,  5,
    13, 12,  12, 13,   4, 14,   6, 12,  12,  6,  14,  4,   6, 10,   2, 14,
    14,  2,  10,  6,   2, 11,   3, 10,  10,  3,  11,  2,   3,  9,   1, 11,
    11,  1,   9,  3,   1, 13,   5,  9,   9,  5,  13,  1,   5, 12,   4, 13,
    13,  4,  12,  5,   0,  7,   0, 15,  15,  8,   8, 15,  15,  1,   7,  1,
     1,  6,   2,  5,   3,  4,   4,  3,   5,  2,   6,  1,   0,  0,   1,  1,
     9,  6,  10,  5,  11,  4,  12,  3,  13,  2,  14,  1,   2,  2,   3,  3,
     4,  4,   5,  5,   6,  6,   7,  7,  14,  9,   1, 15,   8,  9,   8,  8,
     9,  9,   1,  7,   0,  9,   9,  8,   6,  9,  13, 10,   2, 15,   8, 10,
     7,  2,  15,  2,   2,  7,   0, 10,  10,  8,   5, 10,  12, 11,   3, 15,
     8, 11,   7,  3,  15,  3,   3,  7,   0, 11,  11,  8,   4, 11,  11, 12,
     4, 15,   8, 12,   7,  4,  15,  4,   4,  7,   0, 12,  12,  8,   3, 12,
    10, 13,   5, 15,   8, 13,   7,  5,  15,  5,   5,  7,   0, 13,  13,  8,
     2, 13,   9, 14,   6, 15,   8, 14,   7,  6,  15,  6,   6,  7,   0, 14,
    14,  8,   1, 14,   5,  6,   2,  4,  13, 14,  10, 12,   4,  2,   3,  6,
    12, 10,  11, 14,   6,  3,   1,  2,  14, 11,   9, 10,   2,  1,   5,  3,
    10,  9,  13, 11,   3,  5,   4,  1,  11, 13,  12,  9,   1,  4,   6,  5,
     9, 12,  14, 13,   5, 14,   2, 12,  13,  6,  10,  4,   4, 10,   3, 14,
    12,  2,  11,  6,   6, 11,   1, 10,  14,  3,   9,  2,   2,  9,   5, 11,
    10,  1,  13,  3,   3, 13,   4,  9,  11,  5,  12,  1,   1, 12,   6, 13,
     9,  4,  14,  5,  10, 10,  11, 11,  12, 12,  13, 13,  14, 14,  15, 15,
};

static int const slang_assoc[16*16] =
{
    134, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
    28, 135, 214, 86, 219, 91, 133, 127, 26, 23, 240, 112, 245, 117, 141, 126,
    37, 211, 142, 83, 206, 132, 78, 160, 35, 237, 32, 109, 232, 140, 104, 161,
    46, 87, 82, 143, 131, 215, 210, 169, 44, 113, 108, 41, 139, 241, 236, 170,
    55, 222, 203, 130, 144, 94, 75, 178, 53, 248, 229, 138, 50, 120, 101, 179,
    64, 90, 129, 218, 95, 145, 223, 187, 62, 116, 137, 244, 121, 59, 249, 188,
    73, 128, 79, 207, 74, 202, 146, 196, 71, 136, 105, 233, 100, 228, 68, 197,
    122, 153, 162, 171, 180, 189, 198, 147, 16, 25, 34, 43, 52, 61, 70, 18,
    15, 27, 36, 45, 54, 63, 72, 17, 151, 155, 164, 173, 182, 191, 200, 124,
    154, 22, 238, 110, 243, 115, 156, 24, 150, 152, 216, 88, 221, 93, 148, 20,
    163, 235, 31, 107, 230, 165, 102, 33, 159, 213, 250, 85, 208, 157, 80, 29,
    172, 111, 106, 40, 174, 239, 234, 42, 168, 89, 84, 251, 166, 217, 212, 38,
    181, 246, 227, 183, 49, 118, 99, 51, 177, 224, 205, 175, 252, 96, 77, 47,
    190, 114, 192, 242, 119, 58, 247, 60, 186, 92, 184, 220, 97, 253, 225, 56,
    199, 201, 103, 231, 98, 226, 67, 69, 195, 193, 81, 209, 76, 204, 254, 65,
    123, 149, 158, 167, 176, 185, 194, 19, 125, 21, 30, 39, 48, 57, 66, 255,
};

/*
 * Local functions
 */
static void slang_init_palette(void);
static void slang_write_utf32(uint32_t);

#if defined(HAVE_SIGNAL)
static RETSIGTYPE sigwinch_handler(int);
static caca_display_t *sigwinch_d; /* FIXME: we ought to get rid of this */
#endif
#if defined(HAVE_GETENV) && defined(HAVE_PUTENV)
static void slang_install_terminal(caca_display_t *);
static void slang_uninstall_terminal(caca_display_t *);
#endif

struct driver_private
{
    char *term;
};

static void default_sigint (int sig)
{
    /* Do nothing */
    return;
}

static int slang_init_graphics(caca_display_t *dp)
{
    dp->drv.p = malloc(sizeof(struct driver_private));

#if defined(HAVE_GETENV) && defined(HAVE_PUTENV)
    slang_install_terminal(dp);
#endif

#if defined(HAVE_SIGNAL)
    sigwinch_d = dp;
    signal(SIGWINCH, sigwinch_handler);
#endif

    _caca_set_term_title("caca for S-Lang");

    /* Initialise slang library */
    SLsig_block_signals();
    SLtt_get_terminfo();

    if(SLkp_init() == -1)
    {
        SLsig_unblock_signals();
        return -1;
    }

    SLang_init_tty(-1, 0, 1);

    if(SLsmg_init_smg() == -1)
    {
        SLsig_unblock_signals();
        return -1;
    }

    SLsig_unblock_signals();

    SLsmg_cls();
    SLtt_set_cursor_visibility(0);
    SLkp_define_keysym("\e[M", 1001);
    SLtt_set_mouse_mode(1, 0);
    SLsmg_refresh();

    
    /* Disable SLang's own SIGINT on ctrl-c */
    SLang_set_abort_signal (default_sigint);
    
    /* Disable scrolling so that hashmap scrolling optimization code
     * does not cause ugly refreshes due to slow terminals */
    SLtt_Term_Cannot_Scroll = 1;

    slang_init_palette();

#if defined(VMS) || defined(REAL_UNIX_SYSTEM)
    /* Disable alt charset support so that we get a chance to have all
     * 256 colour pairs */
    SLtt_Has_Alt_Charset = 0;
#endif

#ifdef HAVE_SLSMG_UTF8_ENABLE
    SLsmg_utf8_enable(1); /* 1 == force, 0 == disable, -1 == autodetect */
    SLtt_utf8_enable(1);
#endif

    caca_add_dirty_rect(dp->cv, 0, 0, dp->cv->width, dp->cv->height);
    dp->resize.allow = 1;
    caca_set_canvas_size(dp->cv, SLtt_Screen_Cols, SLtt_Screen_Rows);
    dp->resize.allow = 0;

    return 0;
}

static int slang_end_graphics(caca_display_t *dp)
{
    _caca_set_term_title("");
    SLtt_set_mouse_mode(0, 0);
    SLtt_set_cursor_visibility(1);
    SLang_reset_tty();
    SLsmg_reset_smg();

#if defined HAVE_GETENV && defined HAVE_PUTENV
    slang_uninstall_terminal(dp);
#endif

    free(dp->drv.p);

    return 0;
}

static int slang_set_display_title(caca_display_t *dp, char const *title)
{
    _caca_set_term_title(title);

    return 0;
}

static int slang_get_display_width(caca_display_t const *dp)
{
    /* Fallback to a 6x10 font */
    return caca_get_canvas_width(dp->cv) * 6;
}

static int slang_get_display_height(caca_display_t const *dp)
{
    /* Fallback to a 6x10 font */
    return caca_get_canvas_height(dp->cv) * 10;
}

static void slang_display(caca_display_t *dp)
{
    int x, y, i;

    for(i = 0; i < caca_get_dirty_rect_count(dp->cv); i++)
    {
        uint32_t const *cvchars, *cvattrs;
        int dx, dy, dw, dh;

        caca_get_dirty_rect(dp->cv, i, &dx, &dy, &dw, &dh);

        cvchars = (uint32_t const *)caca_get_canvas_chars(dp->cv)
                    + dx + dy * dp->cv->width;
        cvattrs = (uint32_t const *)caca_get_canvas_attrs(dp->cv)
                    + dx + dy * dp->cv->width;

        for(y = dy; y < dy + dh; y++)
        {
            SLsmg_gotorc(y, dx);
            for(x = dx; x < dx + dw; x++)
            {
                uint32_t ch = *cvchars++;

#if defined(OPTIMISE_SLANG_PALETTE)
                /* If foreground == background, just don't use this colour
                 * pair, and print a space instead of the real character. */
                /* XXX: disabled, because I can't remember what it was
                 * here for, and in cases where SLang does not render
                 * bright backgrounds, it's just fucked up. */
#if 0
                uint8_t fgcolor = caca_attr_to_ansi_fg(*cvattrs);
                uint8_t bgcolor = caca_attr_to_ansi_bg(*cvattrs);

                if(fgcolor >= 0x10)
                    fgcolor = CACA_LIGHTGRAY;

                if(bgcolor >= 0x10)
                    bgcolor = CACA_BLACK; /* FIXME: handle transparency */

                if(fgcolor == bgcolor)
                {
                    if(fgcolor == CACA_BLACK)
                        fgcolor = CACA_WHITE;
                    else if(fgcolor == CACA_WHITE
                             || fgcolor <= CACA_LIGHTGRAY)
                        fgcolor = CACA_BLACK;
                    else
                        fgcolor = CACA_WHITE;
                    SLsmg_set_color(slang_assoc[fgcolor + 16 * bgcolor]);
                    SLsmg_write_char(' ');
                    cvattrs++;
                }
                else
#endif
                {
                    SLsmg_set_color(slang_assoc[caca_attr_to_ansi(*cvattrs++)]);
                    slang_write_utf32(ch);
                }
#else
                SLsmg_set_color(caca_attr_to_ansi(*cvattrs++));
                slang_write_utf32(ch);
#endif
            }

            cvchars += dp->cv->width - dw;
            cvattrs += dp->cv->width - dw;
        }
    }
    SLsmg_gotorc(caca_wherey(dp->cv), caca_wherex(dp->cv));
    SLsmg_refresh();
}

static void slang_handle_resize(caca_display_t *dp)
{
    SLtt_get_screen_size();
    dp->resize.w = SLtt_Screen_Cols;
    dp->resize.h = SLtt_Screen_Rows;

    if(dp->resize.w != caca_get_canvas_width(dp->cv)
        || dp->resize.h != caca_get_canvas_height(dp->cv))
        SLsmg_reinit_smg();
}

static int slang_get_event(caca_display_t *dp, caca_privevent_t *ev)
{
    int intkey;

    if(!SLang_input_pending(0))
    {
        ev->type = CACA_EVENT_NONE;
        return 0;
    }

    /* We first use SLang_getkey() to see whether Esc was pressed
     * alone, then (if it wasn't) we unget the key and use SLkp_getkey()
     * instead, so that escape sequences are interpreted. */
    intkey = SLang_getkey();

    if(intkey != 0x1b /* Esc */ || SLang_input_pending(0))
    {
        SLang_ungetkey(intkey);
        intkey = SLkp_getkey();
    }

    /* If the key was ASCII, return it immediately */
    if(intkey < 0x7f)
    {
        ev->type = CACA_EVENT_KEY_PRESS;
        ev->data.key.ch = intkey;
        ev->data.key.utf32 = intkey;
        ev->data.key.utf8[0] = intkey;
        ev->data.key.utf8[1] = '\0';
        return 1;
    }

    /* If the key was UTF-8, parse the whole sequence */
    if(intkey >= 0x80 && intkey < 0x100)
    {
        int keys[7]; /* Necessary for ungetkey(); */
        char utf8[7];
        uint32_t utf32;
        size_t i, bytes = 0;

        keys[0] = intkey;
        utf8[0] = intkey;

        for(i = 1; i < 6; i++)
        {
            if(!SLang_input_pending(0))
                break;
            keys[i] = SLang_getkey();
            utf8[i] = (unsigned char)keys[i];
        }

        utf8[i] = '\0';
        utf32 = caca_utf8_to_utf32(utf8, &bytes);

        while(i > bytes)
            SLang_ungetkey(keys[--i]);

        if(bytes)
        {
            ev->type = CACA_EVENT_KEY_PRESS;
            ev->data.key.ch = 0;
            ev->data.key.utf32 = utf32;
            strcpy(ev->data.key.utf8, utf8);
            return 1;
        }
    }

    if(intkey == 0x3e9)
    {
        int button = (SLang_getkey() - ' ' + 1) & 0xf;
        int x = SLang_getkey() - '!';
        int y = SLang_getkey() - '!';

        ev->data.mouse.button = button;
        ev->type = CACA_EVENT_MOUSE_PRESS;
        _push_event(dp, ev);
        ev->type = CACA_EVENT_MOUSE_RELEASE;
        _push_event(dp, ev);

        if(dp->mouse.x == x && dp->mouse.y == y)
            return _pop_event(dp, ev);

        dp->mouse.x = x;
        dp->mouse.y = y;

        ev->type = CACA_EVENT_MOUSE_MOTION;
        ev->data.mouse.x = dp->mouse.x;
        ev->data.mouse.y = dp->mouse.y;
        return 1;
    }

    switch(intkey)
    {
        case SL_KEY_UP: ev->data.key.ch = CACA_KEY_UP; break;
        case SL_KEY_DOWN: ev->data.key.ch = CACA_KEY_DOWN; break;
        case SL_KEY_LEFT: ev->data.key.ch = CACA_KEY_LEFT; break;
        case SL_KEY_RIGHT: ev->data.key.ch = CACA_KEY_RIGHT; break;

        case SL_KEY_IC: ev->data.key.ch = CACA_KEY_INSERT; break;
        case SL_KEY_DELETE: ev->data.key.ch = CACA_KEY_DELETE; break;
        case 0x7f:
        case SL_KEY_BACKSPACE: ev->data.key.ch = CACA_KEY_BACKSPACE; break;
        case SL_KEY_HOME: ev->data.key.ch = CACA_KEY_HOME; break;
        case SL_KEY_END: ev->data.key.ch = CACA_KEY_END; break;
        case SL_KEY_PPAGE: ev->data.key.ch = CACA_KEY_PAGEUP; break;
        case SL_KEY_NPAGE: ev->data.key.ch = CACA_KEY_PAGEDOWN; break;

        case SL_KEY_F(1): ev->data.key.ch = CACA_KEY_F1; break;
        case SL_KEY_F(2): ev->data.key.ch = CACA_KEY_F2; break;
        case SL_KEY_F(3): ev->data.key.ch = CACA_KEY_F3; break;
        case SL_KEY_F(4): ev->data.key.ch = CACA_KEY_F4; break;
        case SL_KEY_F(5): ev->data.key.ch = CACA_KEY_F5; break;
        case SL_KEY_F(6): ev->data.key.ch = CACA_KEY_F6; break;
        case SL_KEY_F(7): ev->data.key.ch = CACA_KEY_F7; break;
        case SL_KEY_F(8): ev->data.key.ch = CACA_KEY_F8; break;
        case SL_KEY_F(9): ev->data.key.ch = CACA_KEY_F9; break;
        case SL_KEY_F(10): ev->data.key.ch = CACA_KEY_F10; break;
        case SL_KEY_F(11): ev->data.key.ch = CACA_KEY_F11; break;
        case SL_KEY_F(12): ev->data.key.ch = CACA_KEY_F12; break;

        default:
            /* Unknown key */
            ev->type = CACA_EVENT_NONE; return 0;
    }

    ev->type = CACA_EVENT_KEY_PRESS;
    ev->data.key.utf32 = 0;
    ev->data.key.utf8[0] = '\0';
    return 1;
}

static void slang_set_cursor(caca_display_t *dp, int flags)
{
    SLtt_set_cursor_visibility(flags ? 1 : 0);
}

/*
 * XXX: following functions are local
 */

static void slang_init_palette(void)
{
    /* See SLang ref., 5.4.4. */
    static char *slang_colors[16] =
    {
        /* Standard colours */
        "black",
        "blue",
        "green",
        "cyan",
        "red",
        "magenta",
        "brown",
        "lightgray",
        /* Bright colours */
        "gray",
        "brightblue",
        "brightgreen",
        "brightcyan",
        "brightred",
        "brightmagenta",
        "yellow",
        "white",
    };

#if defined(OPTIMISE_SLANG_PALETTE)
    int i;

    for(i = 0; i < 16 * 16; i++)
        SLtt_set_color(i, NULL, slang_colors[slang_palette[i * 2]],
                                slang_colors[slang_palette[i * 2 + 1]]);
#else
    int fg, bg;

    for(bg = 0; bg < 16; bg++)
        for(fg = 0; fg < 16; fg++)
        {
            int i = fg + 16 * bg;
            SLtt_set_color(i, NULL, slang_colors[fg], slang_colors[bg]);
        }
#endif
}

static void slang_write_utf32(uint32_t ch)
{
#ifdef HAVE_SLSMG_UTF8_ENABLE
    char buf[10];
    int bytes;
#else
    char ascii;
#endif

    if(ch == CACA_MAGIC_FULLWIDTH)
        return;

#ifdef HAVE_SLSMG_UTF8_ENABLE
    bytes = caca_utf32_to_utf8(buf, ch);
    buf[bytes] = '\0';
    SLsmg_write_string(buf);
#else
    ascii = caca_utf32_to_ascii(ch);
    SLsmg_write_char(ascii);
    if(caca_utf32_is_fullwidth(ch))
        SLsmg_write_char(ascii);
#endif
}

#if defined(HAVE_SIGNAL)
static RETSIGTYPE sigwinch_handler(int sig)
{
    sigwinch_d->resize.resized = 1;

    signal(SIGWINCH, sigwinch_handler);
}
#endif

#if defined(HAVE_GETENV) && defined(HAVE_PUTENV)
static void slang_install_terminal(caca_display_t *dp)
{
    char *term, *colorterm;

    dp->drv.p->term = NULL;

    term = getenv("TERM");
    colorterm = getenv("COLORTERM");

    if(!term || strcmp(term, "xterm"))
        return;

    /* If we are using gnome-terminal, it's really a 16 colour terminal.
     * Ditto if we are using xfce4-terminal, or Konsole. */
    if((colorterm && (!strcmp(colorterm, "gnome-terminal")
                       || !strcmp(colorterm, "Terminal")))
         || getenv("KONSOLE_DCOP_SESSION"))
    {
        (void)putenv("TERM=xterm-16color");
        dp->drv.p->term = strdup(term);
        return;
    }
}

static void slang_uninstall_terminal(caca_display_t *dp)
{
    /* Needs to be persistent because we use putenv() */
    static char termenv[1024];

    if(!dp->drv.p->term)
        return;

    snprintf(termenv, 1023, "TERM=%s", dp->drv.p->term);
    free(dp->drv.p->term);
    (void)putenv(termenv);
}
#endif

/*
 * Driver initialisation
 */

int slang_install(caca_display_t *dp)
{
    dp->drv.id = CACA_DRIVER_SLANG;
    dp->drv.driver = "slang";

    dp->drv.init_graphics = slang_init_graphics;
    dp->drv.end_graphics = slang_end_graphics;
    dp->drv.set_display_title = slang_set_display_title;
    dp->drv.get_display_width = slang_get_display_width;
    dp->drv.get_display_height = slang_get_display_height;
    dp->drv.display = slang_display;
    dp->drv.handle_resize = slang_handle_resize;
    dp->drv.get_event = slang_get_event;
    dp->drv.set_mouse = NULL;
    dp->drv.set_cursor = slang_set_cursor;

    return 0;
}

#endif /* USE_SLANG */

