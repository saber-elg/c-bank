#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <windows.h>

void red () {
  printf("\033[1;31m");
}

void green () {
  printf("\033[1;32m");
}

void yellow () {
  printf("\033[1;33m");
}

void blue () {
  printf("\033[1;34m");
}

void purple () {
  printf("\033[1;35m");
}

void cyan() {
  printf("\033[1;36m");
}

void color_reset () {
  printf("\033[0m");
}

void enable_cursor () {
    printf("\e[?25h");
}

void hide_cursor () {
    printf("\e[?25l");
}


void loading() {
    system("cls");  
    hide_cursor();
    printf("Loading ");
    fflush(stdout);

    int count = 0;
    green();

    for (int i = 0; i < 30; i++) {
        fputc('=', stdout);
        fflush(stdout);

        Sleep(750 - 70 * count);

        if ((count + 1) * 70 <= 750) {
            count += 1;
        }
    }

    printf("\n");
    color_reset();
    enable_cursor();
}
void processing (){
    system("cls");
    hide_cursor();
    printf("Processing ");
    fflush(stdout);
    for (int i = 0 ; i < 4 ; i++)
    {
        sleep(1);
        fputc('.',stdout);
        fputc(' ',stdout);
        fflush(stdout);
    } 
    printf("\n");
    enable_cursor();
}

#endif