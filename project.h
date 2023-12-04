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
    system("clear");
    if (fopen(PATH_CLIENT_BIN_FILE, "rb") == NULL || fopen(PATH_STAGING_CLIENT_BIN_FILE, "rb") == NULL) 
    {
        files_initialisation();
    }
    printf("*********************c-bank*********************\n\n");
    printf("\n\n(1)    I am a client\n\n");
    printf("(2)    I am an administrator\n\n");
    printf("Answer   :   ");
    char choice;
    while(((choice = getchar()) != '1') && (choice != '2'))
    {
        system("clear");
        printf("\n\n(1)    I am a client\n\n");
        printf("(2)    I am an administrator\n\n");
        printf("Answer   :   ");
    }
    system("clear");
    switch(choice){
    case '1':{
            int i=0;
            do{
                // Cas où l'authentification du client échoue
                if(client_authentification() == NULL){
                    printf("\n\n\n\n\n");
                    printf("\t\t\tInvalid E-mail or password ! retry\n");
                    getchar();// Attente de la saisie d'un caractère par l'utilisateur
                    system("clear");
                    i++;
                }
                else{
                    // Si l'authentification réussit, appeler la fonction client_main_page()
                    system("clear");
                    getchar();
                    client_main_page(*client_authentification()); 
                }
            }while(i<3);
            system("clear");
            admin_or_client();        
        }   
        break;
    case '2':{
            int i=0;
            do{
                // Cas où l'authentification de l'administrateur échoue
                if(!admin_login()){
                    printf("\n\n\n\n\n");
                    printf("\t\t\t\t\tInvalid Username or password ! retry\n");
                    getchar();// Attente de la saisie d'un caractère par l'utilisateur
                    system("clear");
                    i++;
                }else{
                    // Si l'authentification réussit, appeler la fonction admin_main_page()
                    system("clear");
                    admin_main_page();
                    i=3;
                }
            }while(i<3);
            system("clear");
            admin_or_client();
        }
        break;
    default:
        exit(1);
        break;
    }
}
#endif