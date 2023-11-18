#ifndef ADMIN_FUNCTIONS_H
#define ADMIN_FUNCTIONS_H

#include "struct.h"
#include "set_const.h"
#include "client_functions.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Prototype of functions*/

int login_admin();

/*Creation of functions*/

int login_admin(){
    printf("Admin\n\n");
    printf("***************** Authentification *****************");
    Auth* temp = (Auth*)malloc(sizeof(Auth));
    printf("Email      :   ");
    fgets(temp->email,MAX_EMAIL_LENGHT,stdin);
    printf("Password   :   ");
    strcpy(temp->password,get_password());
    if(!strcmp(temp->email,"admin") && !strcmp(temp->password,"admin"))
        return 1;
    return 0;
}



#endif