#ifndef ADMIN_FUNCTIONS_H
#define ADMIN_FUNCTIONS_H

#include"struct.h"
#include <time.h>
#include <stdio.h>
<<<<<<< HEAD
#include<stdlib.h>
=======
>>>>>>> 31a747e (reinitialise the repo)
#include <string.h>
//#include <openssl/sha.h>

/*Prototype of functions*/


int create_client_account();
int login_admin();
//void hash_password(const char *password, char *hash);

/*Creation of functions*/
int create_client_account(){
    Client *client = (Client*)malloc(sizeof(Client));
    printf("******************     Creer un compte      ******************\n");
    /*content*/
    printf("First Name   :   ");
    fgets(client->first_name,FIRST_NAME_LENGHT,stdin);
    printf("Last Name    :   ");
    fgets(client->last_name,LAST_NAME_LENGHT,stdin);
    printf("Email        :   ");
    fgets(client->auth.email,MAX_EMAIL_LENGHT,stdin);
    client->auth.password,MAX_PASSWORD_LENGTH,stdin;
    system("clear");
    return 1;
}

int login_admin(){
    printf("Admin\n\n");
    printf("***************** Authentification *****************");
    Auth* temp = (Auth*)malloc(sizeof(Auth));
    char* email=(char*)malloc(MAX_EMAIL_LENGHT*sizeof(char));
    password = NULL;
    printf("Email      :   ");
    fgets(email,MAX_EMAIL_LENGHT,stdin);
    printf("Password   :   ");
    password = get_password();

    if(!strcmp(email,"admin@gmail.com") && !strcmp(password,"admin1234"))
        return 1;
    return 0;
}





/*void hash_password(const char *password, char *hash) {
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, password, strlen(password));
    SHA256_Final((unsigned char*)hash, &sha256);
}*/



#endif