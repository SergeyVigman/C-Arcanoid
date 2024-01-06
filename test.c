#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>

int getch (void)
{
        int ch;
        struct termios oldt, newt;

        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON|ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

        return ch;
}

int random_int(int min, int max)
{
   return min + rand() % (max + 1 - min);
}

int main()
{
    const int mapX = 10, mapY = 20; // размер игрового поля 
    int heroX = 5, heroY = 10; // изначальное положение героя 
    int enemyX = 2, enemyY = 4; // изначальное положение врага 
    char map[mapX][mapY];
    char key;
    char HeroModel = '>';
    int score = 0; 
    do{
        system("clear");

        // отрисовка поля и героя
        for (int x = 0; x < mapX; x++)
        {
            for (int y = 0; y < mapY; y++)
            {
                map[x][y] = '#';
                if (((x > 0) & (x != mapX - 1)) & ((y > 0) & (y != mapY - 1))) map[x][y] = ' ';
                map[heroX][heroY] = HeroModel; // отрисовка героя
                map[enemyX][enemyY] = '*'; // отрисовка врага
                printf("%c ", map[x][y]);
            }
            printf("\n");
        }  

        // счет
        printf("SCORE\t=\t%d\n", score);

        // printf("X = %d\n", enemyX);
        // printf("Y = %d\n", enemyY);

        // движение
        key = getch();
        if ((key == 'w') & (heroX > 1)) {
            heroX--; 
            HeroModel = '^';
        }
        if ((key == 'a') & (heroY > 1)) {
            heroY--; 
            HeroModel = '<';
        }
        if ((key == 's') & (heroX < 8)) {
            heroX++; 
            HeroModel = 'v';
        }
        if ((key == 'd') & (heroY < 18)) {
            heroY++; 
            HeroModel = '>';
        }

        // уничтожение врага
        if ((heroY == enemyY) && (heroX == enemyX)){
            enemyX = random_int(1, 8); 
            enemyY = random_int(1, 18);
            score++;
        }

    } while (key != 'e');
}