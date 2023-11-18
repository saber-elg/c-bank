#ifndef ADMIN_FUNCTIONS_H
#define ADMIN_FUNCTIONS_H

#include"struct.h"
#include <time.h>
#include <stdio.h>
#include<stdlib.h>
#include <string.h>
//#include <openssl/sha.h>

/*Prototype of functions*/

int login_admin();

//void hash_password(const char *password, char *hash);

/*Creation of functions*/

int login_admin(){
    printf("Admin\n\n");
    printf("***************** Authentification *****************");
    Auth* temp = (Auth*)malloc(sizeof(Auth));
    printf("Email      :   ");
    fgets(temp->email,MAX_EMAIL_LENGHT,stdin);
    printf("Password   :   ");
    temp->password = get_password();
    if(!strcmp(temp->email,"admin") && !strcmp(temp->password,"admin"))
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