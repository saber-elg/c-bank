#ifndef PROJECT_H
#define PROJECT_H
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "struct.h" 
#include "set_const.h"
#include "client_functions.h"
#include "admin_functions.h"
#include "graphic.h"
#include <windows.h>

/*Prototyping the functions*/

void admin_or_client();

/*Declaring the functions*/

void admin_or_client(){
    system("cls");
    loading();
    system("cls");
    yellow();
    printf("_________          ___.                  __ \n");
    printf("\\_   ___ \\         \\_ |__ _____    ____ |  | __\n");
    printf("/    \\  \\/   ______ | __ \\__  \\  /    \\|  |/ /\n");
    printf("\\     \\____ /_____/ | \\_\\ \\/ __ \\|   |  \\    <\n");
    printf(" \\______  /         |___  (____  /___|  /__|_ \\\n");
    printf("        \\/              \\/     \\/     \\/     \\/\n\n");
    color_reset();
    Sleep(2000);
    loading();
    system("cls");
    while (1)
    {
        files_initialisation();
        system("cls");
        if (fopen(PATH_CLIENT_BIN_FILE, "rb") == NULL || fopen(PATH_STAGING_CLIENT_BIN_FILE, "rb") == NULL) 
        {
            files_initialisation();
        }
        yellow();
        printf("*********************  c-bank  *********************\n\n");
        color_reset();
        printf("\n(1)    I am a client\n\n");
        printf("(2)    I am an administrator\n\n");
        printf("(3)    Shut down");
        char choice = getch();
        while((choice != '1') && (choice != '2') && (choice != '3'))
        {
            system("cls");
            yellow();
            printf("*********************  c-bank  *********************\n\n");
            color_reset();
            printf("\n(1)    I am a client\n\n");
            printf("(2)    I am an administrator\n\n");
            printf("(3)    Shut down");
            choice = getch();
        }
        system("cls");
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
                            system("cls");
                            printf("\n\n\n\n\n");
                            blue();
                            printf("\t\t\t\t\tInvalid Username or password ! Retry\n");
                            color_reset();
                            getch();
                            system("cls");
                            i++;
                        }
                        else
                        {
                            system("cls");
                            admin_main_page();
                            i=3;
                        }
                    }while(i<3);
                    system("cls");  
                }  
                break;
            
            case '3':
                yellow();
                printf("\n\n\n\n\n\t\t\tShutting down \n\n\n\n\n\n\n\n\n");
                Sleep(750);
                printf("\n\n\n\n\n\t\t\tShutting down . \n\n\n\n\n\n\n\n\n");
                Sleep(750);
                printf("\n\n\n\n\n\t\t\tShutting down . .\n\n\n\n\n\n\n\n\n");
                color_reset();
                return;

            default:
                blue();
                printf("Invalid answer.");
                color_reset();
                getch();
                break;
        }   
    }
}
#endif
