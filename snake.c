#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int width, height, fruitX, fruitY, score, gameover;
int *hash;
char direction;

struct snake {
    int x, y;
    struct snake *next;
} *snake;

void getInput() {
    char input = getch();
    switch (input) {
    case 'a':
    case 'A':
        if (direction != 'd')
            direction = 'a';
        break;
    case 's':
    case 'S':
        if (direction != 'w')
            direction = 's';
        break;
    case 'd':
    case 'D':
        if (direction != 'a')
            direction = 'd';
        break;
    case 'w':
    case 'W':
        if (direction != 's')
            direction = 'w';
        break;
    case 'q':
    case 'Q':
        gameover = 1;
        break;
    }
}

void update() {
    if (gameover)
        return;
    int x = snake->x, y = snake->y;
    switch (direction) {
    case 'a':
        x--;
        break;
    case 's':
        y++;
        break;
    case 'd':
        x++;
        break;
    case 'w':
        y--;
        break;
    }
    if (x == 0 || x == width - 1 || y == 0 || y == height - 1 ||
        hash[(y * width) + x] == 1) {
        gameover = 1;
        return;
    }
    struct snake *newHead = malloc(sizeof(struct snake));
    newHead->x = x;
    newHead->y = y;
    newHead->next = snake;
    snake = newHead;
    if (snake->x != fruitX || snake->y != fruitY) {
        struct snake *tmp = snake;
        while (tmp->next->next != NULL)
            tmp = tmp->next;
        struct snake *remove = tmp->next;
        tmp->next = NULL;
        hash[(remove->y * width) + remove->x] = 0;
        hash[(snake->y * width) + snake->x] = 1;
    } else if (snake->x == fruitX && snake->y == fruitY) {
        score++;
        hash[(snake->y * width) + snake->x] = 1;
        while (((fruitX = (rand() % (width - 2)) + 1) &&
                (fruitY = (rand() % (height - 2)) + 1)) &&
               (hash[(fruitY * width) + fruitX] == 1))
            ;
    }
}

void print() {
    if (gameover)
        return;
    erase();
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == fruitX && i == fruitY) {
                printw("@");
                continue;
            }
            if (snake->x == j && snake->y == i) {
                switch (direction) {
                case 'a':
                    printw("<");
                    break;
                case 's':
                    printw("v");
                    break;
                case 'd':
                    printw(">");
                    break;
                case 'w':
                    printw("^");
                    break;
                }
                continue;
            }
            if (hash[(i * width) + j] == 1) {
                printw("0");
                continue;
            }
            if (i == 0 || i == height - 1 || j == 0 || j == width - 1) {
                printw("#");
                continue;
            }
            printw(" ");
        }
        printw("\n");
    }
    printw("Score = %d\n", score);
    printw("Press 'q' to exit\n");
    refresh();
}

void init(int sizeX, int sizeY) {
    initscr();
    keypad(stdscr, TRUE);
    //noecho();
    curs_set(FALSE);
    cbreak();
    timeout(500);
    width = sizeX;
    height = sizeY;
    score = 0;
    gameover = 0;
    fruitX = (rand() % (width - 2)) + 1;
    fruitY = (rand() % (height - 2)) + 1;
    snake = malloc(sizeof(struct snake));
    snake->next = NULL;
    while (((snake->x = (rand() % (width - 2)) + 1) &&
            (snake->y = (rand() % (height - 2)) + 1)) &&
           (fruitX == snake->x && fruitY == snake->y))
        ;
    hash = calloc(width * height, sizeof(int));
    hash[(snake->y * width) + snake->x] = 1;
    direction = 'w';
}

int main() {
    srand(time(NULL));
    init(20, 20);
    print();
    while (!gameover) {
        getInput();
        update();
        print();
    }
    endwin();
    return 0;
}