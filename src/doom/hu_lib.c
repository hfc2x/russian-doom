//
// Copyright(C) 1993-1996 Id Software, Inc.
// Copyright(C) 2005-2014 Simon Howard
// Copyright(C) 2016-2020 Julian Nechaevsky
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// DESCRIPTION:  heads-up text and input code
//



#include <ctype.h>

#include "doomdef.h"
#include "doomkeys.h"
#include "v_video.h"
#include "i_swap.h"
#include "hu_lib.h"
#include "r_local.h"
#include "r_draw.h"
#include "v_trans.h"
#include "jn.h"

// boolean : whether the screen is always erased
#define noterased viewwindowx

extern boolean	automapactive;	// in AM_map.c
extern GameMission_t gamemission; // [JN] For uncolored Jaguar messages


void HUlib_init(void)
{
    // 
}


void HUlib_clearTextLine(hu_textline_t* t)
{
    t->len = 0;
    t->l[0] = 0;
    t->needsupdate = true;
}


void HUlib_initTextLine (hu_textline_t* t, int x, int y, patch_t** f, int sc)
{
    t->x = x;
    t->y = y;
    t->f = f;
    t->sc = sc;
    HUlib_clearTextLine(t);
}


boolean HUlib_addCharToTextLine (hu_textline_t* t, char ch)
{
    if (t->len == HU_MAXLINELENGTH)
    return false;
    else
    {
        t->l[t->len++] = ch;
        t->l[t->len] = 0;
        t->needsupdate = 4;
        return true;
    }
}


boolean HUlib_delCharFromTextLine(hu_textline_t* t)
{
    if (!t->len) 
    return false;
    else
    {
        t->l[--t->len] = 0;
        t->needsupdate = 4;
        return true;
    }
}


void
HUlib_drawTextLine (hu_textline_t* l, boolean drawcursor)
{
    int i;
    int w;
    int x;
    boolean wide_4_3 = (aspect_ratio >= 2 && screenblocks == 9);

    // draw the new stuff
    x = l->x;
    for (i=0;i<l->len;i++)
    {
        unsigned char c = toupper(l->l[i]);

        if (c != ' ' && c >= l->sc && c <= '_')
        {
            w = SHORT(l->f[c - l->sc]->width);

            if (x+w > origwidth)
            break;

            // [JN] Colorize message if necessary
            if (messages_pickup_color > 0 && !vanillaparm && gamemission != jaguar)
            {
                messages_pickup_color == 1 ? dp_translation = cr[CR_GREEN] :
                messages_pickup_color == 2 ? dp_translation = cr[CR_BLUE2] :
                messages_pickup_color == 3 ? dp_translation = cr[CR_GOLD]  :
                messages_pickup_color == 4 ? dp_translation = cr[CR_GRAY]  :
                messages_pickup_color == 5 ? dp_translation = cr[CR_TAN]   :
                messages_pickup_color == 6 ? dp_translation = cr[CR_BROWN] :
                messages_pickup_color == 7 ? dp_translation = cr[CR_BRICK] : NULL;
            }
        
            V_DrawShadowedPatchDoom(x + (wide_4_3 ? wide_delta : 0),
                                    l->y, l->f[c - l->sc]);

            // [JN] Clear color translation if necessary
            if (messages_pickup_color > 0 && !vanillaparm && gamemission != jaguar)
            dp_translation = NULL;

            x += w;
        }
        else
        {
        x += 4;

        if (x >= origwidth)
        break;
        }
    }

    // draw the cursor if requested
    if (drawcursor && x + SHORT(l->f['_' - l->sc]->width) <= origwidth)
    {
        // [JN] Colorize message if necessary
        if (messages_pickup_color > 0 && !vanillaparm)
        {
            messages_pickup_color == 1 ? dp_translation = cr[CR_GREEN] :
            messages_pickup_color == 2 ? dp_translation = cr[CR_BLUE2] :
            messages_pickup_color == 3 ? dp_translation = cr[CR_GOLD]  :
            messages_pickup_color == 4 ? dp_translation = cr[CR_GRAY]  :
            messages_pickup_color == 5 ? dp_translation = cr[CR_TAN]   :
            messages_pickup_color == 6 ? dp_translation = cr[CR_BROWN] :
            messages_pickup_color == 7 ? dp_translation = cr[CR_BRICK] : NULL;
        }

        V_DrawShadowedPatchDoom(x + (wide_4_3 ? wide_delta : 0),
                                l->y, l->f['_' - l->sc]);

        // [JN] Clear color translation if necessary
        if (messages_pickup_color > 0 && !vanillaparm)
        dp_translation = NULL;
    }
}

void HUlib_drawTextLineUncolored (hu_textline_t *l, boolean drawcursor)
{
    int i;
    int w;
    int x;
    boolean wide_4_3 = (aspect_ratio >= 2 && screenblocks == 9);

    // draw the new stuff
    x = l->x;
    for (i=0;i<l->len;i++)
    {
        unsigned char c = toupper(l->l[i]);

        if (c != ' ' && c >= l->sc && c <= '_')
        {
            w = SHORT(l->f[c - l->sc]->width);

            if (x+w > origwidth)
            break;

            V_DrawShadowedPatchDoom(x + (wide_4_3 ? wide_delta : 0),
                                    l->y, l->f[c - l->sc]);

            x += w;
        }
        else
        {
        x += 4;

        if (x >= origwidth)
        break;
        }
    }

    // draw the cursor if requested
    if (drawcursor && x + SHORT(l->f['_' - l->sc]->width) <= origwidth)
    {
        V_DrawShadowedPatchDoom(x + (wide_4_3 ? wide_delta : 0),
                                l->y, l->f['_' - l->sc]);
    }
}

void HUlib_drawTextLineSecret (hu_textline_t *l)
{
    int i;
    int w;
    int x;
    boolean wide_4_3 = (aspect_ratio >= 2 && screenblocks == 9);

    // draw the new stuff
    x = l->x;
    for (i=0;i<l->len;i++)
    {
        unsigned char c = toupper(l->l[i]);

        if (c != ' ' && c >= l->sc && c <= '_')
        {
            w = SHORT(l->f[c - l->sc]->width);

            if (x+w > origwidth)
            break;

            // [JN] Colorize message if necessary
            if (messages_secret_color > 0 && !vanillaparm && gamemission != jaguar)
            {
                messages_secret_color == 1 ? dp_translation = cr[CR_GREEN] :
                messages_secret_color == 2 ? dp_translation = cr[CR_BLUE2] :
                messages_secret_color == 3 ? dp_translation = cr[CR_GOLD]  :
                messages_secret_color == 4 ? dp_translation = cr[CR_GRAY]  :
                messages_secret_color == 5 ? dp_translation = cr[CR_TAN]   :
                messages_secret_color == 6 ? dp_translation = cr[CR_BROWN] :
                messages_secret_color == 7 ? dp_translation = cr[CR_BRICK] : NULL;
            }
        
            V_DrawShadowedPatchDoom(x + (wide_4_3 ? wide_delta : 0),
                                    l->y, l->f[c - l->sc]);

            // [JN] Clear color translation if necessary
            if (messages_secret_color > 0 && !vanillaparm && gamemission != jaguar)
            dp_translation = NULL;

            x += w;
        }
        else
        {
        x += 4;

        if (x >= origwidth)
        break;
        }
    }
}

void HUlib_drawTextLineSystem (hu_textline_t *l)
{
    int i;
    int w;
    int x;
    boolean wide_4_3 = (aspect_ratio >= 2 && screenblocks == 9);

    // draw the new stuff
    x = l->x;
    for (i=0;i<l->len;i++)
    {
        unsigned char c = toupper(l->l[i]);

        if (c != ' ' && c >= l->sc && c <= '_')
        {
            w = SHORT(l->f[c - l->sc]->width);

            if (x+w > origwidth)
            break;

            // [JN] Colorize message if necessary
            if (messages_system_color > 0 && !vanillaparm && gamemission != jaguar)
            {
                messages_system_color == 1 ? dp_translation = cr[CR_GREEN] :
                messages_system_color == 2 ? dp_translation = cr[CR_BLUE2] :
                messages_system_color == 3 ? dp_translation = cr[CR_GOLD]  :
                messages_system_color == 4 ? dp_translation = cr[CR_GRAY]  :
                messages_system_color == 5 ? dp_translation = cr[CR_TAN]   :
                messages_system_color == 6 ? dp_translation = cr[CR_BROWN] :
                messages_system_color == 7 ? dp_translation = cr[CR_BRICK] : NULL;
            }
        
            V_DrawShadowedPatchDoom(x + (wide_4_3 ? wide_delta : 0),
                                    l->y, l->f[c - l->sc]);

            // [JN] Clear color translation if necessary
            if (messages_system_color > 0 && !vanillaparm && gamemission != jaguar)
            dp_translation = NULL;

            x += w;
        }
        else
        {
        x += 4;

        if (x >= origwidth)
        break;
        }
    }
}

void HUlib_drawTextLineChat (hu_textline_t *l, boolean drawcursor)
{
    int i;
    int w;
    int x;
    boolean wide_4_3 = (aspect_ratio >= 2 && screenblocks == 9);

    // draw the new stuff
    x = l->x;
    for (i=0;i<l->len;i++)
    {
        unsigned char c = toupper(l->l[i]);

        if (c != ' ' && c >= l->sc && c <= '_')
        {
            w = SHORT(l->f[c - l->sc]->width);

            if (x+w > origwidth)
            break;

            // [JN] Colorize message if necessary
            if (messages_chat_color > 0 && !vanillaparm && gamemission != jaguar)
            {
                messages_chat_color == 1 ? dp_translation = cr[CR_GREEN] :
                messages_chat_color == 2 ? dp_translation = cr[CR_BLUE2] :
                messages_chat_color == 3 ? dp_translation = cr[CR_GOLD]  :
                messages_chat_color == 4 ? dp_translation = cr[CR_GRAY]  :
                messages_chat_color == 5 ? dp_translation = cr[CR_TAN]   :
                messages_chat_color == 6 ? dp_translation = cr[CR_BROWN] :
                messages_chat_color == 7 ? dp_translation = cr[CR_BRICK] : NULL;
            }
        
            V_DrawShadowedPatchDoom(x + (wide_4_3 ? wide_delta : 0),
                                    l->y, l->f[c - l->sc]);

            // [JN] Clear color translation if necessary
            if (messages_chat_color > 0 && !vanillaparm && gamemission != jaguar)
            dp_translation = NULL;

            x += w;
        }
        else
        {
        x += 4;

        if (x >= origwidth)
        break;
        }
    }

    // draw the cursor if requested
    if (drawcursor && x + SHORT(l->f['_' - l->sc]->width) <= origwidth)
    {
        // [JN] Colorize message if necessary
        if (messages_chat_color > 0 && !vanillaparm && gamemission != jaguar)
        {
            messages_chat_color == 1 ? dp_translation = cr[CR_GREEN] :
            messages_chat_color == 2 ? dp_translation = cr[CR_BLUE2] :
            messages_chat_color == 3 ? dp_translation = cr[CR_GOLD]  :
            messages_chat_color == 4 ? dp_translation = cr[CR_GRAY]  :
            messages_chat_color == 5 ? dp_translation = cr[CR_TAN]   :
            messages_chat_color == 6 ? dp_translation = cr[CR_BROWN] :
            messages_chat_color == 7 ? dp_translation = cr[CR_BRICK] : NULL;
        }

        V_DrawShadowedPatchDoom(x + (wide_4_3 ? wide_delta : 0),
                                l->y, l->f['_' - l->sc]);

        // [JN] Clear color translation if necessary
        if (messages_chat_color > 0 && !vanillaparm && gamemission != jaguar)
        dp_translation = NULL;
    }
}


// sorta called by HU_Erase and just better darn get things straight
void HUlib_eraseTextLine(hu_textline_t* l)
{
    // Only erases when NOT in automap and the screen is reduced,
    // and the text must either need updating or refreshing
    // (because of a recent change back from the automap)

    if (!automapactive &&
    viewwindowx && l->needsupdate)
    {
        // [JN] Russian language: clear two extra pixel lines in bordered view (height+2).
        // Fixes remainings of text shadows for chars "Д", "Ц" and "Щ".
        int lh = (SHORT(l->f[0]->height+2) + 1) << hires;
        int y, yoffset;

        for (y=(l->y << hires),yoffset=y*screenwidth ; y<(l->y << hires)+lh ; y++,yoffset+=screenwidth)
        {
            if (y < viewwindowy || y >= viewwindowy + scaledviewheight)
            R_VideoErase(yoffset, screenwidth); // erase entire line
            else
            {
                R_VideoErase(yoffset, viewwindowx); // erase left border
                R_VideoErase(yoffset + viewwindowx + scaledviewwidth, viewwindowx);
                // erase right border
            }
        }
    }

    if (l->needsupdate) l->needsupdate--;
}


void HUlib_initSText (hu_stext_t* s, int x, int y, int h, patch_t** font, int startchar, boolean* on)
{
    int i;

    s->h = h;
    s->on = on;
    s->laston = true;
    s->cl = 0;
    
    for (i=0;i<h;i++)
	HUlib_initTextLine(&s->l[i], x, y - i*(SHORT(font[0]->height)+1)
                       // [JN] Critical: always draw HUD messages 1px below for
                       // preventing framebuffer overflow and normal "Й" apperance.
                       +1, font, startchar);
}


void HUlib_addLineToSText(hu_stext_t* s)
{
    int i;

    // add a clear line
    if (++s->cl == s->h)
    s->cl = 0;

    HUlib_clearTextLine(&s->l[s->cl]);

    // everything needs updating
    for (i=0 ; i<s->h ; i++)
    s->l[i].needsupdate = 4;
}


void HUlib_addMessageToSText (hu_stext_t* s, char* prefix, char* msg)
{
    HUlib_addLineToSText(s);
    
    if (prefix)
    while (*prefix)
    HUlib_addCharToTextLine(&s->l[s->cl], *(prefix++));

    while (*msg)
    HUlib_addCharToTextLine(&s->l[s->cl], *(msg++));
}


void HUlib_drawSText(hu_stext_t* s)
{
    int i;

    if (!*s->on)
    return; // if not on, don't draw

    // draw everything
    for (i=0 ; i<s->h ; i++)
    {
        int idx = s->cl - i;
        hu_textline_t *l = &s->l[idx];

        if (idx < 0)
        idx += s->h; // handle queue of lines

        // l = &s->l[idx];

        // need a decision made here on whether to skip the draw
        HUlib_drawTextLine(l, false); // no cursor, please
    }
}

void HUlib_drawSText_Secret(hu_stext_t* s)
{
    int i;

    if (!*s->on)
    return; // if not on, don't draw

    // draw everything
    for (i=0 ; i<s->h ; i++)
    {
        int idx = s->cl - i;
        hu_textline_t *l = &s->l[idx];

        if (idx < 0)
        idx += s->h; // handle queue of lines

        // need a decision made here on whether to skip the draw
        HUlib_drawTextLineSecret(l); // no cursor, please
    }
}

void HUlib_drawSText_System(hu_stext_t* s)
{
    int i;

    if (!*s->on)
    return; // if not on, don't draw

    // draw everything
    for (i=0 ; i<s->h ; i++)
    {
        int idx = s->cl - i;
        hu_textline_t *l = &s->l[idx];

        if (idx < 0)
        idx += s->h; // handle queue of lines

        // need a decision made here on whether to skip the draw
        HUlib_drawTextLineSystem(l);
    }
}

void HUlib_drawSText_Chat(hu_stext_t* s)
{
    int i;

    if (!*s->on)
    return; // if not on, don't draw

    // draw everything
    for (i=0 ; i<s->h ; i++)
    {
        int idx = s->cl - i;
        hu_textline_t *l = &s->l[idx];

        if (idx < 0)
        idx += s->h; // handle queue of lines

        // need a decision made here on whether to skip the draw
        HUlib_drawTextLineChat(l, false);
    }
}

void HUlib_eraseSText(hu_stext_t* s)
{
    int i;

    for (i=0 ; i<s->h ; i++)
    {
        if (s->laston && !*s->on)
        s->l[i].needsupdate = 4;

        HUlib_eraseTextLine(&s->l[i]);
    }

    s->laston = *s->on;
}


void HUlib_initIText (hu_itext_t* it, int x, int y, patch_t** font, int startchar, boolean* on)
{
    it->lm = 0; // default left margin is start of text
    it->on = on;
    it->laston = true;

    HUlib_initTextLine(&it->l, x, y, font, startchar);
}


// The following deletion routines adhere to the left margin restriction
void HUlib_delCharFromIText(hu_itext_t* it)
{
    if (it->l.len != it->lm)
    HUlib_delCharFromTextLine(&it->l);
}


void HUlib_eraseLineFromIText(hu_itext_t* it)
{
    while (it->lm != it->l.len)
    HUlib_delCharFromTextLine(&it->l);
}


// Resets left margin as well
void HUlib_resetIText(hu_itext_t* it)
{
    it->lm = 0;
    HUlib_clearTextLine(&it->l);
}


void HUlib_addPrefixToIText (hu_itext_t* it, char* str)
{
    while (*str)
    HUlib_addCharToTextLine(&it->l, *(str++));
    it->lm = it->l.len;
}


// wrapper function for handling general keyed input.
// returns true if it ate the key
boolean HUlib_keyInIText (hu_itext_t* it, unsigned char ch)
{
    ch = toupper(ch);

    if (ch >= ' ' && ch <= '_') 
    HUlib_addCharToTextLine(&it->l, (char) ch);
    else if (ch == KEY_BACKSPACE) 
    HUlib_delCharFromIText(it);
	else if (ch != KEY_ENTER) 
    return false; // did not eat key

    return true; // ate the key
}

void HUlib_drawIText(hu_itext_t* it)
{
    hu_textline_t *l = &it->l;

    if (!*it->on)
    return;

    HUlib_drawTextLineChat(l, true); // draw the line w/ cursor
}


void HUlib_eraseIText(hu_itext_t* it)
{
    if (it->laston && !*it->on)
    it->l.needsupdate = 4;

    HUlib_eraseTextLine(&it->l);
    it->laston = *it->on;
}

