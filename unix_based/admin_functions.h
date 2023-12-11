#ifndef ADMIN_FUNCTIONS_H
#define ADMIN_FUNCTIONS_H

#include "struct.h"
#include "set_const.h"
#include "client_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "side_functions.h"
#include "graphic.h"

/*Prototype of functions*/

void accept_account_creation_requests();
void free_request_file();
void find_client_option();
int update_account(int account_number);
int admin_login();
void admin_main_page();

/*Creation of functions*/

//accept or decline creation requests one by one and free the staging file
void accept_account_creation_requests(){
    
    FILE* staging_file = fopen(PATH_STAGING_CLIENT_BIN_FILE, "rb");
    if (staging_file == NULL) 
    {
        printf("Error opening requests file");
        unix_getch();
        system("clear");
        shut_down();
        exit(EXIT_FAILURE);
    }

    Client* client = (Client*)malloc(sizeof(Client));
    
    // Process each client in the requests file
    if (staging_file_length()==0)
    {
        printf("************** Account Creation Requests **************\n\n");
        yellow();
        printf("No pending request.");
        color_reset();
        unix_getch();
    }
    else
    {
        Client *client = (Client*)malloc(sizeof(Client));
        for (int i = 1; i <= staging_file_length(); i++) 
        {
            printf("************** Account Creation Requests **************\n\n");
            fread(client, sizeof(Client), 1, staging_file);
            FILE* client_file = fopen(PATH_CLIENT_BIN_FILE, "ab");
            if (client_file == NULL) {
                printf("Error opening accounts file");
                unix_getch();
                system("clear");
                shut_down();
                exit(EXIT_FAILURE);
            }
            // Check if the client is not already in the accounts file
            if (get_client_by_cin(client->CIN) == NULL) 
            {
                char choice; 
                client->account_number = client->city_id * 10000000 + client_file_length();
                display_client_profile(client);
                puts("\n\n1. Accept \t\t 2. Decline");
                while(((choice = unix_getch()) != '1') && (choice != '2'))
                {
                    system("clear");
                    display_client_profile(client);
                    puts("1. Accept \t\t 2. Decline");
                }
                system("clear");
                switch (choice)
                {
                case '1':
                    fwrite(client, sizeof(Client), 1, client_file);
                    printf("%s %s request has been approved.\n",client->first_name, client->last_name);
                    unix_getch();
                    break;
                
                case '2':
                    printf("%s %s request has been declined.\n", client->first_name, client->last_name); 
                    unix_getch();
                    break;

                default:
                    break;
                } 
            } 
            else 
            {
                printf("\n\n\t%s %s already exists.", client->first_name, client->last_name);
                unix_getch();
            }
            system("clear");
            fclose(client_file);
        }
    }
    system("clear");
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
        unix_getch();
        system("clear");
        shut_down();
        exit(EXIT_FAILURE);
    }
    // Close the file after clearing its content
    fclose(staging_file);
}

void find_client_option()
{
    while (1)
    {
        char choice;
        int account_number;
        char *cin=(char*)malloc(MAX_CIN_LENGHT);
        char* email=(char*)malloc(MAX_EMAIL_LENGHT);
        system("clear");
        printf("******************* Find Client ********************\n\n");
        printf("Search options :\n\n");
        printf("\t\t1. Account number\n");
        printf("\t\t2. CIN\n");
        printf("\t\t3. Email\n\n");
        printf("0. Return");
        choice = unix_getch();
        system("clear");
        switch (choice)
        {
            case '1':
                printf("Enter the client account number  :   ");
                scanf("%d",&account_number);
                system("clear");
                display_client_profile(get_client_by_account(account_number));
                unix_getch();
                break;

            case '2':
                printf("Enter the client CIN    :  ");
                scanf("%s",cin);
                system("clear");
                display_client_profile(get_client_by_cin(cin));
                unix_getch();
                free(cin);
                break;

            case '3':
                printf("Enter the client Email  :   ");
                scanf("%s",email);
                system("clear");
                display_client_profile(get_client_by_email(email));
                free(email);
                unix_getch();
                break;

            case '0':
                system("clear");
                return;

            default:
                printf("\nUnexpected answer! Retry.");
                unix_getch();
                break;
        }
    }
    system("clear");
}

// returns 1 is the client is updated else it returns 0
int update_account(int account_number)
{
    Client* user = (Client*)malloc(sizeof(Client));
    user = get_client_by_account(account_number);
    system("clear");
    if(user == NULL)
    { 
        printf("Client not found.\n");
        unix_getch();
    }
    else
    { 
        red();
        printf("******************     Update Account     ******************\n");
        color_reset();
        Client *update=(Client*)malloc(sizeof(Client));
        update = user;
        printf("First Name       :    ");
        fgets_no_newline_return(update->first_name,FIRST_NAME_LENGHT);
        printf("Last Name        :    ");
        fgets_no_newline_return(update->last_name,LAST_NAME_LENGHT);
        printf("CIN              :    ");
        fgets_no_newline_return(update->CIN, MAX_CIN_LENGHT);
        printf("Email            :    ");
        fgets_no_newline_return(update->email, MAX_EMAIL_LENGHT);
        strcpy(update->password,create_num_password());
        system("clear");
        display_client_profile(update);
        blue();
        printf("\nAccount informations saved, are you sure you want to update this account! [y/n]  :  ");
        color_reset();
        char answer;
        answer=unix_getch();
        while ((answer != 'y')&&(answer != 'n')&&(answer != 'Y')&&(answer != 'N'))
        {
            system("clear");
            blue();
            printf("Unexpected answer, are you sure you want to update this account! [y/n]  :  ");
            color_reset();
            answer=unix_getch();
            system("clear");
        }
        if ((answer == 'y')||(answer == 'Y'))
        {
            yellow();
            update_client_in_file(*user,*update) ? printf("The client is well updated."):printf("The client is not updated.");
            color_reset();
            unix_getch();
        }
        else
        {
            yellow();
            printf("The update is canceled.");
            color_reset();
            unix_getch();
        }
    }
    return 0;
}

// Returns 1 if the admin is well logged else it returns 0
int admin_login(){
    
    system("clear");
    printf("***************** Authentification *****************\n\n\n");
    char* username=malloc(10);
    char* password=malloc(MAX_PASSWORD_LENGTH);
    printf("Username      :   ");
    fgets_no_newline_return(username,10);
    printf("\n\nPassword      :   ");
    password=get_password();
    if((!strcmp(username,"admin")) && (!strcmp(password,"admin")))
    {
        system("clear");
        logging_in();
        system("clear");
        return 1;
    }
    return 0;
}

// Admin options
void admin_main_page()
{
    while(1)
    {   
        system("clear");
        char choice;
        int account_number;
        printf("\n***************** Admin space ******************\n\n");
        printf("\t1. Account requests.\n");
        printf("\t2. Display a client.\n");
        printf("\t3. Update a client.\n");
        printf("\t4. Log out");
        choice = unix_getch();
        system("clear");
        switch (choice) 
        {
            case '1':  // Account creation requests validation
                accept_account_creation_requests();
                break;

            case '2':  // Display client
                system("clear"); 
                find_client_option();
                break;

            case '3':  // Create client
                system("clear"); 
                printf("Enter the account number  :   ");
                scanf("%d",&account_number);
                unix_getch();
                update_account(account_number);
                break;

            case '4':// Quit page
                system("clear");
                printf("Thank you! Goodbye.");
                sleep(1);
                return;

            default:
                yellow();
                printf("Invalid choice. Please enter a valid number.");
                color_reset();
                unix_getch();
                break;
        }
    }
}

#endif