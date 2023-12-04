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

/*Prototype of functions*/

int push_client_to_file(Client client);
void accept_account_creation_requests();
void free_request_file();
int find_and_update_client();
int admin_login_page();
void admin_main_page();

//int check_account_creation_requests();

/*Creation of functions*/

// append the client in the clients file
int push_client_to_file(Client client){
    FILE *client_file = fopen(PATH_CLIENT_BIN_FILE, "ab");
    if (client_file == NULL) 
    {
        perror("Error opening file while saving the client");
        fclose(client_file);
        return 0;
    }
    else
    { 
        fseek(client_file, client_file_length()*sizeof(Client), SEEK_SET);
        fwrite(&client,sizeof(Client),1,client_file);
        fclose(client_file);
        return 1;
    }
}

//create and push client to clients file
int create_push_client_for_admin() {
    system("clear");
    printf("******************     Create an account      ******************\n");
    Client *client=(Client*)malloc(sizeof(Client));
    printf("First Name       :    ");
    fgets_no_newline_return(client->first_name,FIRST_NAME_LENGHT);
    printf("Last Name        :    ");
    fgets_no_newline_return(client->last_name,LAST_NAME_LENGHT);
    printf("CIN              :    ");
    fgets_no_newline_return(client->CIN, MAX_CIN_LENGHT);

    //verifiying whether another account already exist with this CIN or not.
    if (get_client_by_cin(client->CIN) != NULL)
    {
        puts("This cin already exists.");
        return 0;
    }
    printf("Email            :   ");
    fgets_no_newline_return(client->email, MAX_EMAIL_LENGHT);

    //verifiying whether another account already exist with this email or not.
    if (get_client_by_email(client->email)!= NULL)
    {
        puts("This email already exists.");
        return 0;
    }
    strcpy(client->password,create_num_password());
    client->balance=0;
    system("clear");
    display_client_profile(*client);
    printf("\nAccount informations saved, are you sure you want to create this account! [y/n]  :  ");
    char answer;
    answer=getchar();
    while ((answer != 'y')&&(answer != 'n')&&(answer != 'Y')&&(answer != 'N'))
    {
        system("clear");
        printf("Unexpected answer, are you sure you want to create this account! [y/n]  :  ");
        answer=getchar();
        system("clear");
    }
    if ((answer == 'y')||(answer == 'Y'))
    {
        printf("The client is well saved.");
        push_client_to_file(*client);
    }
    else
    {
        printf("Client creation canceled.");
        return 0;
    }
    free(client);
    return 1;
}

//accept or decline creation requests one by one and free the staging file
void accept_account_creation_requests(){
    FILE* staging_file = fopen(PATH_STAGING_CLIENT_BIN_FILE, "rb");
    if (staging_file == NULL) {
        printf("Error opening requests file");
        exit(EXIT_FAILURE);
    }

    FILE* client_file = fopen(PATH_CLIENT_BIN_FILE, "ab");
    if (client_file == NULL) {
        printf("Error opening accounts file");
        exit(EXIT_FAILURE);
    }

    Client* client = (Client*)malloc(sizeof(Client));

    // Process each client in the requests file
    if (staging_file_length()==0)
    {
        printf("No pending request.");
    }
    else
    {
        Client *client = (Client*)malloc(sizeof(Client));
        for (int i = 1; i <= staging_file_length(); i++) 
        {
            fread(client, sizeof(Client), 1, staging_file);

            // Check if the client is not already in the accounts file
            if (get_client_by_cin(client->CIN) == NULL) 
            {
                client->account_number = client_file_length()+10000;
                display_client_profile(*client);
                puts("1. Accept request.\n");
                puts("2. Decline request.\n");
                puts("Answer   :   ");
                char choice; 
                while(((choice = getchar()) != '1') && (choice != '2'))
                {
                    display_client_profile(*client);
                    puts("1. Accept request.\n");
                    puts("2. Decline request.\n");
                    puts("Answer   :   ");
                }
                if (choice == '1')
                {
                    fwrite(client, sizeof(Client), 1, client_file);
                    printf("%s %s request has been approved.\n",client->first_name, client->last_name);
                    getchar();
                }
                else
                {
                    printf("%s %s request has been declined.\n", client->first_name, client->last_name); 
                    getchar();
                }
            } 
            else 
            {
                printf("\nThe client %s %s already exists in the accounts\n", client->first_name, client->last_name);
                getchar();
            }
        }
    }
    
    
    free_request_file();
    fclose(staging_file);
    fclose(client_file);
    free(client);
}

//clear the staging file
void free_request_file() {
    FILE* staging_file = fopen(PATH_STAGING_CLIENT_BIN_FILE, "wb");

    if (staging_file == NULL) 
    {
        printf("Error opening file");
    }
    // Close the file after clearing its content
    fclose(staging_file);
}

// returns 1 is the client is updated else it returns 0
int find_and_update_client()
{
    int choice;
    system("clear");
    printf("how do you want to search for the client that you want to update.\n\n");
    printf("1. Account number\n");
    printf("2. CIN\n");
    printf("3. Email\n");
    printf("4. Return");
    scanf("%d",&choice);
    Client user;
    switch (choice)
    {
    case 1:
        system("clear");
        int account_number;
        printf("Enter the client account number  :   ");
        scanf("%d",&account_number);
        user = *get_client_by_account(account_number);
        break;
    case 2:
        system("clear");
        char *cin=(char*)malloc(MAX_CIN_LENGHT);
        printf("Enter the client CIN    :  ");
        scanf("%s",cin);
        user = *get_client_by_cin(cin);
        break;
    case 3:
        system("clear");
        char* email=(char*)malloc(MAX_EMAIL_LENGHT);
        printf("Enter the client Email  :   ");
        scanf("%s",email);
        user = *get_client_by_email(email);
        break;
    case 4:
        return 0;
    default:
        printf("Unexpected answer! Retry");
        getchar();
        find_and_update_client();
        break;
    }
    system("clear");
    display_client_profile(user);
    getchar();
    system("clear");
    printf("******************     Get the update     ******************\n");
    Client *update=(Client*)malloc(sizeof(Client));
    printf("First Name       :    ");
    fgets_no_newline_return(update->first_name,FIRST_NAME_LENGHT);
    printf("Last Name        :    ");
    fgets_no_newline_return(update->last_name,LAST_NAME_LENGHT);
    printf("CIN              :    ");
    fgets_no_newline_return(update->CIN, MAX_CIN_LENGHT);
    //verifiying whether another account already exist with this CIN or not.
    if ((get_client_by_cin(update->CIN) != NULL) && (get_client_by_cin(update->CIN)))
    {
        puts("This cin already exists.");
        return 0;
    }
    printf("Email            :   ");
    fgets_no_newline_return(update->email, MAX_EMAIL_LENGHT);

    //verifiying whether another account already exist with this email or not.
    if (get_client_by_email(update->email)!= NULL)
    {
        puts("This email already exists.");
        return 0;
    }
    strcpy(update->password,create_num_password());
    update->balance=0;
    system("clear");
    display_client_profile(*update);
    printf("\nAccount informations saved, are you sure you want to update this account! [y/n]  :  ");
    char answer;
    answer=getchar();
    getchar();
    while ((answer != 'y')&&(answer != 'n')&&(answer != 'Y')&&(answer != 'N'))
    {
        system("clear");
        printf("Unexpected answer, are you sure you want to update this account! [y/n]  :  ");
        answer=getchar();
        system("clear");
    }
    if ((answer == 'y')||(answer == 'Y'))
    {
        printf("The client is well updated.");
        update_client_in_file(user,*update);
    }
    else
    {
        printf("The update is canceled.");
    }
    return 0;
}

int admin_login(){
    system("clear");
    printf("Admin\n\n");
    getchar();
    printf("***************** Authentification *****************\n\n");
    char *username=malloc(10),*password=malloc(MAX_PASSWORD_LENGTH);
    printf("Username      :   ");
    scanf("%s",username);
    getchar();
    printf("Password      :   ");
    disableEcho();
    fgets_no_newline_return(password,MAX_PASSWORD_LENGTH);
    if((!strcmp(username,"admin")) && (!strcmp(password,"admin")))
    { 
        return 1;
    }
    return 0;
}

void admin_main_page()
{
    system("clear");
    int choice;
    printf("\n************* Welcome, to admin space! ***************\n\n\n");
    printf("1. Account creation requests validation.\n");
    printf("2. Update a client.\n");
    printf("3. Create a client.\n");
    printf("4. Log out\n\n");
    printf("Enter the number of the desired operation  :  ");
    scanf("%d", &choice);
    switch (choice) {
        case 1:  // Account creation requests validation
            system("clear");
            accept_account_creation_requests();
            getchar();
            system("clear");
            break;

        case 2:// update client
            system("clear"); 
            find_and_update_client();
            getchar();
            system("clear");
            break;
        case 3:// Create client
            system("clear"); 
            create_client_for_admin();
            getchar();
            system("clear");
            break;
        case 4:// Quit page
            system("clear");
            printf("Thank you! Goodbye.\n");
            getchar();
            return;
        default:
            printf("Invalid choice. Please enter a valid number.\n");
            break;
    }
    admin_main_page();
}



#endif