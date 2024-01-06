#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <ncurses.h>


int main() 
{
    initscr();
    int key;
    key = getch();
    printw("Вы нажали = %d\n", key);

    getch();
    endwin();
}