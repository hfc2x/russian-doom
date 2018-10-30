//
// Copyright(C) 1993-1996 Id Software, Inc.
// Copyright(C) 2005-2014 Simon Howard
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
// DESCRIPTION:
//  Sky rendering. The DOOM sky is a texture map like any
//  wall, wrapping around. A 1024 columns equal 360 degrees.
//  The default sky map is 256 columns and repeats 4 times
//  on a 320 screen?
//  
//

// Russian Doom (C) 2016-2018 Julian Nechaevsky


// HEADER FILES ------------------------------------------------------------

#include "m_fixed.h"    // Needed for FRACUNIT
#include "r_data.h"     // Needed for Flat retrieval
#include "r_sky.h"

// PUBLIC DATA DEFINITIONS -------------------------------------------------

int skyflatnum;         //
int skytexture;         // sky mapping
int skytexturemid;      //

// EXTERNAL DATA DECLARATIONS ----------------------------------------------

extern boolean scaled_sky;

// CODE ====================================================================

// -------------------------------------------------------------------------
// R_InitSkyMap
// Called whenever the view size changes.
// -------------------------------------------------------------------------

void R_InitSkyMap (void)
{
    if (scaled_sky)
    {
        skytexturemid = ORIGHEIGHT/2*FRACUNIT;
    }
    else
    {
        skytexturemid = ORIGHEIGHT+199 * FRACUNIT;
    }
}