#ifndef PROJECT_H
#define PROJECT_H
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "struct.h" 
#include "set_const.h"
#include "client_functions.h"
#include "admin_functions.h"


/*Prototyping the functions*/

void admin_or_client();

/*Declaring the functions*/

void admin_or_client(){
    system("cls");
    printf("_________          ___.                  __ \n");
    printf("\\_   ___ \\         \\_ |__ _____    ____ |  | __\n");
    printf("/    \\  \\/   ______ | __ \\__  \\  /    \\|  |/ /\n");
    printf("\\     \\____ /_____/ | \\_\\ \\/ __ \\|   |  \\    <\n");
    printf(" \\______  /         |___  (____  /___|  /__|_ \\\n");
    printf("        \\/              \\/     \\/     \\/     \\/\n");
    getch();
    system("cls");
    while (1)
    {
        files_initialisation();
        system("cls");
        if (fopen(PATH_CLIENT_BIN_FILE, "rb") == NULL || fopen(PATH_STAGING_CLIENT_BIN_FILE, "rb") == NULL) 
        {
            files_initialisation();
        }
        printf("*********************  c-bank  *********************\n\n");
        printf("\n(1)    I am a client\n\n");
        printf("(2)    I am an administrator\n\n");
        printf("(3)    Shut down");
        char choice = getch();
        while((choice != '1') && (choice != '2') && (choice != '3'))
        {
            system("cls");
            printf("*********************  c-bank  *********************\n\n");
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
                            printf("\t\t\t\t\tInvalid Username or password ! Retry\n");
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
                printf("\n\n\n\n\n\t\t\tShutting down . . .\n\n\n\n\n\n\n\n\n");
                return;

            default:
                printf("Invalid answer.");
                getch();
                break;
        }   
    }
}
#endif
