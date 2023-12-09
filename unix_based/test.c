#include <stdio.h>
#include <termios.h>
#include <unistd.h>

//  compatible getch() for unix based system
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

int main() {
    printf("Appuyez sur une touche (sans appuyer sur Entrée) : ");
    char c = unix_getch();
    printf("\nVous avez appuyé sur : %c\n", c);
    return 0;
}