#ifndef CLIENT_FUNCTIONS_H
#define CLIENT_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "struct.h"
#include "set_const.h"
#include "side_functions.h"
#include "graphic.h"
//#include <openssl/sha.h>

/*prototyping functions*/
int request_client_account_creation();
Client* client_authentification();
int forgot_password();
int make_transfer(Client *sender);
int make_deposit(Client *client);
int make_withdrawal(Client *client);
void client_login_page();
void client_main_page(Client* client);


/*Declaring the functions*/

// Returns 1 if the client is well created and saved into the staging file and 0 if it encounter a problem.
int request_client_account_creation() {
    system("clear");
    printf("******************     Create an account      ******************\n\n");
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
        blue();
        puts("\nThis cin already exists. Account creation");
        color_reset();
        unix_getch();
        return 0;
    }
    printf("Email            :    ");
    fgets_no_newline_return(client->email, MAX_EMAIL_LENGHT);

    //verifiying whether another account already exist with this email or not.
    if (get_client_by_email(client->email)!= NULL)
    {
        blue();
        puts("\nThis email already exists.");
        color_reset();
        unix_getch();
        return 0;
    }
    strcpy(client->password,create_num_password());
    strcpy(client->security.question,get_security_question());
    printf("%s   :  ",client->security.question);
    fgets_no_newline_return(client->security.answer,SEC_ANSWER_LENGHT);
    client->city_id = get_city_id();
    client->balance=0;
    system("clear");
    printf("******************     Profile      *******************\n\n");
    printf("Account Holder   :  %s %s\n",client->first_name,client->last_name);
    printf("Account CIN      :  %s\n",client->CIN);
    printf("Account E-mail   :  %s",client->email);
    printf("\n\nAccount informations saved, are you sure you want to create this account! [y/n]  :  ");
    char answer;
    answer = unix_getch();
    while ((answer != 'y')&&(answer != 'n')&&(answer != 'Y')&&(answer != 'N'))
    {
        system("clear");
        blue();
        printf("Unexpected answer, are you sure you want to create this account! [y/n]  :  ");
        color_reset();
        answer = unix_getch();
    }
    system("clear");
    if ((answer == 'y')||(answer == 'Y'))
    {
        push_to_request_file(*client);
        return 1;
    }
    else
    {
        blue();
        printf("Request canceled.");
        color_reset();
        unix_getch();
        system("clear");
    }
    free(client);
    system("clear");
    return 0;
}

// Returns the logged client else if the authentification encounters a problem it returns a NULL.
Client* client_authentification() {
    system("clear");
    printf("\t\t******************** Authentification **********************\n\n\n");
    int account_number;
    char password[MAX_PASSWORD_LENGTH+1];
    printf("Account Number    :   ");
    scanf("%d",&account_number);
    unix_getch();
    printf("\nPassword          :   ");
    disableEcho();
    fgets_no_newline_return(password,MAX_PASSWORD_LENGTH);
    enableEcho();
    system("clear");
    // Ouverture du fichier binaire des clients.
    FILE* client_file = fopen(PATH_CLIENT_BIN_FILE, "rb");
    if (client_file == NULL) 
    {
        perror("Error opening clients file while login");
        unix_getch();
        system("clear");
        printf("Shutting down . . .\n");
        exit(EXIT_FAILURE);
    }
    else
    { 
        for (int i = 0; i < client_file_length(); i++) 
        {
            Client* user=(Client*)malloc(sizeof(Client));
            fread(user, sizeof(Client), 1,client_file);
            if (account_number == user->account_number) 
            { 
                if ((strcmp(password, user->password) == 0))
                {
                    blue();
                    logging_in();
                    color_reset();
                    fclose(client_file);
                    return user;
                }
                else
                {
                    blue();
                    printf("Invalid Password\n");
                    color_reset();
                    unix_getch();
                }
            }
        }
    }
    fclose(client_file);
    return NULL;
}

// Password reset function returns 1 if password is well updated else it returns 0
int forgot_password() {
    FILE* client_file = fopen(PATH_CLIENT_BIN_FILE, "rb");
    if (client_file == NULL) 
    {
        printf("Error opening file while restoring password.");
        unix_getch();
        system("clear");
        shut_down();
        exit(EXIT_FAILURE);
    }

    int account_number;
    Client* client=malloc(sizeof(Client));
    system("clear");
    printf("\n***************** Forgot Password ******************\n\n\n");
    printf("Account number      :    ");
    scanf("%d",&account_number);
    client=get_client_by_account(account_number);

    for (int i = 0; i < client_file_length(); i++) {
        fread(client, sizeof(Client), 1, client_file);
        if (account_number == client->account_number) 
        {
            char* answer = (char*)malloc(SEC_ANSWER_LENGHT*sizeof(char));
            printf("\n%s\n\n",client->security.question);
            unix_getch();
            printf("Answer    :  ");
            fgets_no_newline_return(answer,SEC_ANSWER_LENGHT);
            if (strcmp(answer, client->security.answer) == 0) 
            {
                // valid answer
                system("clear");
                printf("\n***************** Forgot Password ******************\n\n\n");
                char* new_password=(char*)malloc(MAX_PASSWORD_LENGTH*sizeof(char));
                strcpy(new_password,create_num_password());
                if (update_password(account_number,new_password) == 1) 
                {
                    sleep(1);
                    system("clear");
                    processing();
                    system("clear");
                    fclose(client_file);
                    return 1;
                }
            }    
        }
    }
    fclose(client_file);
    return 0;
}

// Returns 1 if the transfer is successful else it returns 0.
int make_transfer(Client *sender) {

    Client* receiver = (Client*)malloc(sizeof(Client));
    Client* update_sender=(Client*)malloc(sizeof(Client));
    Client* update_receiver=(Client*)malloc(sizeof(Client));
    double transfer_amount;
    int account_number;
    printf("****************** Transfer ********************\n\n");
    printf("Enter the destination account number  : ");
    scanf("%d", &account_number);
    unix_getch();
    while(get_client_by_account(account_number)==NULL)
    {
        system("clear");
        printf("Unsuccessful operation! The entred account does not exist!");
        unix_getch();
        system("clear");
        printf("Enter the destination account number  : ");
        scanf("%d", &account_number);
        unix_getch();
    }
    *receiver = *get_client_by_account(account_number);
    printf("Enter the transfer amount   :   ");
    scanf("%lf", &transfer_amount);
    while (transfer_amount<=0)
    {
        unix_getch();
        printf("Invalid amount.");
        getchar();
        clearLine();
        clear_nprevious_lines(2);
        printf("Enter the transfer amount   :   ");
        scanf("%lf", &transfer_amount);
    }
    if (transfer_amount<=sender->balance)
    {
        *receiver = *get_client_by_account(receiver->account_number);
        *update_sender = *sender;
        *update_receiver = *receiver;
        update_sender->balance -= transfer_amount;
        update_receiver->balance += transfer_amount;
        update_client_in_file(*sender,*update_sender);
        update_client_in_file(*receiver,*update_receiver);
        sender->balance -= transfer_amount;
        system("clear");
        processing();
        system("clear");
        blue();
        printf("Transfer of %.2f successfully completed to %s %s.",transfer_amount,receiver->first_name,receiver->last_name);
        color_reset();
        unix_getch();
        return(1);
    }
    else
    { 
        system("clear");
        blue();
        printf("Unsuccessful operation, your balance is  insufficient."); 
        color_reset();
        unix_getch();
    }
    return(0);  
}

// Returns 1 if the deposit is successfull else it returns 0.
int make_deposit(Client *client) {
    double deposit_amount;
    Client* temp=(Client*)malloc(sizeof(Client));
    *temp=*client;
    printf("****************** Deposit ********************\n\n");
    printf("Enter the deposit amount     :     ");
    scanf("%lf", &deposit_amount);
    temp->balance += deposit_amount;
    if(update_client_in_file(*client,*temp)==1)
    {
        system("clear");
        processing();
        system("clear");
        blue();
        printf("Deposit of %.2f successfully completed.\n", deposit_amount);
        color_reset();
        client->balance += deposit_amount;
        free(temp);
        unix_getch();
        return 1;
    }
    free(temp);
    return 0;
}

// Returns 1 if the withdrawal is successfull else it returns 0.
int make_withdrawal(Client *client) {
    double withdrawal_amount;
    Client* temp=(Client*)malloc(sizeof(Client));
    *temp=*client;
    printf("****************** Withdrawal ********************\n\n");
    printf("Enter the withdrawal amount  :     ");
    scanf("%lf", &withdrawal_amount);
    if (withdrawal_amount > client->balance) 
    {
        system("clear");
        blue();
        printf("Unsuccessful operation, your balance is  insufficient.\n");
        color_reset();
        unix_getch();
    }
    else
    {
        temp->balance -= withdrawal_amount;
        if(update_client_in_file(*client,*temp)==1)
        {
            system("clear");
            processing();
            system("clear");
            blue();
            printf("Withdrawal of %.2f successfully completed.\n", withdrawal_amount);
            color_reset();
            client->balance -= withdrawal_amount;
            free(temp);
            unix_getch();
            return 1;
        }
    }
    free(temp);
    return 0;
}

void check_account_creation_status(){
    char* cin = (char*)malloc(sizeof(char)*MAX_CIN_LENGHT);
    char* password = (char*)malloc(sizeof(char)*MAX_PASSWORD_LENGTH);
    printf("***************** Account Creation Status *****************\n\n\n");
    printf("CIN             :     ");
    fgets_no_newline_return(cin,MAX_CIN_LENGHT);
    printf("\nPassword        :      ");
    disableEcho();
    fgets_no_newline_return(password,MAX_PASSWORD_LENGTH);
    enableEcho();
    system("clear");
    Client* user=(Client*)malloc(sizeof(Client));
    FILE* staging_file = fopen(PATH_STAGING_CLIENT_BIN_FILE, "rb");
    if (staging_file == NULL) 
    {
        printf("Error opening clients file while getting the client by cin.");
        unix_getch();
        system("clear");
        shut_down();
        exit(EXIT_FAILURE);
    }
    else
    { 
        if (get_client_by_cin(cin) != NULL)
        {
            blue();
            printf("Your request is realised! Press any key to display your profile.");
            color_reset();
            unix_getch();
            system("clear");
            display_client_profile(get_client_by_cin(cin));
            unix_getch();
            system("clear");
            return;
        }
        else
        {
            for(int i=0 ; i < staging_file_length() ; i++)
            {                 
                user = (Client*)malloc(sizeof(Client));
                fread(user,sizeof(Client),1,staging_file);
                if(!strcmp(user->CIN,cin))
                { 
                    fclose(staging_file);
                    break;
                } 
                user = NULL;
            } 
        }         
    }
    fclose(staging_file);

    if (user != NULL)
    {
        blue();
        printf("Your request is still in process! Visit us later.");
        color_reset(); 
    }
    else
    {
        blue();
        printf("You have not submitted any request, please try to sign up.");  
        color_reset();
    }
    unix_getch();
    system("clear");
}

void client_login_page(){
    while(1)
    {
        system("clear");
        Client* user=(Client*)malloc(sizeof(Client));
        char choice;
        printf("\n****************** Client Space *******************\n\n\n");
        printf("\t1. Log in\n");
        printf("\t2. Sign up\n");
        printf("\t3. Request status\n");
        printf("\t4. Forgot password\n");
        printf("\t5. Quit");
        choice=unix_getch();
        system("clear");
        switch (choice) 
        {
            case '1':  //log in
                    system("clear");
                    user=client_authentification();
                    if(user == NULL)
                    {   
                        printf("\n\n\n");
                        blue();
                        printf("Invalid Account_number or password ! Retry.\n");
                        color_reset();
                        unix_getch();
                    }
                    else
                    {
                        client_main_page(user); 
                    }
                break;        

            case '2':// Sign up
                request_client_account_creation();
                break;

            case '3':// Check creation account status.
                check_account_creation_status();
                break;

            case '4':// Forgot password
                if (forgot_password()==1)
                {
                    blue();
                    printf("\nThe password has been succesfully changed.");
                    color_reset();
                }
                else
                {
                    blue();
                    printf("\nInvalid answer! Password reset canceled.");
                    color_reset();
                } 
                unix_getch();
                break;

            case '5':// Quit page
                system("clear");
                yellow();
                printf("\n\n\tThank you! Goodbye.");
                color_reset();
                fflush(stdout);
                sleep(2);
                return;

            default:
                printf("\n\n\tInvalid choice. Please enter a valid number.");
                unix_getch();
                system("clear");
                break;
        }
    }
}

void client_main_page(Client *client){
    while(1)
    {
        system("clear");
        char choice;
        printf("\n*************** Welcome, %s %s ****************\n\n", client->first_name, client->last_name);
        printf("\t1. Profile\n"); 
        printf("\t2. Deposit\n");
        printf("\t3. Withdrawal\n");
        printf("\t4. Transfer\n");
        printf("\t5. Check balance\n");
        printf("\t6. Log out");
        choice=unix_getch();
        system("clear");
        switch (choice) {
            case '1':// Profile
                display_client_profile(client);
                unix_getch();
                break;

            case '2':// Deposit
                make_deposit(client);
                unix_getch();
                break;

            case '3':// Withdrawal
                make_withdrawal(client);
                unix_getch();
                break;

            case '4':// Transfer
                make_transfer(client);
                unix_getch();
                break;

            case '5':// Balance inquiry
                printf("****************** Balance ******************\n\n");
                printf("Account Number       :   %d\n",client->account_number);
                printf("Account Holder       :   %s %s\n",client->first_name,client->last_name);
                printf("Your account balance :   %.2f\n", client->balance);
                unix_getch();
                break;

            case '6':// Log out
                yellow();
                logging_out();
                color_reset();
                system("clear");
                return;

            default:
                yellow();
                printf("Invalid choice. Please enter a valid number.\n");
                color_reset();
                unix_getch();
                break;
        }
    }  
}

#endif
