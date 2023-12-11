#ifndef ADMIN_FUNCTIONS_H
#define ADMIN_FUNCTIONS_H

#include "struct.h"
#include "set_const.h"
#include "client_functions.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "side_functions.h"
#include "graphic.h"
/*Prototype of functions*/

void accept_account_creation_requests();
void free_request_file();
Client* find_client_option();
int update_account(int account_number);
int admin_login();
void admin_main_page();

//int check_account_creation_requests();

/*Creation of functions*/

//accept or decline creation requests one by one and free the staging file
void accept_account_creation_requests(){
    FILE* staging_file = fopen(PATH_STAGING_CLIENT_BIN_FILE, "rb");
    if (staging_file == NULL) {
        printf("Error opening requests file");
        getch();
        system("cls");
        printf("Shutting down.");
        exit(EXIT_FAILURE);
    }

    Client* client = (Client*)malloc(sizeof(Client));

    // Process each client in the requests file
    if (staging_file_length()==0)
    {
        yellow();
        printf("\n\n\n\t\tNo pending request.");
        color_reset();
        getch();
    }
    else
    {
        Client *client = (Client*)malloc(sizeof(Client));
        for (int i = 1; i <= staging_file_length(); i++) 
        {
            fread(client, sizeof(Client), 1, staging_file);
            FILE* client_file = fopen(PATH_CLIENT_BIN_FILE, "ab");
            if (client_file == NULL) {
                printf("Error opening accounts file");
                getch();
                system("cls");
                printf("Shutting down.");
                exit(EXIT_FAILURE);
            }
            // Check if the client is not already in the accounts file
            if (get_client_by_cin(client->CIN) == NULL) 
            {
                char choice; 
                client->account_number = client->city_id * 10000000 + client_file_length();
                display_client_profile(client);
                puts("\n\n1. Accept \t\t 2. Decline");
                while(((choice = getch()) != '1') && (choice != '2'))
                {
                    system("cls");
                    display_client_profile(client);
                    puts("1. Accept \t\t 2. Decline");
                }
                system("cls");
                switch (choice)
                {
                case '1':
                    fwrite(client, sizeof(Client), 1, client_file);
                    printf("%s %s request has been approved.\n",client->first_name, client->last_name);
                    getch();
                    break;
                case '2':
                    printf("%s %s request has been declined.\n", client->first_name, client->last_name); 
                    getch();
                    break;

                default:
                    break;
                }
            } 
            else 
            {
                blue();
                printf("\nThe client %s %s already exists.\n", client->first_name, client->last_name);
                color_reset();
                getch();
            }
            system("cls");
            fclose(client_file);
        }
    }
    system("cls");
    free_request_file();
    fclose(staging_file);
    free(client);
}

//clear the staging file
void free_request_file() {
    FILE* staging_file = fopen(PATH_STAGING_CLIENT_BIN_FILE, "wb");

    if (staging_file == NULL) 
    {
        printf("Error opening file");
        getch();
        system("cls");
        printf("Shutting down.");
        exit(EXIT_FAILURE);
    }
    // Close the file after clearing its content
    fclose(staging_file);
}

Client* find_client_option()
{
    while (1)
    {
        char choice;
        int account_number;
        char *cin=(char*)malloc(MAX_CIN_LENGHT);
        char* email=(char*)malloc(MAX_EMAIL_LENGHT);
        system("cls");
        printf("******************* Find Client ********************\n\n");
        printf("Search options :\n\n");
        printf("\t\t1. Account number\n");
        printf("\t\t2. CIN\n");
        printf("\t\t3. Email\n\n");
        printf("0. Return");
        choice = getch();
        system("cls");
        switch (choice)
        {
            case '1':
                printf("Enter the client account number  :   ");
                scanf("%d",&account_number);
                getchar();
                system("cls");
                display_client_profile(get_client_by_account(account_number));
                getch();
                break;

            case '2':        
                printf("Enter the client CIN    :  ");
                scanf("%s",cin);
                getchar();
                system("cls");
                display_client_profile(get_client_by_cin(cin));
                getch();
                free(cin);
                break;

            case '3':
                printf("Enter the client Email  :   ");
                scanf("%s",email);
                getchar();
                system("cls");
                display_client_profile(get_client_by_email(email));
                free(email);
                getch();
                break;

            case '0':
                system("cls");
                break;

            default:
                yellow();
                printf("\nUnexpected answer! Retry.");
                color_reset();
                getch();
                break;
        }
    }
    system("cls");
}

// returns 1 is the client is updated else it returns 0
int update_account(int account_number)
{
    Client* user = (Client*)malloc(sizeof(Client));
    user = get_client_by_account(account_number);
    system("clear");
    if(user == NULL)
    { 
        blue();
        printf("Client not found.\n");
        color_reset();
        getch();
    }
    else
    { 
        printf("******************     Update Account     ******************\n");
        Client* update=(Client*)malloc(sizeof(Client));
        printf("First Name       :    ");
        fgets_no_newline_return(update->first_name,FIRST_NAME_LENGHT);
        printf("Last Name        :    ");
        fgets_no_newline_return(update->last_name,LAST_NAME_LENGHT);
        printf("CIN              :    ");
        fgets_no_newline_return(update->CIN, MAX_CIN_LENGHT);
        printf("Email            :   ");
        fgets_no_newline_return(update->email, MAX_EMAIL_LENGHT);
        strcpy(update->password,create_num_password());
        system("cls");
        display_client_profile(update);
        blue();
        printf("\nAccount informations saved, are you sure you want to update this account! [y/n]  :  ");
        color_reset();
        char answer;
        answer=getch();
        while ((answer != 'y')&&(answer != 'n')&&(answer != 'Y')&&(answer != 'N'))
        {
            system("cls");
            blue();
            printf("Unexpected answer, are you sure you want to update this account! [y/n]  :  ");
            color_reset();
            answer=getch();
            system("cls");
        }
        if ((answer == 'y')||(answer == 'Y'))
        {
            yellow();
            update_client_in_file(*user,*update) ? printf("The client is well updated."):printf("The client is not updated.");
            color_reset();
            getch();
        }
        else
        {
            yellow();
            printf("The update is canceled.");
            color_reset();
            getch();
        }
    }
    return 0;
}
// returns 1 is the client is updated else it returns 0
int find_and_update_client(int account_number)
{
    Client* user = (Client*)malloc(sizeof(Client));
    user = get_client_by_account(account_number);
    system("cls");
    if(user == NULL)
    {
        yellow();
        printf("Client not find.");
        color_reset();
        getch();
    }
    else
    { 
        printf("******************     Update Account     ******************\n");
        Client *update=(Client*)malloc(sizeof(Client));
        printf("First Name       :    ");
        fgets_no_newline_return(update->first_name,FIRST_NAME_LENGHT);
        printf("Last Name        :    ");
        fgets_no_newline_return(update->last_name,LAST_NAME_LENGHT);
        printf("CIN              :    ");
        fgets_no_newline_return(update->CIN, MAX_CIN_LENGHT);
        printf("Email            :   ");
        fgets_no_newline_return(update->email, MAX_EMAIL_LENGHT);
        strcpy(update->password,create_num_password());
        system("cls");
        display_client_profile(update);
        blue();
        printf("\nAccount informations saved, are you sure you want to update this account! [y/n]  :  ");
        color_reset();
        char answer;
        answer=getch();
        while ((answer != 'y')&&(answer != 'n')&&(answer != 'Y')&&(answer != 'N'))
        {
            system("cls");
            blue();
            printf("Unexpected answer, are you sure you want to update this account! [y/n]  :  ");
            color_reset();
            answer=getch();
            system("cls");
        }
        if ((answer == 'y')||(answer == 'Y'))
        {   
            yellow();
            update_client_in_file(*user,*update) ? printf("The client is well updated."):printf("The client is not updated.");
            color_reset();
            getch();
        }
        else
        {
            yellow();
            printf("The update is canceled.");
            color_reset();
            getch();
        }
    }
    return 0;
}

// Returns 1 if the admin is well logged else it returns 0
int admin_login(){
    
    system("cls");
    printf("***************** Authentification *****************\n\n\n");
    char* username=malloc(10);
    char* password=malloc(MAX_PASSWORD_LENGTH);
    printf("Username      :   ");
    scanf("%s",username);
    printf("\n\nPassword      :   ");
    strcpy(password,get_password());
    if((!strcmp(username,"admin")) && (!strcmp(password,"admin")))
    {
        return 1;
    }
    return 0;
}

// Admin options
void admin_main_page()
{
    while(1)
    {   
        system("cls");
        char choice;
        int account_number;
        printf("\n************* Admin space ***************\n\n");
        printf("1. Account requests.\n");
        printf("2. Display a client.\n");
        printf("3. Update a client.\n");
        printf("4. Log out");
        choice = getch();
        system("cls");
        switch (choice) 
        {
            case '1':  // Account creation requests validation
                accept_account_creation_requests();
                break;

            case '2':// Display client
                system("cls"); 
                find_client_option();
                break;

            case '3':// Create client
                system("cls"); 
                printf("Enter the account number  :   ");
                scanf("%d",&account_number);
                getch();
                update_account(account_number);
                break;

            case '4':// Quit page
                system("cls");
                printf("Thank you! Goodbye.\n");
                getch();
                return;

            default:
                yellow();
                printf("Invalid choice. Please enter a valid number.\n");
                color_reset();
                getch();
                break;
        }
    }
}

#endif