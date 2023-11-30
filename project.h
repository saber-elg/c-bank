#ifndef PROJECT_H
#define PROJECT_H
#include <time.h>
#include<stdio.h>
#include<stdlib.h>
#include "struct.h"

/*Prototyping the functions*/
Date catch_Date();
Time catch_Time();
void admin_or_client();

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

void admin_or_client(){
    printf("(1)    I am a client\n\n");
    printf("(2)    I am an administrator\n");
    printf("\n");
    char ch= getchar();
    system("cls");
    switch(ch){
    case '1':{
            system("cls");
            int i=0;
            do{
                // Cas où l'authentification du client échoue
                if(login_client()== 0){
                    printf("\n\n\n\n\n");
                    printf("\t\t\t\t\tInvalid E-mail or password ! retry\n");
                    getchar();// Attente de la saisie d'un caractère par l'utilisateur
                    system("cls");
                    i++;
                }else{
                    // Si l'authentification réussit, appeler la fonction client_main_page()
                    system("cls");
                    client_main_page(clients, transactions, num_clients, num_transactions); 
                    //smth still to add
                }
            }while(i<3);
            system("cls");
            admin_or_client();        
        }   
        break;
    case '2':{
            system("cls");
            int i=0;
            do{
                // Cas où l'authentification de l'administrateur échoue
                if(!login_admin()){
                    printf("\n\n\n\n\n");
                    printf("\t\t\t\t\tInvalid E-mail or password ! retry\n");
                    getchar();// Attente de la saisie d'un caractère par l'utilisateur
                    system("cls");
                    i++;
                }else{
                    // Si l'authentification réussit, appeler la fonction admin_main_page()
                    system("cls");
                    /*admin_main_page();not yet added*/
                }
            }while(i<3);
            system("cls");
            admin_or_client();
        }
        break;
    default:
        exit(1);
        break;
    }
}

#endif