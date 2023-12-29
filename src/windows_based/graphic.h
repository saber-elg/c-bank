#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

void start();
void loading();
void processing();
void shut_down();

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

void start (){
  system("cls");
    yellow();
    printf("\n\n");
    printf("_________          ___.                  __ \n");
    printf("\\_   ___ \\         \\_ |__ _____    ____ |  | __\n");
    printf("/    \\  \\/   ______ | __ \\__  \\  /    \\|  |/  /\n");
    printf("\\     \\____ /_____/ | \\_\\ \\/ __ \\|   |  \\    <\n");
    printf(" \\______  /         |___  ____  /___|  /__|_  \\\n");
    printf("        \\/              \\/     \\/     \\/     \\/\n\n");
    color_reset();
    Sleep(750);
    printf("\n\n");
    loading();
}

void loading(){
    printf("Loading ");
    fflush(stdout);
    int count = 0;
    green();
    for (int i = 0 ; i < 40 ; i++)
    {
        fputc('=',stdout);
        Sleep(750-70*count);
        fflush(stdout);
        if ((count+2)*70 <=750)
        {
            count+=1;
        }
    }  
    printf("\n");
    color_reset();
}

void processing (){
    printf("Processing ");
    fflush(stdout);
    for (int i = 0 ; i < 4 ; i++)
    {
        Sleep(500);
        fputc('.',stdout);
        fputc(' ',stdout);
        fflush(stdout);
    } 
    printf("\n");
}

void shut_down(){
  printf("Shutting down  ");
  fflush(stdout);
  for (int i = 0 ; i < 4 ; i++)
  {
      Sleep(500);
      fputc('.',stdout);
      fputc(' ',stdout);
      fflush(stdout);
  } 
  printf("\n");
  system("cls");
  Sleep(1000);
  red();
  printf("\t\t   Developed by :\n\n");
  green();
  printf("El Guelta Mohamed-Saber ");
  color_reset();
  printf(" &&  ");
  green();
  printf(" El  Hadifi  Soukaina");
  color_reset();
  getch();
}

void goodbye(){
  printf("Goodbye ");
    fflush(stdout);
    for (int i = 0 ; i < 4 ; i++)
    {
        Sleep(500);
        fputc('.',stdout);
        fputc(' ',stdout);
        fflush(stdout);
    } 
    printf("\n");
}

void logging_in(){
    printf("Logging in ");
    fflush(stdout);
    for (int i = 0 ; i < 4 ; i++)
    {
        Sleep(500);
        fputc('.',stdout);
        fputc(' ',stdout);
        fflush(stdout);
    } 
    printf("\n");
}

void logging_out(){
    printf("Logging out ");
    fflush(stdout);
    for (int i = 0 ; i < 4 ; i++)
    {
        Sleep(500);
        fputc('.',stdout);
        fputc(' ',stdout);
        fflush(stdout);
    } 
    printf("\n");
}

#endif