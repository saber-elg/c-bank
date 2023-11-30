#ifndef CLIENT_FUNCTIONS_H
#define CLIENT_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "struct.h"
#include "set_const.h"
#include <time.h>

//#include <openssl/sha.h>

/*prototyping functions*/

int create_client_account();
void display_client_profile(Client client);
char* create_num_password();
char* get_password();
int cin_is_in_file(Client client);
int email_is_in_file(Client client);
int client_file_length();
int login_client();
void save_client_to_file(Client *client);
int update_client_in_file(Client client,Client update);
Client get_client_by_account(int account_number);
int client_position_by_account(int account_number);
int make_transfer(Client sender);
void make_deposit(Client *client);
void make_withdrawal(Client client);
void Client_main_page(Client *clients);


/*Declaring the functions*/

int create_client_account(){
    printf("******************     Create an account      ******************\n");
    Client client;
    printf("First Name    :   ");
    fgets(client.first_name, FIRST_NAME_LENGHT, stdin);
    printf("Last Name     :   ");
    fgets(client.last_name, LAST_NAME_LENGHT, stdin);
    printf("CIN           :   ");
    fgets(client.CIN, MAX_CNE_LENGHT, stdin); 
    client.balance=0;
    client.account_number=client_file_length()+1;

    //verifiying whether another account already exist with this CIN or not.
    if (cin_is_in_file(client))
    {
        puts("This cin already exists.");
        return 0;
    }

    printf("Email         :   ");
    fgets(client.auth.email, MAX_EMAIL_LENGHT, stdin);

    //verifiying whether another account already exist with this email or not.
    if (email_is_in_file(client))
    {
        puts("This email already exists.");
        return 0;
    }

    printf("Password      :   ");
    strcpy(client.auth.password,create_num_password());

    system("cls");
    save_client_to_file(&client);
    return 1;
}

void display_client_profile(Client client){
    printf("******************     Profile      *******************\n\n");
    printf("Account Number   :  %d\n",client.account_number);
    printf("Account Holder   :  %s %s\n",client.first_name,client.last_name);
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

int cin_is_in_file(Client client){
    FILE *client_file = fopen(PATH_CLIENT_BIN_FILE, "rb");
    if (client_file == NULL) {
        printf("Error opening clients file");
        return (0);
    }
    int client_count=client_file_length();
    Client* user=NULL;
    for(int i=0;i<client_count;i++)
    { 
        fread(user,sizeof(Client),1,client_file);
        if(!strcmp(user->CIN,client.CIN))
        return 1;
    }
    return 0;
}

int email_is_in_file(Client client){
    FILE *client_file = fopen(PATH_CLIENT_BIN_FILE, "rb");
    if (client_file == NULL) {
        printf("Error opening clients file");
        return (0);
    }
    int client_count=client_file_length();
    Client* user=NULL;
    for(int i=0;i<client_count;i++)
    { 
        fread(user,sizeof(Client),1,client_file);
        if(!strcmp(user->auth.email,client.auth.email))
        return 1;
    }
    return 0;
}

int client_file_length(){   //function that retrun the lenght of clients file
    FILE *client_file = fopen("Client.bin","rb");
    fseek(client_file,0,SEEK_END);
    long lenght_in_bytes = ftell(client_file);
    fclose(client_file);
    int lenght = (int)lenght_in_bytes/sizeof(Client);
    return lenght;
}

int login_client() {
    printf("Client\n\n");
    printf("***************** Authentification *****************");
    Auth *temp = (Auth *)malloc(sizeof(Auth));

    // Saisie de l'email et du mot de passe
    printf("Email      :   ");
    fgets(temp->email, MAX_EMAIL_LENGHT, stdin);
    printf("Password   :   ");
    strcpy(temp->password, get_password());

    // Ouverture du fichier binaire des clients
    FILE *client_file = fopen(PATH_CLIENT_BIN_FILE, "rb");
    if (client_file == NULL) {
        perror("Error opening clients file while login");
        return 0;
    }

    Client *client=NULL;
    int client_count=client_file_length();
    fread(client, sizeof(Client), 1,client_file);
    for (int i = 0; i < client_count; i++) {
        fread(client, sizeof(int), 1,client_file);
        if (strcmp(temp->email, client->auth.email) == 0 && strcmp(temp->password, client->auth.password) == 0) 
        return i;
    }
    fclose(client_file);

    // Libération de la mémoire allouée dynamiquement
    free(client);
    free(temp);
    return 0;
}

void save_client_to_file(Client *client){
    FILE *client_file = fopen(PATH_CLIENT_BIN_FILE, "ab");
    if (client_file == NULL) {
        perror("Error opening file while saving the client");
        return; //break the function
    }
    fseek(client_file, client_file_length()*sizeof(Client), SEEK_SET);
    fwrite(client,sizeof(Client),1,client_file);
    fclose(client_file);
}

int update_client_in_file(Client client,Client update){
    FILE *client_file = fopen(PATH_CLIENT_BIN_FILE, "a+b");
    if (client_file == NULL) {
        perror("Error opening file while updating the client");
        return (-1); //break the function
    }
    fseek(client_file,sizeof(Client)*client_position_by_account(client.account_number),SEEK_SET);
    if(fwrite(&update,sizeof(Client),1,client_file)!=0)
    {
        fclose(client_file);
        return 1;
    }
    return 0;
}

Client get_client_by_account(int account_number){
    Client user;
    FILE *client_file = fopen(PATH_CLIENT_BIN_FILE, "rb");
    if (client_file == NULL) {
        perror("Error opening file while saving the client");
    }
    else{ 
        // Read clients one by one until the end of the file
        while (!feof(client_file)) 
        {
            fread(&user, sizeof(Client), 1, client_file);
            // Check if the current client has the target account number
            if (user.account_number == account_number) 
            break; // Client found and break the loop
        }
        fclose(client_file);
    }
    return (user);
}

int client_position_by_account(int account_number) {
    Client *user=(Client*)malloc(sizeof(Client));
    FILE *client_file = fopen(PATH_CLIENT_BIN_FILE, "rb");
    if (client_file == NULL) {
        perror("Error opening file while saving the client");
        return (-1); //break the function
    }
    // Read clients one by one until the end of the file
    while (!feof(client_file)) 
    {
        fread(user, sizeof(Client), 1, client_file);
        // Check if the current client has the target account number
        if (user->account_number == account_number) {
            int pos=(ftell(client_file)/sizeof(client))-1;
            return pos;  // Client found and return its position
        }
    }
    fclose(client_file);
    return -1;  // Client with the target account number not found
}

int make_transfer(Client sender) {

    Client *receiver=NULL,*update_sender=NULL,*update_receiver=NULL;
    double transfer_amount;
    printf("Enter the destination account number: ");
    scanf("%d", &receiver->account_number);
    printf("Enter the transfer amount: ");
    scanf("%lf", &transfer_amount);

    if (client_position_by_account(receiver->account_number)==-1)
    {
        puts("Account not not found");
        return(0);
    }
    else
    {
        if (transfer_amount<=sender.balance)
        {
            *receiver=get_client_by_account(receiver->account_number);
            *update_sender=sender;
            update_sender->balance -= transfer_amount;
            update_receiver->balance +=transfer_amount;
            update_client_in_file(sender,*update_sender);
            update_client_in_file(*receiver,*update_receiver);
            return(1);
        }
        else
        printf("Insufficient funds for the transfer.\n"); 
        return(0);
    }
    
}

void make_deposit(Client *client) {
    double deposit_amount;
    // Saisie du montant du dépôt
    printf("Enter the deposit amount: ");
    scanf("%lf", &deposit_amount);

 
    //Erreur !! clients[i].balance est extrait de fichier & I have to modefie it also dans le fichier apres l'ajout de montant !!!!!!!!!!!!
    client->balance+= deposit_amount;

    printf("Deposit of %.2f successfully completed.\n", deposit_amount);
}

void make_withdrawal(Client client) {
    double withdrawal_amount;
    printf("Enter the withdrawal amount: ");
    scanf("%lf", &withdrawal_amount);
   
    //Verifiying the balance for the withdraw
    if (withdrawal_amount > client.balance) 
    printf("Unsuccessful operation, your balance is  insufficient.\n");
    else
    {
        FILE *client_file = fopen(PATH_CLIENT_BIN_FILE, "a+b");
        if (client_file == NULL) {
        perror("Error opening clients file while making a withdraw.");
        return;
        }

        // Mise à jour du solde du client
        fseek(client_file,client_position_by_account(client.account_number)*sizeof(Client),SEEK_SET);
        client.balance -= withdrawal_amount;
        // Handle error while writing
        if (fwrite(&client, sizeof(Client), 1, client_file) != 1) {
        perror("Error writing to file");
    }
        // Affichage des informations mises à jour
        printf("Withdrawal of %.2f successfully completed.\n", withdrawal_amount);
    }
}

void Client_main_page(Client *client){
    int choice;
    printf("\n******* Welcome, %s %s! *******\n", client->first_name, client->last_name);
    printf("Possible transactions:\n");
    printf("1. Deposit\n");
    printf("2. Withdrawal\n");
    printf("3. Transfer\n");
    printf("4. balance inquiry");
    printf("5. Quit page\n");
    printf("Enter the number of the desired operation: ");
    scanf("%d", &choice);
    
    switch (choice) {
        case 1:  //deposit
            system("cls");
            make_deposit(client);
            getchar();
            system("cls");
            break;

        case 2:// Withdrawal
            system("cls"); 
            make_withdrawal(client);
            getchar();
            system("cls");
            break;

        case 3:// Transfer
            system("cls"); 
            make_transfer(client);
            getchar();
            system("cls");
            break;
        case 4://balance inquiry
            system("cls"); 
            printf("Your account balance : %.2f\n", client->balance);
            getchar();
            system("cls");
            break;
        case 5:
            system("cls");
            // Quit page
            printf("Thank you! Goodbye.\n");
            getchar();
            return;

        default:
            printf("Invalid choice. Please enter a valid number.\n");
            break;
    }
}

#endif