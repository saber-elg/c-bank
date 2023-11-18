#ifndef CLIENT_FUNCTIONS_H
#define CLIENT_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "struct.h"
#include "set_const.h"

/*prototyping functions*/

int create_client_account();
void display_client_profile(Client client);
char* create_num_password();
char* get_password();
Auth *authentification(char* email, char* password);
int login_admin();

/*Declaring the functions*/

int create_client_account(){
    Client *client = (Client*)malloc(sizeof(Client));
    printf("******************     Create an account      ******************\n");
    printf("First Name   :   ");
    fgets(client->first_name,FIRST_NAME_LENGHT,stdin);
    printf("Last Name    :   ");
    fgets(client->last_name,LAST_NAME_LENGHT,stdin);
    printf("Email        :   ");
    fgets(client->auth.email,MAX_EMAIL_LENGHT,stdin);
    printf("Password     :   ");
    strcpy(client->auth.password,create_num_password());
    system("clear");
    return 1;
}

void display_client_profile(Client client){
    printf("******************     Profile      *******************\n\n");
    printf("Account Number   :  %d\n",client.account_number);
    printf("Account Holder   :  %s %s\n",client.last_name,client.last_name);
    printf("Account CIN      :  %s\n",client.CIN);
    printf("Account E-mail   :  %s\n",client.auth.email);
}

char* create_num_password() {
    char* password = (char*)malloc((MAX_PASSWORD_LENGTH + 1) * sizeof(char)); // Allocate memory for the password

    if (password == NULL) {
        printf("Password memory allocation failed\n");
        exit(1);    
    }//Assuring the memory allocation for the password                                       

    printf("Enter password  : ");
    char ch;
    int i = 0;
    while ((ch = getchar()) != '\n' && i < MAX_PASSWORD_LENGTH) 
    {
        if (isdigit(ch)) {
            password[i] = ch;
            i++;
        } else if (ch == 8 && i > 0) {  // Backspace (ASCII backspace==8)
            printf("\b \b");  // Move cursor back, print space, move cursor back again
            i--;
        }
    }
    if (i < MIN_PASSWORD_LENGTH) {
        printf("\nError: Password must be at least %d digits long.\n", MIN_PASSWORD_LENGTH);
        free(password);
        exit(EXIT_FAILURE);
    }

    *(password+i) = '\0';  // Null-terminate the password

    printf("\n");  // Move to the next line after password entry

    return password;
}

char* get_password(){
    printf("Enter password: ");
    char* password = (char*)malloc((MAX_PASSWORD_LENGTH + 1) * sizeof(char)); // Allocate memory for the password
    char ch;
    int i = 0;
    while ((ch = getchar()) != '\n') 
    {
        password[i] = ch;
        i++;
        if (ch == 8 && i > 0) {  // Backspace (ASCII backspace==8)
            printf("\b \b");  // Move cursor back, print space, move cursor back again
            i--;
        }
    }
    return password;
}





#endif