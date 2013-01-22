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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL/SDL.h>

#include "iface.h"

// global surface
SDL_Surface *screen;

Uint32 color_bit_on;
Uint32 color_bit_off;

bool sdl_process_event(void)
{
   SDL_Event event;// Objekt události

   while(SDL_PollEvent(&event))
   {
      switch(event.type)
      {
         // Klávesnice
         case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
               case SDLK_ESCAPE:
                  return false;
                  break;

               case SDLK_F1:
                  if(!SDL_WM_ToggleFullScreen(screen))
                     return false;
                  break;

               default:
                  break;
            }
            break;
            // Požadavek na ukončení
         case SDL_QUIT:
            return false;
            break;
            // Ostatní se ignorují
         default:
            break;
      }
   }

   return true;
}


/* initialize GUI */
int init_screen(void){
   
   if (SDL_Init(SDL_INIT_VIDEO) != 0){
      fprintf(stderr, "Err in SDL_Init: %s\n", SDL_GetError());
      return -1;
   }

   screen = SDL_SetVideoMode(WIN_WIDTH, WIN_HEIGHT, 0, SDL_DOUBLEBUF);
   if (screen == NULL){
      fprintf(stderr, "Unable to set video mode %s\n", SDL_GetError());
      SDL_Quit();
      return -1;
   }
   
   color_bit_on = SDL_MapRGB(screen->format, 0, 255, 0);
   color_bit_off = SDL_MapRGB(screen->format, 50, 50, 50);

   
   screen_byte(0);

   return 0; // ok
}

/* unload GUI */
void cleanup_screen(void){

   SDL_Quit();

   return;
}

/* show binary representation of byte */
void screen_byte(unsigned char byte){
   printf("byte is here: %d\n", byte);

   // divide screen on NR_BITS regions and make rectangles
   // in these regions, which will be green, if bit_is_set(pos, byte) ..
   
   SDL_Rect bit_rect = {
      .x = (WIN_WIDTH / 8) * 7 + BIT_OFFSET,
      .y = WIN_HEIGHT / 4,
      .w = (WIN_WIDTH / 8) * 0.5,
      .h = WIN_WIDTH / 8
   };

   int i;

   for (i=8;i>0; i--){// LSB first

      if ( byte & 0x01 ){
         SDL_FillRect(screen, &bit_rect, color_bit_on);
      }else{
         SDL_FillRect(screen, &bit_rect, color_bit_off);
      }

      bit_rect.x -= WIN_WIDTH / 8;

      byte = byte >> 1;
   }

   SDL_Flip(screen);
   return;
}



