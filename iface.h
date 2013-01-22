/*
   Datum:   28.01.2010 16:56
   Autor:   xferra00
   Projekt: 
   Popis:   

   Copyright (C) 2002 xferra00

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   in a file called COPYING along with this program; if not, write to
   the Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA
   02139, USA.
*/


#ifndef __IFACE_H__
#define __IFACE_H__

#define WIN_WIDTH 400
#define WIN_HEIGHT 100
#define BIT_OFFSET 10

/* initialize GUI */
int init_screen(void);

/* unload GUI */
void cleanup_screen(void);

/* show binary representation of byte */
void screen_byte(unsigned char byte);

bool sdl_process_event(void);


#endif

