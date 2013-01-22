/*
   File name: main.c
Date:      28.01.2010 16:51
Author:    xferra00
Project:   

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
#define _POSIX_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>
#include <sys/types.h>  //mkfifo
#include <sys/stat.h>   //mkfifo
#include <sys/wait.h>
//#include <sys/select.h>

#include "iface.h"

#define FIFO_NAME "input"

int *app_exit;

void sigint_handler(int signal){
   switch (signal){
      case SIGTERM: *app_exit = 1;
         break;
      case SIGINT:   *app_exit = 1;
                    break;
   }
   
}

void print_help(FILE *stream){
   fprintf(stream, "virtual byte device\n\
         Write bytes to 'input' file and see its binary representation\n");
   return;
}


int main(int argc, char *argv[])
{

   int x=0;
   app_exit = &x ;

   if (argc == 2){
      if (strcmp(argv[1], "-h")==0){
         print_help(stdout);
         exit(EXIT_SUCCESS);
      }else{
         fprintf(stderr, "unknown parameter!\n");
         exit(EXIT_FAILURE);
      }
   }else if (argc != 1){
      fprintf(stderr, "unknown parameter, see vp -h for help\n");
      exit(EXIT_FAILURE);
   }

   int fifo_wfd, fifo_rfd;
   int res;

   signal(SIGTERM, sigint_handler);

   // unlink existing fifo if it exists
   res = unlink(FIFO_NAME);
   if (res != 0){
      perror("removing stale fifo");
      //return 1;
   }
   // create fifo
   res = mkfifo(FIFO_NAME, 0777);
   if (res != 0){
      perror("mkfifo");
      return 1;
   }

   if (fork()){
      // parent

      // open it writeonly again, so other clients dont trigger EOF to be sent
      // when they close file
      fifo_wfd = open(FIFO_NAME, O_WRONLY);
      if (fifo_wfd < 0){
         perror("opening fifo writeonly");
         *app_exit = 1;
         wait(NULL);
         return 1;
      }

      printf("parent: going to sleep\n");
      printf("parent: sleeping, waiting for exit request at %p\n", (void *)app_exit);
      while(*app_exit == 0){
         //printf("sleep 1, app_exit: %d\n", *app_exit);
         sleep(1);
      }
   
      printf("waiting for child to exit\n");
      wait(NULL);
      // exit request
      close(fifo_wfd);
      exit(0);
   }else{
      //child
      // open fifo for reading
      fifo_rfd = open(FIFO_NAME, O_RDONLY);
      if (fifo_rfd < 0){
         perror("opening fifo for reading");
         return 1;
      }

      // init interface
      res = init_screen();
      if (res != 0){
         fprintf(stderr, "Cannot initialize graphics, exiting.\n");
         // close(fifo_wfd);
         close(fifo_rfd);
         return 1;
      }

      // all set up, listen at fifo
      char c=1;

      while ( c != 0 && (*app_exit == 0)){
         // loop

         if (sdl_process_event() != true){
            break;
         }

         res = read(fifo_rfd, &c, 1);
         if (res < 0){
            // read could be interupted (errno is EINTR),
            // in our case, we can silently ignore it
            ;
         }else if(res == 0){
            // EOF - this shouldnt happen, as we have fifo 
            // opened for writing
            // logit
            fprintf(stderr, "EOF from fifo ! Exiting.\n");
            break;
         }

         screen_byte(c);
      }

      printf("child requesting exit\n");
      kill(getppid(), SIGTERM);
      //close(fifo_wfd);
      close(fifo_rfd);
      unlink(FIFO_NAME);

      cleanup_screen();

      if (res == 0){
         // EOF from fifo - this isnt right
         return EXIT_FAILURE;
      }else{
         return EXIT_SUCCESS;
      }

   } // fork()
}
