/* This header contains all the structures used in this project */

#ifndef STRUCT_H
#define STRUCT_H

#include "set_const.h"

typedef struct security_qa{
    char question[SEC_QUESTION_LENGHT];
    char answer[SEC_ANSWER_LENGHT];
}Security_Qa;

typedef struct client{
    int account_status; // 0 if banned 1 if active
    int account_number;
    char first_name[FIRST_NAME_LENGHT];
    char last_name[LAST_NAME_LENGHT];  
    char CIN[MAX_CIN_LENGHT];
    char email[MAX_EMAIL_LENGHT];
    int city_id;
    double balance;
    char password[MAX_PASSWORD_LENGTH];
    Security_Qa security;
}Client;

#endif
