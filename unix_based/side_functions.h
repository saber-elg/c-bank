#ifndef SIDE_FUNCTIONS_H
#define SIDE_FUNCTIONS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "struct.h"
#include "set_const.h"
#include "graphic.h"
#include <termios.h>
#include <ctype.h>
#include <unistd.h>


/*Prototyping the functions*/
int push_to_request_file(Client client);
char* create_num_password();
void encrypt_password(char *password);
int get_city_id();
char* get_security_question();
void display_client_profile(Client *client);
char* get_password();
int disable_account(Client* client);
int enable_account(Client* client);
Client* get_client_by_cin(char* cin);
Client* get_client_by_email(char* email);
Client* get_client_by_account(int account_number);
int find_client_account_position(int account_number);
Client* get_client_by_position(int pos);
int client_file_length();
int staging_file_length();
int update_client_in_file(Client client,Client update);
int update_password(int account_number,  char* new_password);
void fgets_no_newline_return(char* buffer,int size);
void files_initialisation();
void clear_nprevious_lines(int n);
void clearLine();
void disableEcho();
void enableEcho();
char unix_getch();

/*Declaring the functions*/

// Returns 1 if the client is well saved in the staging file else it returns 0
int push_to_request_file(Client client){
    FILE* staging_file = fopen(PATH_STAGING_CLIENT_BIN_FILE, "ab");
    if (staging_file == NULL) 
    {
        printf("Error opening file while saving the client");
        unix_getch();
        system("clear");
        shut_down();
        exit(EXIT_FAILURE);
    }
    else
    { 
        fseek(staging_file, client_file_length()*sizeof(Client), SEEK_SET);
        fwrite(&client,sizeof(Client),1,staging_file);
        fclose(staging_file);
        return 1;
    }
}

// Returns a numeric password as a string without showing thw password while typing.
char* create_num_password() {
    char* password = (char*)malloc((MAX_PASSWORD_LENGTH ) * sizeof(char));                                     
    printf("  Enter password   :    ");
    char ch;  
    int i = 0;
    while ((ch = unix_getch()) != '\n' && i < MAX_PASSWORD_LENGTH ) {
        if (!isdigit(ch) && ch != 127)
        {
            continue;
        }
        if (ch == '\b' || ch == 127) // Check for backspace or DEL (ASCII 127)
        {  
            if (i > 0) 
            {
                printf("\b");
                printf(" ");
                printf("\b");
                i--;
            }
            continue;
        }
        *(password + i) = ch;
        printf("*");
        i++;
    }
    if (i < MIN_PASSWORD_LENGTH) {
        printf("\nPassword must be at least %d digits long.\n", MIN_PASSWORD_LENGTH);
        free(password);
        return create_num_password();
    }
    *(password+i) = '\0';  // Null-terminate the password
    printf("\n"); 
    enableEcho();
    encrypt_password(password);
    return password;
}

// Encypt typed password
void encrypt_password(char *password) {
    int length = strlen(password);
    for (int i = 0; i < length; i++) 
    {
        password[i] = ((password[i] - '0' + ENCRYPT_SHIFT) % 10) + '0';
    }
}

// Getting the city code for account number creation
int get_city_id(){
    
    while(1)
    {
        system("clear");
        printf(" -------------------- Chose Your City ---------------------\n\n");
        printf("\t 1.  Los Angeles                                         \n");
        printf("\t 2.  Chicago                                             \n");
        printf("\t 3.  Houston                                             \n");
        printf("\t 4.  Atlanta                                             \n");
        printf("\t 5.  Philadelphia                                        \n");
        printf("\t 6.  Houston                                             \n");
        printf("\t 7.  Dallas                                              \n");
        printf("\t 8.  Miami                                               \n");
        printf("\t 9.  Phoenix                                             \n\n");
        printf(" ----------------------------------------------------------\n");
        char choice = unix_getch();
        switch (choice)
        {
            case '1':
                return city1_id; 

            case '2':
                return city1_id;

            case '3':
                return city3_id;

            case '4':
                 return city4_id;

            case '5':
                 return city5_id;

            case '6':
                 return city6_id;

            case '7':
                 return city7_id;

            case '8':
                 return city8_id;

            case '9':
                 return city9_id;

            default:
                break;
        }
        system("clear");
    }
}

char* get_security_question(){
    while (1)
    {
        system("clear");
        printf(" ---------------------- Security Question --------------------------\n\n");
        printf("\t 1. What is the first name of your oldest cousin?       \n");
        printf("\t 2. What is your favorite food?                         \n");
        printf("\t 3. Where did you go on your favorite vacation?         \n\n");
        printf(" -----------------------------------------------------------------------\n\n");
        char choice = unix_getch();
        switch (choice)
        {
            case '1':
                system("clear");
                return("What is the first name of your oldest cousin?");

            case '2':
                system("clear");
                return("What is your favorite food?");

            case '3':
                system("clear");
                return("Where did you go on your favorite vacation?");

            default:
                break;
        }
        system("clear");
    } 
}

// Displays the client related info: acount number, full name, CIN, email.
void display_client_profile(Client *client){
    system("clear");
    if (client==NULL)
    {
        red();
        puts("This client does not exists.");
        color_reset();
        unix_getch();
    }
    else
    {
        yellow();
        printf("*********************** Account Profile  ***********************\n\n");
        color_reset();
        printf("              Account Number   :  %d\n",client->account_number);
        printf("              Account Holder   :  %s %s\n",client->first_name,client->last_name);
        printf("              Account CIN      :  %s\n",client->CIN);
        printf("              Account E-mail   :  %s\n\n",client->email);
        yellow();
        printf("****************************************************************\n\n");
        color_reset();
    }    
}

// Returns the typed password from the user.
char* get_password(){
    char* password = (char*)malloc((MAX_PASSWORD_LENGTH) * sizeof(char)); 
    char ch;
    int i = 0;
    disableEcho();
    while ((ch = unix_getch()) != '\n' && i < MAX_PASSWORD_LENGTH ) 
    {   
        if (ch == '\b' || ch == 127) // Check for backspace or DEL (ASCII 127)
        {  
            if (i > 0) 
            {
                printf("\b");
                printf(" ");
                printf("\b");
                i--;
            }
            continue;
        }
        *(password + i) = ch;
        i++;
    }
    enableEcho();
    *(password+i)='\0';
    encrypt_password(password);
    return password;
}

// Returns 1 if the account is closed else 0
int disable_account(Client* client){
    Client* update = (Client*)malloc(sizeof(Client));
    *update = *client;
    update->account_status = 0; 
    return update_client_in_file(*client,*update);
}

// Returns 1 if the account is enabled else 0
int enable_account(Client* client){
    Client* update = (Client*)malloc(sizeof(Client));
    *update = *client;
    update->account_status = 1; 
    return update_client_in_file(*client,*update);
}

// Returns a client if a client has the corresponding cin exists else returns NULL
Client* get_client_by_cin(char* cin){
    Client* user=(Client*)malloc(sizeof(Client));
    FILE* client_file = fopen(PATH_CLIENT_BIN_FILE, "rb");
    if (client_file == NULL) 
    {
        printf("Error opening clients file while getting the client by cin.");
        unix_getch();
        system("clear");
        shut_down();
        exit(EXIT_FAILURE);
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

// Returns a client if a client has the corresponding email exists else returns NULL
Client* get_client_by_email(char* email){
    Client* user = (Client*)malloc(sizeof(Client));
    FILE* client_file = fopen(PATH_CLIENT_BIN_FILE, "rb");
    if (client_file == NULL) 
    {
        printf("Error opening clients file while getting the client by email.");
        unix_getch();
        system("clear");
        shut_down();
        exit(EXIT_FAILURE);
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

// Returns a client if a client has the corresponding email exists else returns NULL
Client* get_client_by_account(int account_number){
    Client* user=(Client*)malloc(sizeof(Client));
    FILE* client_file = fopen(PATH_CLIENT_BIN_FILE, "rb");
    if (client_file == NULL) 
    {
        perror("Error opening file while getting the client by account.");
        unix_getch();
        system("clear");
        shut_down();
        exit(EXIT_FAILURE);
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
        unix_getch();
        system("clear");
        shut_down();
        exit(EXIT_FAILURE);
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

Client* get_client_by_position(int pos){
    Client* user=(Client*)malloc(sizeof(Client));
    FILE* client_file = fopen(PATH_CLIENT_BIN_FILE, "rb");
    if (client_file == NULL) 
    {
        perror("Error opening file while searching for a client by position.");
        unix_getch();
        system("clear");
        shut_down();
        exit(EXIT_FAILURE);
    }
    else
    {
        if (client_file_length()<pos+1)
        {
            fclose(client_file);
            return NULL;
        }
        else
        {
            fseek(client_file,sizeof(Client)*pos,SEEK_SET);
            fread(user, sizeof(Client), 1, client_file);
        }
    }
    fclose(client_file);
    return user;
}

// Retruns the lenght of clients file
int client_file_length(){  
    int lenght=0; 
    long lenght_in_bytes;
    FILE* client_file = fopen(PATH_CLIENT_BIN_FILE,"rb");
    if (client_file == NULL) 
    {
        perror("Error opening file while counting the file number of elements.");
        unix_getch();
        system("clear");
        shut_down();
        exit(EXIT_FAILURE);
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

// Retruns the lenght of clients staging file
int staging_file_length(){  
    int lenght=0; 
    long lenght_in_bytes;
    FILE* client_file = fopen(PATH_STAGING_CLIENT_BIN_FILE,"rb");
    if (client_file == NULL) 
    {
        perror("Error opening file while counting the file number of elements.");
        unix_getch();
        system("clear");
        shut_down();
        exit(EXIT_FAILURE);
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

// Returns 1 if the update is passed else it returns 0.
int update_client_in_file(Client client,Client update){
    FILE* client_file = fopen(PATH_CLIENT_BIN_FILE, "rb+");
    if (client_file == NULL) 
    {
        printf("Error opening file while updating the client");
        unix_getch();
        system("clear");
        shut_down();
        exit(EXIT_FAILURE);
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
            else
            {
                red();
                printf("Error while updating the balance.");
                color_reset();
                unix_getch();
            }
        }
        else
        {
            blue();
            puts("Client does not esist.");
            color_reset();
            unix_getch();
        }    
    }
    fclose(client_file);
    return 0;
}

// Returns 1 if password is well updated else it returns 0
int update_password(int account_number,  char* new_password) 
{
    FILE* client_file = fopen(PATH_CLIENT_BIN_FILE, "rb+");
    if (client_file == NULL) 
    {
        perror("Error opening file while changing password.\n");
        unix_getch();
        system("clear");
        shut_down();
        exit(EXIT_FAILURE);
    }

    Client* user = (Client*)malloc(sizeof(Client));

    for(int i=0;i<client_file_length();i++) {
        fread(user, sizeof(Client), 1, client_file);
        if (user->account_number == account_number) 
        {
            // update the password for the clinet
            strcpy(user->password, new_password);
            // return to client's position
            fseek(client_file, -sizeof(Client), SEEK_CUR);
            // write new structure on the old one
            fwrite(user, sizeof(Client), 1, client_file);
            fclose(client_file);
            return 1;
        }
    }
    fclose(client_file);
    return 0;
}

// fgets equivalance without new linw return character '\n' the buffer should be dynamicaly allocated 
void fgets_no_newline_return(char* buffer,int size){
    fgets(buffer,size,stdin);
    buffer[strcspn(buffer,"\n")]='\0';
}

// Create clients files for the first time
void files_initialisation(){
    FILE* client_file=fopen(PATH_CLIENT_BIN_FILE,"ab");
    if (client_file == NULL) 
    {
        printf("Error while initialising clients file.");
        unix_getch();
        system("clear");
        shut_down();
        exit(EXIT_FAILURE);
    }
    FILE* stage_file=fopen(PATH_STAGING_CLIENT_BIN_FILE,"ab");
    if (stage_file == NULL) 
    {
        printf("Error while initialising staging clients file.");
        unix_getch();
        system("clear");
        shut_down();
        exit(EXIT_FAILURE);
    }
    fclose(client_file);
    fclose(stage_file);
}

void clear_nprevious_lines(int n) {
    for (int i = 0; i < n; i++) {
        printf("\033[F\033[2K");  // Move cursor up one line and clear the line
    }
    fflush(stdout);  // Flush the output to ensure immediate effect
}

void clearLine(){
    printf("\033[2K\r"); // ANSI escape code to clear the line and move the cursor to the beginning of the line
    fflush(stdout);      // Flush the output to ensure immediate effect
}

// Function to disable echoing in the terminal
void disableEcho() {
    struct termios term;
    
    // Get the current terminal settings
    tcgetattr(STDIN_FILENO, &term);

    // Turn off echoing
    term.c_lflag &= ~ECHO;

    // Set the new terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

// Function to enable echoing in the terminal
void enableEcho() {
    struct termios term;
    
    // Get the current terminal settings
    tcgetattr(STDIN_FILENO, &term);

    // Turn on echoing
    term.c_lflag |= ECHO;

    // Set the new terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

// Compatible getch() for unix based system
char unix_getch() {
    struct termios oldt, newt;
    char ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}


#endif