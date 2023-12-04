#include <stdio.h>
#include <stdlib.h>
#include "struct.h"
#include "set_const.h"
#include <time.h>
#include "project.h"
#include "client_functions.h"
#include "side_functions.h"

int main()
{
    system("clear");
    files_initialisation();
    //request_client_account_creation();
    //accept_account_creation_requests();
    //Client client=get_client_by_position(1);
    //client_main_page(client);
    display_client_profile(*client_authentification());
    //request_client_account_creation();
    //request_client_account_creation();
    //admin_or_client();
    //display_client_profile(get_client_by_position(0));
    //accept_account_creation_requests();
    //display_client_profile(*client_authentification());
    //client_login_page();
    return 0;
}

