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
void display_client_profile();
char* create_num_password();
char* get_password();
Auth *authentification(char* email, char* password);
int login_admin();



void display_client_profile(Client client){


}

char* create_num_password() {
    char* password = (char*)malloc((MAX_PASSWORD_LENGTH + 1) * sizeof(char)); // Allocate memory for the password

    if (password == NULL) {
        printf("Password memory allocation failed\n");
        exit(1);    
    }//Assuring the memory allocation for the password                                       

    printf("Enter password: ");
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
}


void admin_or_client(){
    printf("(1)    I am a client\n\n");
    printf("(2)    I am an administrator\n");
    printf("\n");
    char ch= getchar();
    system("cls");
    switch(ch){
    case '1':
        /*client_services(); not yet added*/
        break;
    case '2':{
            system("cls");
            int i=0;
            do{
                if(!login_admin()){
                    printf("\n\n\n\n\n");
                    printf("\t\t\t\t\tInvalid E-mail or password ! retry\n");
                    getchar();
                    system("cls");
                    i++;
                }else{
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