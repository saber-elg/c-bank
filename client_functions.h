#ifndef CLIENT_FUNCTIONS_H
#define CLIENT_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "struct.h"
#include "set_const.h"
#include <time.h>
#include "side_functions.h"
//#include <openssl/sha.h>

/*prototyping functions*/

int push_to_request_file(Client client);
int request_client_account_creation();
char* create_num_password();
void display_client_profile(Client client);
char* get_password();
Client* get_client_by_cin(char* cin);
Client* get_client_by_email(char* email);
Client* get_client_by_account(int account_number);
int find_client_account_position(int account_number);
Client get_client_by_position(int pos);
int client_file_length();
Client* client_authentification();
int update_client_in_file(Client client,Client update);
int make_transfer(Client sender);
int make_deposit(Client client);
int make_withdrawal(Client client);
void client_login_page();
void client_main_page(Client client);


/*Declaring the functions*/

//returns 1 if the client is well saved in the staging file else it returns 0
int push_to_request_file(Client client){
    FILE *staging_file = fopen(PATH_STAGING_CLIENT_BIN_FILE, "ab");
    if (staging_file == NULL) 
    {
        perror("Error opening file while saving the client");
        fclose(staging_file);
        return 0;
    }
    else
    { 
        fseek(staging_file, client_file_length()*sizeof(Client), SEEK_SET);
        fwrite(&client,sizeof(Client),1,staging_file);
        fclose(staging_file);
        return 1;
    }
}

//returns 1 if the client is well created and saved into the staging file and 0 if it encounter a problem.
int request_client_account_creation() {
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
    getchar();
    while ((answer != 'y')&&(answer != 'n')&&(answer != 'Y')&&(answer != 'N'))
    {
        system("clear");
        printf("Unexpected answer, are you sure you want to create this account! [y/n]  :  ");
        answer=getchar();
        system("clear");
    }
    if ((answer == 'y')||(answer == 'Y'))
    {
        push_to_request_file(*client);
    }
    free(client);
    return 1;
}

//returns a numeric password as a string without showing thw password while typing.
char* create_num_password() {
    char* password = (char*)malloc((MAX_PASSWORD_LENGTH + 1) * sizeof(char));                                     
    printf("Enter password   :   ");
    char ch;
    int i = 0;
    disableEcho();
    while ((ch = getchar()) != '\n' && i < MAX_PASSWORD_LENGTH) 
    {
        if (isdigit(ch)) 
        {
            password[i] = ch;
            i++;
            printf("*");
        }else 
            {
                if ((ch == 8 || '9'<=ch || ch<='0') && i > 0) // Backspace (ASCII backspace==8)
                {  
                    printf("\b");   //move cursor back
                    printf(" ");    //print space
                    printf("\b");   //move cursor back again
                    i--;
                }
            }
    }
    if (i < MIN_PASSWORD_LENGTH) {
        printf("\nPassword must be at least %d digits long.\n", MIN_PASSWORD_LENGTH);
        free(password);
        return create_num_password();
    }
    *(password+i) = '\0';  // Null-terminate the password
    printf("\n"); 
    enableEcho();
    return password;
}

//displays the client related info: acount number, full name, CIN, email.
void display_client_profile(Client client){
    system("clear");
    printf("******************     Profile      *******************\n\n");
    printf("Account Number   :  %d\n\n",client.account_number);
    printf("Account Holder   :  %s %s\n\n",client.first_name,client.last_name);
    printf("Account CIN      :  %s\n\n",client.CIN);
    printf("Account E-mail   :  %s\n\n",client.email);
}

//returns the typed password from the user.
char* get_password(){
    printf("Enter password: ");
    char* password = (char*)malloc((MAX_PASSWORD_LENGTH + 1) * sizeof(char)); 
    char ch;
    int i = 0;
    while ((ch = getchar()) != '\n') 
    {
        *(password+i) = ch;
        i++;
        if (ch == 8 && i > 0) {  // Backspace (ASCII backspace==8)
            printf("\b \b");  // Move cursor back, print space, move cursor back again
            i--;
        }
    }
    return password;
}

//returns a client if a client has the corresponding cin exists else returns NULL
Client* get_client_by_cin(char* cin){
    Client* user=(Client*)malloc(sizeof(Client));
    FILE* client_file = fopen(PATH_CLIENT_BIN_FILE, "rb");
    if (client_file == NULL) 
    {
        printf("Error opening clients file while getting the client by cin.");
    }
    else
    { 
        int client_count=client_file_length();
        for(int i=0;i<client_count;i++)
        { 
            user = (Client*)malloc(sizeof(Client));
            fread(user,sizeof(Client),1,client_file);
            if(!strcmp(user->CIN,cin))
            { 
                fclose(client_file);
                return user;
            }
        }
    }
    fclose(client_file);
    return NULL;
}

//returns a client if a client has the corresponding email exists else returns NULL
Client* get_client_by_email(char* email){
    Client* user = (Client*)malloc(sizeof(Client));
    FILE* client_file = fopen(PATH_CLIENT_BIN_FILE, "rb");
    if (client_file == NULL) 
    {
        perror("Error opening clients file while getting the client by email.");
    }
    else
    { 
        int client_count=client_file_length();
        for(int i=0;i<client_count;i++)
        {
            fread(user,sizeof(Client),1,client_file);
            if(!strcmp(user->email,email))
            { 
                fclose(client_file);
                return user;
            }
        }
    }
    fclose(client_file);
    return NULL;
}

//returns a client if a client has the corresponding email exists else returns NULL
Client* get_client_by_account(int account_number){
    Client* user=(Client*)malloc(sizeof(Client));
    FILE* client_file = fopen(PATH_CLIENT_BIN_FILE, "rb");
    if (client_file == NULL) 
    {
        perror("Error opening file while getting the client by account.");
    }
    else
    { 
        int client_count=client_file_length();
        for(int i=0;i<client_count;i++)
        {
            fread(user, sizeof(Client), 1, client_file); 
            if (user->account_number == account_number) 
            {
                fclose(client_file);
                return user;
            }
        }
    }
    fclose(client_file);
    return NULL;
}

//returns the position of the client which is superior or equal to 0 if it exists else it returns -1.
int find_client_account_position(int account_number) {
    int pos=-1;
    Client* user=(Client*)malloc(sizeof(Client));
    FILE* client_file = fopen(PATH_CLIENT_BIN_FILE, "rb");
    if (client_file == NULL) 
    {
        perror("Error opening file while getting the client position.");
    }
    else
    {
        while (!feof(client_file)) 
        {

            fread(user, sizeof(Client), 1, client_file);
            if (user->account_number == account_number) 
            {
                pos=(ftell(client_file)/sizeof(Client))-1;
                break;
            }
        }
    }
    fclose(client_file);
    return pos;
}

Client get_client_by_position(int pos){
    Client* user=(Client*)malloc(sizeof(Client));
    FILE* client_file = fopen(PATH_CLIENT_BIN_FILE, "rb");
    if (client_file == NULL) 
    {
        perror("Error opening file while searching for a client by position.");
    }
    else
    {
        fseek(client_file,sizeof(Client)*pos,SEEK_SET);
        fread(user, sizeof(Client), 1, client_file);
    }
    fclose(client_file);
    return *user;
}

//function that retrun the lenght of clients file
int client_file_length(){  
    int lenght=0; 
    long lenght_in_bytes;
    FILE* client_file = fopen(PATH_CLIENT_BIN_FILE,"rb");
    if (client_file == NULL) 
    {
        perror("Error opening file while counting the file number of elements.");
    }
    else
    { 
        fseek(client_file,0,SEEK_END);
        lenght_in_bytes = ftell(client_file);
        lenght = (int)(lenght_in_bytes/sizeof(Client));
    }
    fclose(client_file);
    return lenght;
}

int staging_file_length(){  
    int lenght=0; 
    long lenght_in_bytes;
    FILE* client_file = fopen(PATH_STAGING_CLIENT_BIN_FILE,"rb");
    if (client_file == NULL) 
    {
        perror("Error opening file while counting the file number of elements.");
    }
    else
    { 
        fseek(client_file,0,SEEK_END);
        lenght_in_bytes = ftell(client_file);
        lenght = (int)(lenght_in_bytes/sizeof(Client));
    }
    fclose(client_file);
    return lenght;
}

//returns the logged client else if the authentification encounters a problem it returns a NULL.
Client* client_authentification() {
    printf("***************** Authentification *****************\n");
    Auth* temp = (Auth*)malloc(sizeof(Auth));
    Client* user=(Client*)malloc(sizeof(Client));
    // Saisie de numero de compte et du mot de passe pour l'authentification.
    printf("Account Number  :   ");
    scanf("%d",&temp->account_number);
    printf("%d",temp->account_number);
    getchar();
    printf("Password        :   ");
    fgets_no_newline_return(temp->password,MAX_PASSWORD_LENGTH);
    printf("%s",temp->password);

    // Ouverture du fichier binaire des clients.
    FILE* client_file = fopen(PATH_CLIENT_BIN_FILE, "rb");
    if (client_file == NULL) 
    {
        perror("Error opening clients file while login");
    }
    else
    { 
        int client_count=client_file_length();
        for (int i = 0; i < client_count; i++) 
        {
            fread(user, sizeof(int), 1,client_file);
            if ((temp->account_number == user->account_number) && (strcmp(temp->password, user->password) == 0)) 
            { 
                break;
            }
            user=NULL;
        }
    }
    fclose(client_file);
    free(temp);
    return user;
}

//returns 1 if the update is passed else it returns 0.
int update_client_in_file(Client client,Client update){
    FILE *client_file = fopen(PATH_CLIENT_BIN_FILE, "a+b");
    if (client_file == NULL) 
    {
        perror("Error opening file while updating the client");
    }
    else
    {
        if (find_client_account_position(client.account_number)!=-1)
        {
            fseek(client_file,sizeof(Client)*find_client_account_position(client.account_number),SEEK_SET);
            if(fwrite(&update,sizeof(Client),1,client_file)!=0)
            {
                fclose(client_file);
                return 1;
            }
        }
        else
        {
            puts("Client does not esist.");
        }    
    }
    return 0;
}

//returns 1 if the transfer is successful else it returns 0.
int make_transfer(Client sender) {

    Client *receiver=NULL,*update_sender=NULL,*update_receiver=NULL;
    double transfer_amount;
    int account_number;
    printf("Enter the destination account number: ");
    scanf("%d", &account_number);
    *receiver = *get_client_by_account(account_number);
    printf("Enter the transfer amount: ");
    scanf("%lf", &transfer_amount);

    if (find_client_account_position(receiver->account_number)==-1)
    {
        puts("Unsuccessful operation! The account does not exist."); 
    }
    else
    {
        if (transfer_amount<=sender.balance)
        {
            *receiver=*get_client_by_account(receiver->account_number);
            *update_sender=sender;
            update_sender->balance -= transfer_amount;
            update_receiver->balance += transfer_amount;
            update_client_in_file(sender,*update_sender);
            update_client_in_file(*receiver,*update_receiver);
            system("clear");
            printf("Transfer of %.2f successfully completed to %s %s.\n",transfer_amount,receiver->first_name,receiver->last_name);
            return(1);
        }
        else
        { 
            printf("Unsuccessful operation, your balance is  insufficient.\n"); 
        }
    }
    return(0);  
}

//returns 1 if the deposit is successfull else it returns 0.
int make_deposit(Client client) {
    double deposit_amount;
    Client* temp=(Client*)malloc(sizeof(Client));
    *temp=client;
    printf("Enter the deposit amount  :     ");
    scanf("%lf", &deposit_amount);
    temp->balance += deposit_amount;
    if(update_client_in_file(client,*temp)==1)
    {
        printf("Deposit of %.2f successfully completed.\n", deposit_amount);
        free(temp);
        return 1;
    }
    free(temp);
    return 0;
}

//returns 1 if the withdrawal is successfull else it returns 0.
int make_withdrawal(Client client) {
    double withdrawal_amount;
    Client* temp=(Client*)malloc(sizeof(Client));
    *temp=client;
    printf("Enter the withdrawal amount  :     ");
    scanf("%lf", &withdrawal_amount);
    if (withdrawal_amount > client.balance) 
    {
        printf("Unsuccessful operation, your balance is  insufficient.\n");
    }
    else
    {
        temp->balance -= withdrawal_amount;
        if(update_client_in_file(client,*temp)==1)
        {
            printf("Withdrawal of %.2f successfully completed.\n", withdrawal_amount);
            free(temp);
            return 1;
        }
    }
    free(temp);
    return 0;
}

void client_login_page(){
    char choice;
    printf("\n************ Client Space ************\n\n");
    printf("1. Log in\n");
    printf("2. Sign up\n");
    printf("3. Check request status\n");
    printf("4. Forgot password\n");
    printf("5. Quit\n\n");
    printf("Enter the number of the desired operation  :  ");
    scanf("%c", &choice);
    switch (choice) {
        case '1':  //log in
            getchar();
            Client* user=(Client*)malloc(sizeof(Client));
            *user = *client_authentification();
            puts("the user who wants to connect.");
            display_client_profile(*user);
            if (user == NULL)
            {
                client_main_page(*user);
            }
            client_login_page();
            break;

        case '2':// Sign up
            getchar();
            system("clear"); 
            request_client_account_creation();
            system("clear");
            client_login_page();
            break;

        /*case '3':// Forgot password
            system("clear"); 
            restore_client_password(client);
            getchar();
            system("clear");
            break;*/

        case '4':// Quit page
            system("clear");
            getchar();
            printf("Thank you! Goodbye.\n");
            
            return;
        default:
            printf("Invalid choice. Please enter a valid number.\n");
            getchar();
            system("clear");
            client_login_page();
            break;
    }

}

void client_main_page(Client client){
    int choice;
    printf("\n************ Welcome, %s %s! ************\n", client.first_name, client.last_name);
    printf("1. Deposit\n");
    printf("2. Withdrawal\n");
    printf("3. Transfer\n");
    printf("4. Check balance\n");
    printf("5. Log out\n");
    printf("Enter the number of the desired operation  :  ");
    scanf("%d", &choice);
    
    switch (choice) {
        case 1:  //deposit
            system("clear");
            make_deposit(client);
            getchar();
            system("clear");
            break;

        case 2:// Withdrawal
            system("clear"); 
            make_withdrawal(client);
            getchar();
            system("clear");
            break;

        case 3:// Transfer
            system("clear"); 
            make_transfer(client);
            getchar();
            system("clear");
            break;
        case 4://balance inquiry
            system("clear"); 
            printf("Your account balance : %.2f\n", client.balance);
            getchar();
            system("clear");
            break;
        case 5:// Quit page
            system("clear");
            printf("Thank you! Goodbye.\n");
            getchar();
            return;
        default:
            printf("Invalid choice. Please enter a valid number.\n");
            break;
    }
}

#endif
