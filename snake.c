#include <stdio.h>
#include <stdlib.h>

int width, height, fruitX, fruitY, score, gameover;
int *hash;
char direction;

struct snake {
    int x, y;
    struct snake *next;
} *snake;

void getInput() {
    char input = getchar();
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
    if(gameover) return;
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
    } else {
        score++;
    }
}

void print() {
    if(gameover) return;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            if (i == 0 || i == width - 1 || j == 0 || j == height - 1) {
                printf("#");
                continue;
            }
            if (i == fruitX && j == fruitY) {
                printf("@");
                continue;
            }
            if (snake->x == j && snake->y == i) {
                switch (direction) {
                case 'a':
                    printf("<");
                    break;
                case 's':
                    printf("v");
                    break;
                case 'd':
                    printf(">");
                    break;
                case 'w':
                    printf("^");
                    break;
                }
                continue;
            }
            if (hash[(i * width) + j] == 1) {
                printf("0");
                continue;
            }
            printf(" ");
        }
        printf("\n");
    }
    printf("Score = %d\n", score);
    printf("Press 'q' to exit\n");
}

void init(int sizeX, int sizeY) {
    width = sizeX;
    height = sizeY;
    score = 0;
    gameover = 0;
    fruitX = (rand() % width) + 1;
    fruitY = (rand() % height) + 1;
    snake = malloc(sizeof(struct snake));
    snake->next = NULL;
    while (((snake->x = (rand() % width) + 1) &&
            (snake->y = (rand() % height) + 1)) &&
           (fruitX == snake->x && fruitY == snake->y))
        ;
    hash = calloc(width * height, sizeof(int));
    hash[(snake->y * width) + snake->x] = 1;
    direction = 'w';
}

int main() {
    init(20, 40);
    while (!gameover) {
        getInput();
        update();
        print();
    }
    return 0;
}