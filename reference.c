#include <ncurses.h>
#include <unistd.h>  // Для функции usleep

const int FIELD_WIDTH = 80;
const int FIELD_HEIGHT = 25;
const int PADDLE_SIZE = 3;
const int BALL_SIZE = 1;

int ballX, ballY;
int ballDirX, ballDirY;
int paddle1Y, paddle2Y;
int score1, score2;

void drawField() {
    clear();

    // Отрисовка верхней и нижней границ поля
    for (int i = 0; i < FIELD_WIDTH; i++) {
        mvprintw(0, i, "#");
        mvprintw(FIELD_HEIGHT - 1, i, "#");
    }
    //
    for (int j = 0; j < FIELD_WIDTH; j++) {
        mvprintw(j, 0, "#");
        mvprintw(j, FIELD_WIDTH - 1, "#");
    }

    // Отрисовка мяча
    mvprintw(ballY, ballX, "O");

    // Отрисовка левой и правой ракеток
    for (int i = 0; i < PADDLE_SIZE; i++) {
        mvprintw(paddle1Y + i, 2, "|");
        mvprintw(paddle2Y + i, FIELD_WIDTH - 3, "|");
    }

    // Вывод счета
    mvprintw(0, FIELD_WIDTH / 2 - 5, "Score: %d - %d", score1, score2);

    refresh();
}

int updateGame() {
    ballX += ballDirX;
    ballY += ballDirY;

    if (ballY == 1 || ballY == FIELD_HEIGHT - 2) {
        ballDirY = -ballDirY;
    }

    if ((ballX == 3 && ballY >= paddle1Y && ballY < paddle1Y + PADDLE_SIZE) ||
        (ballX == FIELD_WIDTH - 4 && ballY >= paddle2Y && ballY < paddle2Y + PADDLE_SIZE)) {
        ballDirX = -ballDirX;
    }

    if (ballX == 0 || ballX == FIELD_WIDTH - 1) {
        if (ballX == 0) {
            score2++;
        } else {
            score1++;
        }
        ballX = FIELD_WIDTH / 2;
        ballY = FIELD_HEIGHT / 2;
        ballDirX = -ballDirX;
    }

    if (score1 == 21 || score2 == 21) {
        clear();
        if (score1 == 21) {
            mvprintw(FIELD_HEIGHT / 2, FIELD_WIDTH / 2 - 10, "Player 1 wins!");
        } else {
            mvprintw(FIELD_HEIGHT / 2, FIELD_WIDTH / 2 - 10, "Player 2 wins!");
        }
        refresh();
        return 0;
    }
    return 1;
}

int main() {
    initscr();  // Инициализация ncurses
    noecho();   // Не отображать вводимые символы
    cbreak();   // Режим неканонического ввода
    nodelay(stdscr, TRUE);

    ballX = FIELD_WIDTH / 2;
    ballY = FIELD_HEIGHT / 2;
    ballDirX = 1;
    ballDirY = 1;
    paddle1Y = FIELD_HEIGHT / 2 - PADDLE_SIZE / 2;
    paddle2Y = FIELD_HEIGHT / 2 - PADDLE_SIZE / 2;
    score1 = 0;
    score2 = 0;

    while (1) {
        drawField();
        int input = getch();

        if (input == 'a' && paddle1Y > 1) {
            paddle1Y--;
        } else if (input == 'z' && paddle1Y < FIELD_HEIGHT - PADDLE_SIZE - 1) {
            paddle1Y++;
        } else if (input == 'k' && paddle2Y > 1) {
            paddle2Y--;
        } else if (input == 'm' && paddle2Y < FIELD_HEIGHT - PADDLE_SIZE - 1) {
            paddle2Y++;
        } else if (input == 'q') {
            break;
        }

        if (!updateGame()) {
            break;
        }

        usleep(35000);  // Пауза 100 миллисекунд
    }
    // usleep(900000);
    endwin();  // Завершение работы с ncurses
    return 0;
}
