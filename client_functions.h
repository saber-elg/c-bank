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
Auth* authentification(char* email, char* password);
int login_client(const char *path);
void save_clients_to_file(const Client *clients, size_t num_clients);
void save_transactions_to_file(const char *path, const Transaction *transactions, size_t num_transactions);
int read_transactions_from_file(Transaction *transactions);
void make_deposit(Client *clients, size_t num_clients, Transaction *transactions, size_t *num_transactions);
void Client_main_page(Client *clients, Transaction *transactions);
void make_withdrawal(Client *clients, Transaction *transactions);
int find_client_by_account_number(const Client *clients, size_t num_clients, int account_number);
void make_transfer(Client *clients, size_t num_clients, Transaction *transactions);


/*Declaring the functions*/

int create_client_account() {
    size_t num_clients = 0;
    Client clients[MAX_CLIENTS];

    printf("******************     Create an account      ******************\n");
    printf("First Name    :   ");
    fgets(clients[num_clients].first_name, FIRST_NAME_LENGTH, stdin);
    printf("Last Name     :   ");
    fgets(clients[num_clients].last_name, LAST_NAME_LENGTH, stdin);
    printf("CIN           :   ");
    fgets(clients[num_clients].CIN, MAX_CNE_LENGTH, stdin); 
    printf("Email         :   ");
    fgets(clients[num_clients].auth.email, MAX_EMAIL_LENGTH, stdin);
    printf("Password      :   ");
    strcpy(clients[num_clients]->auth.password,create_num_password());//this is yours verifie it
    system("cls");

    num_clients++;

    // Enregistrer le tableau de clients dans le fichier
    save_clients_to_file(clients, num_clients);
    
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

int login_client() {
    printf("Client\n\n");
    printf("***************** Authentification *****************");
    Auth *temp = (Auth *)malloc(sizeof(Auth));

    // Saisie de l'email et du mot de passe
    printf("Email      :   ");
    fgets(temp->email, MAX_EMAIL_LENGHT, stdin);
    printf("Password   :   ");
    strcpy(temp->password, get_password());

    // Ouverture du fichier
    const char *path = "C:/Users/admin/Desktop/Vs code_project C/clients.b";
    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        printf("Error opening file");
        exit(EXIT_FAILURE);
    }

    size_t num_clients;
    // Lecture du nombre de clients dans le fichier
    fread(&num_clients, sizeof(size_t), 1, file);

    // Allocation dynamique du tableau de clients
    Client *clients = (Client *)malloc(sizeof(Client) * num_clients);

    // Lecture du tableau de clients
    fread(clients, sizeof(Client), num_clients, file);

    for (size_t i = 0; i < num_clients; i++) {
        // Comparaison avec les informations saisies par l'utilisateur
        if (strcmp(temp->email, clients[i].auth.email) == 0 && strcmp(temp->password, clients[i].auth.password) == 0) {
            return i;
            break;
        }
    }

    // Libération de la mémoire allouée dynamiquement
    free(clients);
    free(temp);
    fclose(file);

    return 0;
}

void save_clients_to_file(const Client *clients, int num_clients) {
    const char *path = "C:/Users/admin/Desktop/Vs code_project C/clients.b";
    FILE *file = fopen(path, "ab");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    fwrite(clients + (num_clients - 1), sizeof(Client), 1, file);
    fclose(file);
}
void save_transactions_to_file(const Transaction *transactions, int num_transactions) {
    const char *path = "C:/Users/admin/Desktop/Vs code_project C/clients.b";
    FILE *f = fopen(path, "ab");
    if (f == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    /*transactions: C'est un pointeur vers le début du tableau des transactions.
    + (num_transactions - 1): Cela déplace le pointeur de transactions vers la position de la dernière 
    transaction dans le tableau. Puisque les indices dans un tableau commencent à 0,
    la dernière transaction est à la position num_transactions - 1*/
    fwrite(transactions + (num_transactions - 1), sizeof(Transaction), 1, f);
    fclose(f);
}
int read_transactions_from_file(Transaction *transactions) {
    const char *path = "C:/Users/admin/Desktop/Vs code_project C/transaction.b";
    FILE *f = fopen(path, "rb");
    if (file == NULL) {
        printf("Error opening file");
        exit(EXIT_FAILURE);
    }
    int num_transactions = fread(transactions, sizeof(Transaction), MAX_TRANSACTIONS, file);
    //if fichier est empty num_transactions=0 a ajouter !!!!
    fclose(f);
    //returner le dernier element de transactions
    return num_transactions;
}
void make_deposit(Client *clients, size_t num_clients, Transaction *transactions, size_t *num_transactions) {
    double deposit_amount;
    int i;
    // Saisie du montant du dépôt
    printf("Enter the deposit amount: ");
    scanf("%lf", &deposit_amount);

 
    //Erreur !! clients[i].balance est extrait de fichier & I have to modefie it also dans le fichier apres l'ajout de montant !!!!!!!!!!!!
    clients[i].balance += deposit_amount;

    // Récupération du dernier numéro de transaction
    int last_transaction_number = read_transactions_from_file(transactions);

    // Ajout d'une nouvelle transaction
    transactions[*num_transactions].trans_number = last_transaction_number + 1;
    transactions[*num_transactions].type = 1;  // Type 1 pour dépôt
    transactions[*num_transactions].trans_amount = deposit_amount;
    transactions[*num_transactions].trans_date = time(NULL);

    // Enregistrement de la transaction dans le fichier
    save_transactions_to_file(transactions, *num_transactions + 1);

    printf("Deposit of %.2f successfully completed.\n", deposit_amount);

    // Affichage des informations mises à jour
    printf("-----------Updated information-----------\n");
    printf("Account balance       : %.2f\n", clients[i].balance);
    printf("Transaction number    : %d\n", transactions[*num_transactions].trans_number);
    printf("Transaction type      : %d\n", transactions[*num_transactions].type);
    printf("Transaction amount    : %.2f\n", transactions[*num_transactions].trans_amount);
    printf("Transaction date      : %s\n", ctime(&transactions[*num_transactions].trans_date));
}
void Client_main_page(Client *clients, Transaction *transactions){
    int choice;
    printf("\n******* Welcome, %s %s! *******\n", clients->first_name, clients->last_name);
    printf("Possible transactions:\n");
    printf("1. Deposit\n");
    printf("2. Withdrawal\n");
    printf("3. Transfer\n");
    printf("4. balance inquiry")
    printf("5. Quit page\n");
    printf("Enter the number of the desired operation: ");
    scanf("%d", &choice);
    
    switch (choice) {
        case 1:  //deposit
            system("cls");
            make_deposit(clients,num_clients, transactions, num_transactions)
            getchar();
            system("cls");
            break;

        case 2:// Withdrawal
            system("cls"); 
            make_withdrawal(clients, transactions);
            getchar();
            system("cls");
            break;

        case 3:// Transfer
            system("cls"); 
            make_transfer(clients, num_clients, transactions);
            getchar();
            system("cls");
            break;
        case 4://balance inquiry
            system("cls"); 
            printf("Your account balance : %.2f\n", clients[i]->balance);
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
void make_withdrawal(Client *clients, Transaction *transactions) {
    const char *path = "C:/Users/admin/Desktop/Vs code_project C/transaction.b";
    double withdrawal_amount;
    int i;
    // Saisie du montant du retrait
    printf("Enter the withdrawal amount: ");
    scanf("%lf", &withdrawal_amount);
   
    // Vérification des fonds suffisants pour le retrait
    if (withdrawal_amount > clients[i]->balance) {
        printf("Insufficient funds for withdrawal.\n");
        return;
    }
    //Erreur !!clients[i]->balance est extrait de fichier en mode lire et je dois modefier le apres le retrait !!!!!!!
    // Mise à jour du solde du client
    clients[i]->balance -= withdrawal_amount;

    // Récupération du dernier numéro de transaction
    int num_transactions = read_transactions_from_file(transactions);

    // Mise à jour des informations de la nouvelle transaction
    transactions[num_transactions].trans_number = num_transactions + 1;
    transactions[num_transactions].type = 2;  // Type 2 pour le retrait
    transactions[num_transactions].trans_amount = withdrawal_amount;
    transactions[num_transactions].trans_date = time(NULL);

    // Enregistrement de la transaction dans le fichier
    save_transactions_to_file(transactions, num_transactions + 1);

    // Affichage des informations mises à jour
    printf("Withdrawal of %.2f successfully completed.\n", withdrawal_amount);
    printf("Updated information:\n");
    printf("Account balance: %.2f\n", clients[i]->balance);
    printf("Transaction number: %d\n", transactions[num_transactions].trans_number);
    printf("Transaction type: %d\n", transactions[num_transactions].type);
    printf("Transaction amount: %.2f\n", transactions[num_transactions].trans_amount);
    printf("Transaction date: %s", ctime(&transactions[num_transactions].trans_date));
}
int find_client_by_account_number(const Client *clients, size_t num_clients, int account_number) {
    for (size_t i = 0; i < num_clients; ++i) {
        if (clients[i].account_number == account_number) {
            return i; // Retourne l'indice du client trouvé
        }
    }
    return -1; // Retourne 0 si le client n'est pas trouvé
}
void make_transfer(Client *clients, size_t num_clients, Transaction *transactions) {
    const char *path = "C:/Users/admin/Desktop/Vs code_project C/transaction.b";
    double transfer_amount;
    int i;
    // Saisie du numéro de compte de destination
    printf("Enter the destination account number: ");
    scanf("%d", &destination_account);
    
    // Recherche du compte de destination dans le fichier
    
    int j = find_client_by_account_number(clients, num_clients, destination_account);
    if (j == -1) {
        printf("Source account not found.\n");
        return;
    }

    Client *clients[j]; 
    // Saisie du montant du transfert
    printf("Enter the transfer amount: ");
    scanf("%lf", &transfer_amount);

    // Vérification de la suffisance des fonds
    if (clients[i]->balance >= transfer_amount) {
        // Effectuer le transfert
        clients[i]->balance -= transfer_amount;
        clients[j]->balance += transfer_amount;

        // Récupération du dernier numéro de transaction
        int num_transactions = read_transactions_from_file(transactions);

        // Mise à jour des informations de la nouvelle transaction
        transactions[num_transactions].trans_number = num_transactions + 1;
        transactions[num_transactions].type = 3;  // Type 3 pour le transfert
        transactions[num_transactions].trans_amount = transfer_amount;
        transactions[num_transactions].trans_date = time(NULL);

        // Enregistrement de la transaction dans le fichier
        save_transactions_to_file(transactions, num_transactions + 1);

        // Affichage des informations mises à jour
        printf("Transfer of %.2f from account %d to account %d completed.\n", transfer_amount, source_account, destination_account);
        printf("-------------Updated information------------ \n");
        printf("Source account balance        : %.2f\n", clients[i]->balance);
        printf("Destination account balance   : %.2f\n", clients[j]->balance);
        printf("Transaction number            : %d\n", transactions[num_transactions].trans_number);
        printf("Transaction type              : %d\n", transactions[num_transactions].type);
        printf("Transaction amount            : %.2f\n", transactions[num_transactions].trans_amount);
        printf("Transaction date              : %s", ctime(&transactions[num_transactions].trans_date));
    } else {
        printf("Insufficient funds for the transfer.\n");
    }
}


#endif