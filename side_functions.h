#ifndef SIDE_FUNCTIONS_H
#define SIDE_FUNCTIONS_H

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "struct.h"
#include "set_const.h"
#include <termios.h>
#include <unistd.h>

/*Prototyping the functions*/
Date catch_Date();
Time catch_Time();
void fgets_no_newline_return(char* buffer,int size);
void files_initialisation();
void disableEcho();
void enableEcho();

/*Declaring the functions*/

Date catch_Date(){
    time_t t = time(NULL);
    struct tm time = *localtime(&t);
    Date date;
    date.day = time.tm_mday;
    date.month = time.tm_mon+1;
    date.year = time.tm_year+1900;
    return date;
}

Time catch_Time(){
    time_t t = time(NULL);
    struct tm time = *localtime(&t);
    Time cur_time;
    cur_time.hour = time.tm_hour;
    cur_time.minute = time.tm_min;
    cur_time.second = time.tm_sec;
    return cur_time;
}

//fgets equivalance without new linw return character '\n' the buffer should be dynamicaly allocated 
void fgets_no_newline_return(char* buffer,int size){
    fgets(buffer,size,stdin);
    buffer[strcspn(buffer,"\n")]='\0';
}

//create client file for the firrst time
void files_initialisation(){
    FILE* client_file=fopen(PATH_CLIENT_BIN_FILE,"ab");
    if (client_file == NULL) 
    {
        printf("Error while initialising clients file.");
    }
    FILE* stage_file=fopen(PATH_STAGING_CLIENT_BIN_FILE,"ab");
    if (stage_file == NULL) 
    {
        printf("Error while initialising staging clients file.");
    }
    fclose(client_file);
    fclose(stage_file);
}

// Function to disable echoing in the terminal
void disableEcho() {
    struct termios term;
    
    // Get the current terminal settings
    tcgetattr(STDIN_FILENO, &term);

    // Turn off echoing
    term.c_lflag &= ~ECHO;

    // Set the new terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

// Function to enable echoing in the terminal
void enableEcho() {
    struct termios term;
    
    // Get the current terminal settings
    tcgetattr(STDIN_FILENO, &term);

    // Turn on echoing
    term.c_lflag |= ECHO;

    // Set the new terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}


#endif