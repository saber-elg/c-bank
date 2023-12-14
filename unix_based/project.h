#ifndef PROJECT_H
#define PROJECT_H
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "struct.h" 
#include "set_const.h"
#include "client_functions.h"
#include "admin_functions.h"
#include "graphic.h"

/*Prototyping the functions*/

void admin_or_client();

/*Declaring the functions*/

void admin_or_client(){
    hide_cursor();
    start();
    system("clear");
    while (1)
    {
        files_initialisation();
        system("clear");
        if (fopen(PATH_CLIENT_BIN_FILE, "rb") == NULL || fopen(PATH_STAGING_CLIENT_BIN_FILE, "rb") == NULL) 
        {
            files_initialisation();
        }
        yellow();
        printf("*********************** C-bank ************************\n\n");
        color_reset();
        printf("\n\t(1)  I am a client\n\n");
        printf("\t(2)  I am an administrator\n\n");
        printf("\t(3)  Shut down\n\n");
        yellow();
        printf("\n*******************************************************\n\n");
        color_reset();
        char choice = unix_getch();
        while((choice != '1') && (choice != '2') && (choice != '3'))
        {
            system("clear");
            yellow();
            printf("\n*********************** C-bank  ***********************\n\n");
            color_reset();
            printf("\n(1)  I am a client\n\n");
            printf("(2)  I am an administrator\n\n");
            printf("(3)  Shut down\n\n");
            yellow();
            printf("\n*******************************************************\n\n");
            color_reset();
            choice = unix_getch();
        }
        system("clear");
        switch(choice)
        {
            case '1':
                client_login_page();    
                break;

            case '2':
                {
                    int i=0;
                    do{
                        if(!admin_login()){
                            system("clear");
                            printf("\n\n\n\n\n");
                            blue();
                            color_reset();
                            unix_getch();
                            system("clear");
                            i++;
                        }
                        else
                        {
                            system("clear");
                            admin_main_page();
                            i=3;
                        }
                    }while(i<3);
                    system("clear");
                }
                break;
                
            case '3':
                yellow();
                shut_down();
                color_reset();
                enable_cursor();
                return;

            default:
                blue();
                printf("Invalid answer.");
                color_reset();
                unix_getch();
                break;
        }   
    }
    enable_cursor();
}
#endif
