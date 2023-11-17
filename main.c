#include<stdio.h>
#include<stdlib.h>
#include "set_const.h"
<<<<<<< HEAD
//#include "admin_functions.h"
//#include "client_functions.h"
#include "struct.h"
int create_client_account(){
    Client *client = (Client*)malloc(sizeof(Client));
    printf("******************     Creer un compte      ******************\n");
    /*content*/
    printf("First Name   :   ");
    fgets(client->first_name,FIRST_NAME_LENGHT,stdin);
    printf("Last Name    :   ");
    fgets(client->last_name,LAST_NAME_LENGHT,stdin);
    printf("Email        :   ");
    fgets(client->auth.email,MAX_EMAIL_LENGHT,stdin);
    client->auth.password,MAX_PASSWORD_LENGTH,stdin;
    system("clear");
    return 1;
}
int main()
{
    create_client_account();
=======
#include "admin_functions.h"
#include "client_functions.h"
#include "struct.h"

int main()
{

>>>>>>> 31a747e (reinitialise the repo)
    return 0;
}

