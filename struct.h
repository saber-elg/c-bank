
/* This file contains all the structures used in this project */

typedef struct time
{
    int hour,minute,second;
}Time;

typedef struct date
{
    int day,month,year;
}Date;


typedef struct security_qa
{
    char question[SEC_QUESTION_SIZE];
    char answer[SEC_ANSWER_SIZE];
}Security_Qa;

typedef struct auth
{
    char email[EMAIL_SIZE];
    char password[PASSWORD_SIZE];
}Auth;

typedef struct auth
{
    char email[EMAIL_SIZE];
    char password[PASSWORD_SIZE];
    char hash[]
}Auth;

typedef struct client
{
    int client_id;
    double balance;
    char first_name[FIRST_NAME_SIZE];
    char last_name[LAST_NAME_SIZE];
    char email;
    char password;
    Auth auth;
    Security_Qa security;
}Client;




