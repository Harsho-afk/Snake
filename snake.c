#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int width, height, fruitX, fruitY, score = 0, gameover = 0, speed, direction;
int *hash;
WINDOW *win;

enum { UP, LEFT, DOWN, RIGHT };

struct snake {
    int x, y;
    struct snake *next;
} *snake;

void getInput() {
    char input = getch();
    if (input >= 'A' && input <= 'Z')
        input = input - 'A' + 'a';
    switch (input) {
    case 'a':
    case KEY_LEFT:
        if (direction != RIGHT)
            direction = LEFT;
        break;
    case 's':
    case KEY_DOWN:
        if (direction != UP)
            direction = DOWN;
        break;
    case 'd':
    case KEY_RIGHT:
        if (direction != LEFT)
            direction = RIGHT;
        break;
    case 'w':
    case KEY_UP:
        if (direction != DOWN)
            direction = UP;
        break;
    case 'q':
        gameover = 1;
        break;
    }
}

void update() {
    if (gameover)
        return;
    int x = snake->x, y = snake->y;
    switch (direction) {
    case LEFT:
        x--;
        break;
    case DOWN:
        y++;
        break;
    case RIGHT:
        x++;
        break;
    case UP:
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
            ; // get new position of fruit
        speed -= 20;
        if (speed < 250)
            speed = 250;
        timeout(speed);
    }
}

void print() {
    if (gameover)
        return;
    werase(win);
    box(win, 0, 0);
    wattron(win, COLOR_PAIR(1));
    mvwprintw(win, fruitY, fruitX, "@");
    wattroff(win, COLOR_PAIR(1));
    wattron(win, COLOR_PAIR(2));
    mvwprintw(win, snake->y, snake->x,
              (direction == UP)     ? "^"
              : (direction == LEFT) ? "<"
              : (direction == DOWN) ? "v"
                                    : ">");
    for (struct snake *tmp = snake->next; tmp != NULL; tmp = tmp->next) {
        mvwprintw(win, tmp->y, tmp->x, "O");
    }
    wattroff(win, COLOR_PAIR(2));
    mvprintw(((LINES - height) / 2) - 1, (COLS - width) / 2, "Score = %d",
             score);
    wrefresh(win);
    wrefresh(stdscr);
}

void init() {
    int startY, startX;
    // terminal
    initscr();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(FALSE);
    cbreak();
    speed = 500;
    timeout(speed);
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    // window
    height = LINES < 20 ? LINES : 20;
    width = COLS < 20 ? (LINES + 50) : 70;
    if (COLS < width)
        width = COLS;
    startY = (LINES - height) / 2;
    startX = (COLS - width) / 2;
    win = newwin(height, width, startY, startX);
    refresh();
    wrefresh(win);
    // snake
    snake = malloc(sizeof(struct snake));
    snake->next = NULL;
    snake->x = width / 2;
    snake->y = height / 2;
    hash = calloc(width * height, sizeof(int));
    hash[(snake->y * width) + snake->x] = 1;
    direction = UP;
    // fruit
    while (((fruitX = (rand() % (width - 2)) + 1) &&
            (fruitY = (rand() % (height - 2)) + 1)) &&
           (hash[(fruitY * width) + fruitX] == 1))
        ; // get position of fruit where snake is not there
}

void freeAll() {
    struct snake* tmp = snake;
    while(tmp != NULL) {
        snake = snake->next;
        free(tmp);
        tmp = snake;
    }
}

int main() {
    srand(time(NULL));
    init();
    print();
    while (!gameover) {
        getInput();
        update();
        print();
    }
    delwin(win);
    endwin();
    freeAll();
    return 0;
}