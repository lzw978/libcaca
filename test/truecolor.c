/*
 *  truecolor     truecolor canvas features
 *  Copyright (c) 2006 Sam Hocevar <sam@zoy.org>
 *                All Rights Reserved
 *
 *  $Id$
 *
 *  This program is free software. It comes without any warranty, to
 *  the extent permitted by applicable law. You can redistribute it
 *  and/or modify it under the terms of the Do What The Fuck You Want
 *  To Public License, Version 2, as published by Sam Hocevar. See
 *  http://sam.zoy.org/wtfpl/COPYING for more details.
 */

#include "config.h"
#include "common.h"

#if !defined(__KERNEL__)
#   if defined(HAVE_INTTYPES_H)
#      include <inttypes.h>
#   endif
#   include <stdio.h>
#endif

#include "cucul.h"
#include "caca.h"

int main(int argc, char *argv[])
{
    cucul_canvas_t *cv;
    caca_display_t *dp;

    int x, y;

    cv = cucul_create_canvas(32, 16);
    if(cv == NULL)
    {
        printf("Failed to create canvas\n");
        return 1;
    }

    dp = caca_create_display(cv);
    if(dp == NULL)
    {
        printf("Failed to create display\n");
        return 1;
    }

    for(y = 0; y < 16; y++)
        for(x = 0; x < 16; x++)
    {
        uint16_t bgcolor = 0xff00 | (y << 4) | x;
        uint16_t fgcolor = 0xf000 | ((15 - y) << 4) | ((15 - x) << 8);

        cucul_set_color_argb(cv, fgcolor, bgcolor);
        cucul_put_str(cv, x * 2, y, "CA");
    }

    cucul_set_color_ansi(cv, CUCUL_WHITE, CUCUL_LIGHTBLUE);
    cucul_put_str(cv, 2, 1, " truecolor libcaca ");

    caca_refresh_display(dp);

    caca_get_event(dp, CACA_EVENT_KEY_PRESS, NULL, -1);

    caca_free_display(dp);
    cucul_free_canvas(cv);

    return 0;
}

