                                                // ARCANOID
#include <stdbool.h> // для bool
#include <math.h> // для cos sin
#include <unistd.h> // для usleep
#include <ncurses.h> 


#define heigh 25
#define width 65
#define width_platform 7
#define C_SPEED_GAME 90000 // задержка между итерациями
#define Platform_STEP 2 // шаг платформы

int map[heigh][width]; // память поля
int PLAYER_SCORE = 0; 
int PLAYER_HEALTH = 3;
int LEVEL_NUMBER = 1;
int SCORE_TO_NEXT_LEVEL = 0;
bool start_game = false;

typedef struct TPlatform    
{
    int x, y, w;
}TPlatform;

typedef struct TBall
{
    float x, y;
    int ix, iy;
    float alfa;
}TBall;

TPlatform Platform;
TBall Ball;

void platform_init() // инициализация платформы
{
    Platform.w = width_platform;
    Platform.y = heigh - 1;
    Platform.x = (width - Platform.w) / 2;
}

void ball_init() // инициализация шарика
{
    Ball.x = Platform.x + (Platform.w / 2);
    Ball.y = Platform.y - 2;
    Ball.alfa = -1;
}

void put_ball() // помещаем шарик на карту
{
    mvprintw((int)Ball.y, (int)Ball.x, "o"); // приводим float к int без округления т.к. знаем что изначально будет целое
}

void put_platform()  // помещаем платформу на карту
{
    for (int i = 0; i < Platform.w; i++)
        mvprintw(Platform.y - 1, Platform.x + i, "=");

    for (int j = 0; j < Platform.w; j++)
        map[Platform.y - 1][Platform.x + j] = 1;
}

void refresh_round()
{
        start_game = 0;
        ball_init();
}

void show_map(int x)  // инициализация и отрисовка карты
{
    // первая строчка
    for (int i = 0; i < width; i++) 
    {
        mvprintw(0, i,"#");
        map[0][i] = 1; // в память закидываем
    }
        
    // остальные строчки
    for (int j = 1; j < heigh - 1; j++)
        for (int i = 0; i < width; i++) 
        {
            if ((i == 0) || (i == width - 1)) 
            {
                mvprintw(j, i,"#");
                map[j][i] = 1; // в память закидываем
            }
            else 
            {
                mvprintw(j, i," ");
                map[j][i] = 0; // в память закидываем
            }
        }
    
    // ============ УРОВНИ ============

    if (x > 0) 
    {
        for (int i = 15; i < width - 15; i++) // 1 <------
        {
            mvprintw(10, i,"#");
            map[10][i] = 1; // в память закидываем
        }

        if (x > 1)
        {
            for (int i = 25; i < width - 25; i++) // 2 <------
            {
                mvprintw(15, i,"#");
                map[15][i] = 1; // в память закидываем
            }
        }
        if (x > 2)
            for (int i = 27; i < width - 27; i++) // 2 <------
            {
                mvprintw(18, i,"#");
                map[18][i] = 1; // в память закидываем
            } 
    }
    // ============ УРОВНИ ============
}

void movePlatform (int x) // перемещение платформы
{
    if (x < 1) Platform.x = 1;
    else if (x > width - Platform.w - 1) Platform.x = width - Platform.w - 1;
    else Platform.x = x;
}

void update_player_score (int x)
{
    PLAYER_SCORE += x;
    SCORE_TO_NEXT_LEVEL += x;
    mvprintw(7, 70, "SCORE = %d", PLAYER_SCORE);
}

void update_player_health (int x)
{
    PLAYER_HEALTH += x;
    mvprintw(4, 70, "HEALTH = %d", PLAYER_HEALTH);
}

void update_level_number (int x)
{
    LEVEL_NUMBER += x;
    mvprintw(1, 70, "LEVEL = %d", LEVEL_NUMBER);
}

void move_ball(float y, float x) // перемещение шарика
{

    // приводим координаты типа float к типу int через округление (потому что после рассчета угла будет float)
    Ball.ix = (int)round(x); 
    Ball.iy = (int)round(y);

    Ball.y = Ball.iy;
    Ball.x = Ball.ix;
}

void change_level()
{
    SCORE_TO_NEXT_LEVEL = 0;
    LEVEL_NUMBER++;
}

void game_over ()
{
    clear();
    mvprintw(15, 30, "GAME OVER");
    mvprintw(17, 30, "SCORE = %d", PLAYER_SCORE);
}

void auto_move_ball() // полет шарика после старта игры
{
    // приводим альфу к виду от 0 до 2pi
    if (Ball.alfa < 0) Ball.alfa += M_PI * 2;           // если альфа < 0 то добавляем к ней 2пи
    if (Ball.alfa > M_PI * 2) Ball.alfa -= M_PI * 2;    // если альфа > 2 пи то отнимаем от нее эти 2пи

    TBall bl = Ball; // сохраняем старые корды шарика

    // рассчет угла наклона шарика
    move_ball(Ball.y + sin(Ball.alfa)
             ,Ball.x + cos(Ball.alfa));

    // ЛОГИКА НАПРАВЛЕНИЯ ОТСКОКА
    if (map[Ball.iy][Ball.ix] == 1) // если шарик столкнулся со стеной или ракеткой
    {
        if ((Ball.ix != bl.ix) && (Ball.iy != bl.iy)) // изменился .iy и .ix
        {
            mvprintw(heigh + 6, width + 16, "changed iy ix");
            if (map[bl.iy][Ball.ix] == map[Ball.iy][bl.ix]) // угол
            {
                mvprintw(heigh + 7, width + 17, "UGOL");    
                bl.alfa = bl.alfa + M_PI;    // инвертируем направление шарика
                update_player_score(1);
            }
            else // либо стена либо потолок
            {
                if (map[bl.iy][Ball.ix] == 1)       // если старый x и новый y это стена => вертикальная поверхность
                {
                    update_player_score(1);
                    bl.alfa = (2 * M_PI - bl.alfa) + M_PI; // отражаем по вертикали
                }
                else
                {
                    update_player_score(1);
                    bl.alfa = (2 * M_PI - bl.alfa); // отражаем по горизонтали 
                }
            }   
        }
        else if (Ball.iy == bl.iy)  // изменился только .xi
        {
            mvprintw(heigh + 8, width + 18, "HORIZONTAL");  
            bl.alfa = (2 * M_PI - bl.alfa) + M_PI;  // шарик движется по горизонтали - отражаем по вертикали
            update_player_score(1);
        }
        else                        // изменился только .yi
        {
            bl.alfa = (2 * M_PI - bl.alfa);         // шарик движется по вертикали - отражаем по горизонтали 
            // mvprintw(heigh + 9, width + 19, "VERTICAL");  
            update_player_score(1);
        }
        
        Ball = bl; // перезаписываем корды
        auto_move_ball(); // пересчитываем заново
    }

    if (Ball.iy > heigh) 
    {
        update_player_health(-1);
        refresh_round();
    }

    update_player_health(0);
    update_player_score(0);
    update_level_number(0);
    mvprintw(heigh + 5, width + 15, "move_ball(%f, %f)", Ball.y + sin(Ball.alfa), Ball.x + cos(Ball.alfa)); // корды шарика на экран
}

int main()
{
    initscr();
    start_color();
    noecho(); //убираем ввод
    curs_set(0); //убираем курсор
    keypad(stdscr, 1);
    nodelay(stdscr, TRUE);
    
    platform_init();

    int key = 0;

    do
    {
        clear();
        show_map(LEVEL_NUMBER);
        put_platform();
        put_ball();
        update_player_score(0);
        update_player_health(0);
        update_level_number(0);

        if (start_game == 0) ball_init();
        if (start_game == 1) auto_move_ball();

        if (key == KEY_LEFT) movePlatform(Platform.x - Platform_STEP);
        else if (key == KEY_RIGHT) movePlatform(Platform.x + Platform_STEP);
        if (key == KEY_UP) start_game = true;
        if (key == 119) usleep(9000000);
        if (PLAYER_HEALTH == 0) game_over();
        if (SCORE_TO_NEXT_LEVEL == 20) change_level();

        usleep(C_SPEED_GAME);
    } while ((key = getch()) != 27); // 27 - ESCAPE

    endwin();

    //что в мозгах
    printf("\n");
    for (int i = 0; i < heigh - 1; i++)
    {
    printf("\t");
        for (int j = 0; j < width; j++)
            printf("%d", map[i][j]);
    printf("\n");
    }
    printf("\n");
}