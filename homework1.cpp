#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define MAX_ROWS 24
#define MAX_COLS 24
#define MIN_ROWS 5
#define MIN_COLS 5
#define MIN_MINES 1

typedef struct {
    char board[MAX_ROWS][MAX_COLS];
    int rows, cols, mines;
    int revealed[MAX_ROWS][MAX_COLS];
    int flagged[MAX_ROWS][MAX_COLS];
    int gameOver;
    int win;
    int firstMove;
} Minesweeper;

void clearInputBuffer() {
    while (getchar() != '\n');
}

void initializeBoard(Minesweeper *game) {
    for (int i = 0; i < game->rows; i++) {
        for (int j = 0; j < game->cols; j++) {
            game->board[i][j] = ' ';
            game->revealed[i][j] = 0;
            game->flagged[i][j] = 0;
        }
    }
    game->gameOver = 0;
    game->win = 0;
    game->firstMove = 1;
}

void placeMines(Minesweeper *game, int safeX, int safeY) {
    int mineCount = 0;
    while (mineCount < game->mines) {
        int x = rand() % game->rows;
        int y = rand() % game->cols;
        if (game->board[x][y] != '*' && !(x == safeX && y == safeY)) {
            game->board[x][y] = '*';
            mineCount++;
        }
    }

    for (int i = 0; i < game->rows; i++) {
        for (int j = 0; j < game->cols; j++) {
            if (game->board[i][j] != '*') {
                int count = 0;
                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {
                        int ni = i + dx;
                        int nj = j + dy;
                        if (ni >= 0 && ni < game->rows && nj >= 0 && nj < game->cols && game->board[ni][nj] == '*') {
                            count++;
                        }
                    }
                }
                game->board[i][j] = (count == 0) ? ' ' : '0' + count;
            }
        }
    }
}

void displayBoard(Minesweeper *game, int showMines) {
    printf("\n  ");
    for (int j = 0; j < game->cols; j++) {
        printf("%2d", j);
    }
    printf("\n");

    for (int i = 0; i < game->rows; i++) {
        printf("%2d ", i);
        for (int j = 0; j < game->cols; j++) {
            if (game->flagged[i][j]) {
                printf("? ");
            } else if (game->revealed[i][j]) {
                if (game->board[i][j] == ' ') {
                    printf("  ");
                } else {
                    printf("%c ", game->board[i][j]);
                }
            } else if (game->gameOver && showMines && game->board[i][j] == '*') {
                printf("× ");
            } else {
                printf("■ ");
            }
        }
        printf("\n");
    }
}

void reveal(Minesweeper *game, int x, int y) {
    if (x < 0 || x >= game->rows || y < 0 || y >= game->cols ||
        game->revealed[x][y] || game->flagged[x][y]) {
        return;
    }

    if (game->firstMove) {
        placeMines(game, x, y);
        game->firstMove = 0;
    }

    game->revealed[x][y] = 1;

    if (game->board[x][y] == '*') {
        game->gameOver = 1;
        game->win = 0;
        return;
    }

    if (game->board[x][y] == ' ') {
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx != 0 || dy != 0) {
                    reveal(game, x + dx, y + dy);
                }
            }
        }
    }
}

int checkWin(Minesweeper *game) {
    for (int i = 0; i < game->rows; i++) {
        for (int j = 0; j < game->cols; j++) {
            if (game->board[i][j] != '*' && !game->revealed[i][j]) {
                return 0;
            }
        }
    }
    game->win = 1;
    game->gameOver = 1;
    return 1;
}

void play(Minesweeper *game) {
    while (!game->gameOver) {
        displayBoard(game, 0);
        printf("\n剩余地雷: %d\n", game->mines);
        printf("输入 (行 列) 和操作 (1: 翻开, 2: 标记/取消标记): ");

        int x, y, action;
        while (scanf("%d %d %d", &x, &y, &action) != 3 ||
               x < 0 || x >= game->rows ||
               y < 0 || y >= game->cols ||
               action < 1 || action > 2) {
            clearInputBuffer();
            printf("输入无效！请重新输入 (行 列 操作): ");
        }

        if (action == 1) {
            reveal(game, x, y);
            if (game->board[x][y] == '*' && !game->flagged[x][y]) {
                displayBoard(game, 1);
                printf("游戏失败！你踩到了地雷！\n");
                return;
            }
        } else {
            if (!game->revealed[x][y]) {
                game->flagged[x][y] = !game->flagged[x][y];
            } else {
                printf("不能标记已翻开的格子！\n");
            }
        }

        if (checkWin(game)) {
            displayBoard(game, 1);
            printf("恭喜你，成功排除了所有地雷！\n");
            return;
        }
    }
}

void showRules() {
    printf("\n============== 扫雷游戏 ==============\n");
    printf("游戏目标：\n");
    printf("  翻开所有没有地雷的格子\n");
    printf("游戏规则：\n");
    printf("  1. 数字表示相邻8个格子中的地雷数量\n");
    printf("  2. 空格表示周围没有地雷\n");
    printf("  3. 使用标记功能标记可能的地雷位置\n");
    printf("操作方法：\n");
    printf("  输入格式：行号 列号 操作\n");
    printf("  操作1: 翻开格子\n");
    printf("  操作2: 标记/取消标记格子\n");
    printf("提示：\n");
    printf("  ■ - 未翻开的格子\n");
    printf("  ? - 标记的格子\n");
    printf("  数字 - 周围地雷数量\n");
    printf("  空格 - 周围没有地雷\n");
    printf("======================================\n\n");
}

void selectDifficulty(int *rows, int *cols, int *mines) {
    printf("\n请选择游戏难度：\n");
    printf("1. 简单 (8x8, 10个地雷)\n");
    printf("2. 中等 (16x16, 40个地雷)\n");
    printf("3. 困难 (24x24, 99个地雷)\n");
    printf("4. 自定义\n");
    printf("选择 (1-4): ");

    int choice;
    while (scanf("%d", &choice) != 1 || choice < 1 || choice > 4) {
        clearInputBuffer();
        printf("输入无效！请选择1-4: ");
    }

    switch (choice) {
        case 1: *rows = *cols = 8; *mines = 10; break;
        case 2: *rows = *cols = 16; *mines = 40; break;
        case 3: *rows = *cols = 24; *mines = 99; break;
        case 4:
            printf("\n自定义设置：\n");
            printf("输入行数 (%d-%d): ", MIN_ROWS, MAX_ROWS);
            while (scanf("%d", rows) != 1 || *rows < MIN_ROWS || *rows > MAX_ROWS) {
                clearInputBuffer();
                printf("输入无效！请输入%d-%d: ", MIN_ROWS, MAX_ROWS);
            }

            printf("输入列数 (%d-%d): ", MIN_COLS, MAX_COLS);
            while (scanf("%d", cols) != 1 || *cols < MIN_COLS || *cols > MAX_COLS) {
                clearInputBuffer();
                printf("输入无效！请输入%d-%d: ", MIN_COLS, MAX_COLS);
            }

            int maxMines = (*rows * *cols) - 1;
            printf("输入地雷数 (%d-%d): ", MIN_MINES, maxMines);
            while (scanf("%d", mines) != 1 || *mines < MIN_MINES || *mines > maxMines) {
                clearInputBuffer();
                printf("输入无效！请输入%d-%d: ", MIN_MINES, maxMines);
            }
            break;
    }
}

void printBanner() {
    printf("\n");
    printf("  ██████╗ ██╗   ██╗███████╗███████╗██████╗ \n");
    printf("  ██╔══██╗╚██╗ ██╔╝██╔════╝██╔════╝██╔══██╗\n");
    printf("  ██████╔╝ ╚████╔╝ ███████╗█████╗  ██████╔╝\n");
    printf("  ██╔══██╗  ╚██╔╝  ╚════██║██╔══╝  ██╔══██╗\n");
    printf("  ██████╔╝   ██║   ███████║███████╗██║  ██║\n");
    printf("  ╚═════╝    ╚═╝   ╚══════╝╚══════╝╚═╝  ╚═╝\n");
    printf("\n");
}

int main() {
    srand((unsigned int)time(NULL));

    printBanner();
    showRules();

    int rows, cols, mines;
    selectDifficulty(&rows, &cols, &mines);

    while (1) {
        Minesweeper game;
        game.rows = rows;
        game.cols = cols;
        game.mines = mines;

        initializeBoard(&game);
        play(&game);

        printf("\n是否要再玩一局？ (1: 是, 其他: 退出): ");
        int choice;
        if (scanf("%d", &choice) != 1 || choice != 1) {
            break;
        }

        printf("\n是否要更改游戏难度？ (1: 是, 其他: 否): ");
        if (scanf("%d", &choice) == 1 && choice == 1) {
            selectDifficulty(&rows, &cols, &mines);
        }
        clearInputBuffer();
    }

    printf("\n感谢游玩！再见！\n");
    return 0;
}
