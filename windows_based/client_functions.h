#ifndef CLIENT_FUNCTIONS_H
#define CLIENT_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include<windows.h>
#include "struct.h"
#include "set_const.h"
#include "side_functions.h"
#include "graphic.h"

/*prototyping functions*/

int request_client_account_creation();
Client* client_authentification();
int forgot_password();
int make_transfer(Client *sender);
int make_deposit(Client *client);
int make_withdrawal(Client *client);
void check_account_creation_status();
void client_login_page();
void client_main_page(Client* client);

/*Declaring the functions*/

// Returns 1 if the client is well created and saved into the staging file and 0 if it encounter a problem.
int request_client_account_creation() {
    system("cls");
    yellow();
    printf("********************** Account Creation ***********************\n\n");
    color_reset();
    Client *client=(Client*)malloc(sizeof(Client));
    printf("  First Name       :    ");
    fgets_no_newline_return(client->first_name,FIRST_NAME_LENGHT);
    printf("  Last Name        :    ");
    fgets_no_newline_return(client->last_name,LAST_NAME_LENGHT);
    printf("  CIN              :    ");
    fgets_no_newline_return(client->CIN, MAX_CIN_LENGHT);

    //verifiying whether another account already exist with this CIN or not.
    if (get_client_by_cin(client->CIN) != NULL)
    {
        blue();
        puts("\nThis CIN already exists.");
        color_reset();
        getch();
        return 0;
    }
    printf("  Email            :   ");
    fgets_no_newline_return(client->email, MAX_EMAIL_LENGHT);

    //verifiying whether another account already exist with this email or not.
    if (get_client_by_email(client->email)!= NULL)
    {
        blue();
        puts("\this email already exists.");
        color_reset();
        getch();
        return 0;
    }
    strcpy(client->password,create_num_password());
    strcpy(client->security.question,get_security_question());
    printf("%s   :  ",client->security.question);
    fgets_no_newline_return(client->security.answer,SEC_ANSWER_LENGHT);
    client->city_id = get_city_id();
    client->balance=0;
    client->account_status = 1;
    system("cls");
    yellow();
    printf("*********************** Account Creation ************************\n\n");
    color_reset();
    printf("              Account Holder   :  %s %s\n\n",client->first_name,client->last_name);
    printf("              Account CIN      :  %s\n\n",client->CIN);
    printf("              Account E-mail   :  %s\n\n",client->email);
    yellow();
    printf("******************************************************************\n\n\n");
    green();
    printf("\n\nAccount informations saved, are you sure you want to create this account! [y/n]  :  ");
    color_reset();
    char answer;
    answer = getch();
    while ((answer != 'y')&&(answer != 'n')&&(answer != 'Y')&&(answer != 'N'))
    {
        system("cls");
        blue();
        printf("Unexpected answer, are you sure you want to create this account! [y/n]  :  ");
        color_reset();
        answer = getch();
    }
    system("cls");
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
        getch();
        system("cls");
    }
    free(client);
    system("cls");
    return 0;
}

// Returns the logged client else if the authentification encounters a problem it returns a NULL.
Client* client_authentification() {
    system("cls");
    yellow();
    printf("******************** Authentification **********************\n\n\n");
    color_reset();
    int account_number;
    char password[MAX_PASSWORD_LENGTH];
    printf("   Account Number    :   ");
    scanf("%d",&account_number);
    printf("\n   Password          :   ");
    strcpy(password,get_password());
    system("cls");
    FILE* client_file = fopen(PATH_CLIENT_BIN_FILE, "rb");
    if (client_file == NULL) 
    {
        perror("Error opening clients file while login");
        getch();
        system("cls");
        shut_down();
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
                    if (user->account_status != 0)
                    {
                        blue();
                        logging_in();
                        color_reset();
                        fclose(client_file);
                        return user;  
                    }
                    else
                    {
                        yellow();
                        printf("This account is closed. Contact admin for help.");
                        color_reset();
                        getch();
                        fclose(client_file);
                        return NULL;
                    }
                }
                else
                {
                    red();
                    printf("Invalid Password\n");
                    color_reset();
                    getch();
                    fclose(client_file);
                    return NULL;
                }
            }
        }
        blue();
        printf("Account number does not exist");
        color_reset();
        getch();
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
        getch();
        system("cls");
        printf("Shutting down.");
        exit(EXIT_FAILURE);
    }

    int account_number;
    Client* client=malloc(sizeof(Client));
    system("cls");
    yellow();
    printf("\n******************* Forgot Password ********************\n\n\n");
    color_reset();
    printf("         Account number      :    ");
    scanf("%d",&account_number);
    client=get_client_by_account(account_number);
    char* answer = (char*)malloc(SEC_ANSWER_LENGHT*sizeof(char));
    for (int i = 0; i < client_file_length(); i++) {
        fread(client, sizeof(Client), 1, client_file);
        if (account_number == client->account_number) 
        {
            printf("\n%s\n",client->security.question);
            printf("   Answer    :  ");
            fflush(stdout);
            int c;
            while ((c=getchar()!='\n') && c!=EOF);
            fgets_no_newline_return(answer,SEC_ANSWER_LENGHT);
            if (strcmp(answer, client->security.answer) == 0) 
            {
                // Valid answer
                system("cls");
                yellow();
                printf("\n***************** Reset Password ******************\n\n\n");
                color_reset();
                char* new_password=(char*)malloc(MAX_PASSWORD_LENGTH*sizeof(char));
                strcpy(new_password,create_num_password());
                if (update_password(account_number,new_password) == 1) 
                {
                    Sleep(1000);
                    system("cls");
                    processing();
                    system("cls");
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
    yellow();
    printf("\n****************** Transfer ********************\n\n");
    color_reset();
    printf("   Enter the destination account number  : ");
    scanf("%d", &account_number);
    getch();
    while(get_client_by_account(account_number)==NULL)
    {
        system("cls");
        blue();
        printf("Unsuccessful operation! The entred account does not exist!");
        color_reset();
        getch();
        system("cls");
        yellow();
        printf("\n****************** Transfer ********************\n\n\n");
        color_reset();
        printf("   Enter the destination account number  : ");
        scanf("%d", &account_number);
        getch();
    }
    *receiver = *get_client_by_account(account_number);
    printf("\n   Enter the transfer amount   :   ");
    scanf("%lf", &transfer_amount);
    while (transfer_amount<=0)
    {
        getch();
        clear_nprevious_lines(2);
        red();
        printf("Invalid amount.");
        color_reset();
        getchar();
        clearLine();
        clear_nprevious_lines(2);
        printf("   Enter the transfer amount   :   ");
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
        system("cls");
        processing ();
        system("cls");
        blue();
        printf("Transfer of %.2f $ successfully completed to %s %s.",transfer_amount,receiver->first_name,receiver->last_name);
        color_reset();
        getch();
        return(1);
    }
    else
    { 
        system("cls");
        red();
        printf("Unsuccessful operation, your balance is  insufficient."); 
        color_reset();
        getch();
    }
    return(0);   
}

// Returns 1 if the deposit is successfull else it returns 0.
int make_deposit(Client *client) {
    double deposit_amount;
    yellow();
    printf("******************** Deposit ********************\n\n");
    color_reset();
    printf("   Enter the deposit amount     :     ");
    scanf("%lf", &deposit_amount);
    if (deposit_amount <= 0)
    {
        clear_nprevious_lines(2);
        red();
        printf("\n Invalid deposit amount.\n");
        color_reset();
        getch();
        return 0;
    }
    Client* temp=(Client*)malloc(sizeof(Client));
    *temp=*client;
    temp->balance += deposit_amount;
    if(update_client_in_file(*client,*temp)==1)
    {
        system("cls");
        processing ();
        system("cls");
        blue();
        printf("Deposit of %.2f $ successfully completed.\n", deposit_amount);
        color_reset();
        client->balance += deposit_amount;
        free(temp);
        getch();
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
    yellow();
    printf("\n******************* Withdrawal ********************\n\n");
    color_reset();
    printf("   Enter the withdrawal amount  :     ");
    scanf("%lf", &withdrawal_amount);
    if (withdrawal_amount > client->balance) 
    {
        system("cls");
        red();
        printf("Unsuccessful operation, your balance is  insufficient.\n");
        color_reset();
        getch();
    }
    else
    {
        if (withdrawal_amount <= 0)
        {
            clear_nprevious_lines(2);
            red();
            printf("\n Invalid Withdrawal amount.\n");
            color_reset();
            getch();
            return 0;
        }
        temp->balance -= withdrawal_amount;
        if(update_client_in_file(*client,*temp)==1)
        {
            system("cls");
            processing ();
            system("cls");
            blue();
            printf("Withdrawal of %.2f $ successfully completed.\n", withdrawal_amount);
            color_reset();
            client->balance -= withdrawal_amount;
            free(temp);
            getch();
            return 1;
        }
    }
    free(temp);
    return 0;
}

void check_account_creation_status(){
    char* cin = (char*)malloc(sizeof(char)*MAX_CIN_LENGHT);
    char* password = (char*)malloc(sizeof(char)*MAX_PASSWORD_LENGTH);
    yellow();
    printf("\n***************** Account Creation Status *****************\n\n\n");
    color_reset();
    printf("   CIN             :    ");
    fgets_no_newline_return(cin,MAX_CIN_LENGHT);
    Client* user=(Client*)malloc(sizeof(Client));
    FILE* staging_file = fopen(PATH_STAGING_CLIENT_BIN_FILE, "rb");
    if (staging_file == NULL) 
    {
        printf("Error opening clients file while getting the client by cin.");
        getch();
        system("cls");
        shut_down();
        exit(EXIT_FAILURE);
    }
    else
    { 
        user = get_client_by_cin(cin);
        if (user != NULL)
        {
            if (user->account_status == 0)
            {
                red();
                printf("\n\nYour account is closed. Contact admin for help\n");
                color_reset();
                getch();
                return;
            }
            else
            {
                printf("\n   Password        :    ");
                strcpy(password,get_password());
                fflush(stdout);
                if (strcmp(password,user->password) == 0 )
                {
                    green();
                    printf("\n\nYour request is realised! Press any key to display your profile\n");
                    color_reset();
                    getch();
                    system("cls");
                    display_client_profile(get_client_by_cin(cin));
                    getch();
                    system("cls");
                    fclose(staging_file);
                }
                else
                {
                    red();
                    printf("\n\nInvalid Password\n");
                    color_reset();
                    getch();
                    system("cls");
                }
            }
            return;
        }
        else
        {
            system("cls");
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
    getch();
    system("cls");
}

void client_login_page(){
    while(1)
    {
        system("cls");
        Client* user=(Client*)malloc(sizeof(Client));
        char choice;
        yellow();
        printf("\n******************* Client Space ******************\n\n");
        color_reset();
        printf("\t1. Log in\n\n");
        printf("\t2. Sign up\n\n");
        printf("\t3. Request status\n\n");
        printf("\t4. Forgot password\n\n");
        printf("\t5. Quit\n\n");
        yellow();
        printf("\n***************************************************\n\n");
        color_reset();
        choice=getch();
        system("cls");
        switch (choice) 
        {
            case '1':  //log in
                    system("cls");
                    user=client_authentification();
                    if(user != NULL)
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
                    getch();
                    blue();
                    printf("\nThe password has been succesfully changed.");
                    color_reset();
                }
                else
                {
                    red();
                    printf("\nInvalid answer! Password reset canceled.");
                    color_reset();
                } 
                getch();
                break;

            case '5':// Quit page
                system("cls");
                yellow(); 
                goodbye();
                color_reset();
                fflush(stdout);
                Sleep(1000);
                return;

            default:
                printf("\n\n\tInvalid choice. Please enter a valid number.");
                color_reset();
                getch();
                system("cls");
                break;
        }
    }
}

void client_main_page(Client *client){
    while(1)
    {
        system("cls");
        yellow();
        printf("\n********************* Account Space **********************\n\n\n");
        color_reset();
        printf("\t1. Profile\n\n"); 
        printf("\t2. Deposit\n\n");
        printf("\t3. Withdrawal\n\n");
        printf("\t4. Transfer\n\n");
        printf("\t5. Check balance\n\n");
        printf("\t6. Account closure\n\n");
        printf("\t7. Log out\n\n");
        yellow();
        printf("\n***********************************************************\n\n");
        color_reset();
        char choice, choice1;
        choice = getch();
        system("cls");
        switch (choice) {
            case '1':// Profile
                display_client_profile(client);
                getch();
                break;

            case '2':// Deposit
                make_deposit(client);
                getch();
                break;

            case '3':// Withdrawal
                make_withdrawal(client);
                getch();
                break;

            case '4':// Transfer
                make_transfer(client);
                getch();
                break;

            case '5':// Balance inquiry
                yellow();
                printf("********************* Balance **********************\n\n");
                color_reset();
                printf("    Account Number       :   %d\n",client->account_number);
                printf("    Account Holder       :   %s %s\n",client->first_name,client->last_name);
                printf("    Your account balance :   %.2f $\n", client->balance);
                yellow();
                printf("\n\n*************************************************\n\n");
                color_reset();
                getch();
                break;

            case '6':// Account closure
                yellow();
                printf("************************ Account closure *************************\n\n");
                color_reset();
                red();
                printf("\n Are you sure you want to continue. This operation is critical [y/n]\n ");
                choice1 = getch();
                while (choice1 != 'y' && choice1 !='Y' && choice1 != 'n' && choice1 !='N')
                {
                    choice1 = getch();
                } 
                if (choice1 == 'Y' || choice1 == 'y')
                {
                    clear_nprevious_lines(2);
                    disable_account(client);
                    printf("\nThe account has been closed. You can no longer acces to this account\n");
                }
                else
                {
                    clear_nprevious_lines(2);
                    printf("\nOperation canceled\n");
                }
                color_reset();
                getch();
                system("clear");
                return;

            case '7':// Log out
                yellow();
                logging_out();
                color_reset();
                system("cls");
                return;

            default:
                red();
                printf("Invalid choice. Please enter a valid number.\n");
                color_reset();
                getch();
                break;
        }
    } 
}

#endif
