#ifndef STRUCT_H
#define STRUCT_H
#include "set_const.h"
/* This file contains all the structures used in this project */

typedef struct time{
    int hour,minute,second;
}Time;

typedef struct date{
    int day,month,year;
}Date;

typedef struct security_qa{
    char question[SEC_QUESTION_LENGHT];
    char answer[SEC_ANSWER_LENGHT];
}Security_Qa;

typedef struct auth{
    char email[MAX_EMAIL_LENGHT];
    char password[MAX_PASSWORD_LENGTH];//char password[MAX_PASSWORD_LENGTH]; will be affected to hash
}Auth;

typedef struct client{
    int account_number;
<<<<<<< HEAD
    char CNE[MAX_CNE_LENGHT];
    double balance;
    char first_name[FIRST_NAME_LENGHT];
    char last_name[LAST_NAME_LENGHT];  
=======
    double balance;
    char first_name[FIRST_NAME_LENGHT];
    char last_name[LAST_NAME_LENGHT];
    char email[MAX_EMAIL_LENGHT];
    char password[MAX_PASSWORD_LENGTH];
>>>>>>> 31a747e (reinitialise the repo)
    Auth auth;
    Security_Qa security;
}Client;

#endif


