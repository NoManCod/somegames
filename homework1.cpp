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
                printf("�� ");
            } else {
                printf("�� ");
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
        printf("\nʣ�����: %d\n", game->mines);
        printf("���� (�� ��) �Ͳ��� (1: ����, 2: ���/ȡ�����): ");

        int x, y, action;
        while (scanf("%d %d %d", &x, &y, &action) != 3 ||
               x < 0 || x >= game->rows ||
               y < 0 || y >= game->cols ||
               action < 1 || action > 2) {
            clearInputBuffer();
            printf("������Ч������������ (�� �� ����): ");
        }

        if (action == 1) {
            reveal(game, x, y);
            if (game->board[x][y] == '*' && !game->flagged[x][y]) {
                displayBoard(game, 1);
                printf("��Ϸʧ�ܣ���ȵ��˵��ף�\n");
                return;
            }
        } else {
            if (!game->revealed[x][y]) {
                game->flagged[x][y] = !game->flagged[x][y];
            } else {
                printf("���ܱ���ѷ����ĸ��ӣ�\n");
            }
        }

        if (checkWin(game)) {
            displayBoard(game, 1);
            printf("��ϲ�㣬�ɹ��ų������е��ף�\n");
            return;
        }
    }
}

void showRules() {
    printf("\n============== ɨ����Ϸ ==============\n");
    printf("��ϷĿ�꣺\n");
    printf("  ��������û�е��׵ĸ���\n");
    printf("��Ϸ����\n");
    printf("  1. ���ֱ�ʾ����8�������еĵ�������\n");
    printf("  2. �ո��ʾ��Χû�е���\n");
    printf("  3. ʹ�ñ�ǹ��ܱ�ǿ��ܵĵ���λ��\n");
    printf("����������\n");
    printf("  �����ʽ���к� �к� ����\n");
    printf("  ����1: ��������\n");
    printf("  ����2: ���/ȡ����Ǹ���\n");
    printf("��ʾ��\n");
    printf("  �� - δ�����ĸ���\n");
    printf("  ? - ��ǵĸ���\n");
    printf("  ���� - ��Χ��������\n");
    printf("  �ո� - ��Χû�е���\n");
    printf("======================================\n\n");
}

void selectDifficulty(int *rows, int *cols, int *mines) {
    printf("\n��ѡ����Ϸ�Ѷȣ�\n");
    printf("1. �� (8x8, 10������)\n");
    printf("2. �е� (16x16, 40������)\n");
    printf("3. ���� (24x24, 99������)\n");
    printf("4. �Զ���\n");
    printf("ѡ�� (1-4): ");

    int choice;
    while (scanf("%d", &choice) != 1 || choice < 1 || choice > 4) {
        clearInputBuffer();
        printf("������Ч����ѡ��1-4: ");
    }

    switch (choice) {
        case 1: *rows = *cols = 8; *mines = 10; break;
        case 2: *rows = *cols = 16; *mines = 40; break;
        case 3: *rows = *cols = 24; *mines = 99; break;
        case 4:
            printf("\n�Զ������ã�\n");
            printf("�������� (%d-%d): ", MIN_ROWS, MAX_ROWS);
            while (scanf("%d", rows) != 1 || *rows < MIN_ROWS || *rows > MAX_ROWS) {
                clearInputBuffer();
                printf("������Ч��������%d-%d: ", MIN_ROWS, MAX_ROWS);
            }

            printf("�������� (%d-%d): ", MIN_COLS, MAX_COLS);
            while (scanf("%d", cols) != 1 || *cols < MIN_COLS || *cols > MAX_COLS) {
                clearInputBuffer();
                printf("������Ч��������%d-%d: ", MIN_COLS, MAX_COLS);
            }

            int maxMines = (*rows * *cols) - 1;
            printf("��������� (%d-%d): ", MIN_MINES, maxMines);
            while (scanf("%d", mines) != 1 || *mines < MIN_MINES || *mines > maxMines) {
                clearInputBuffer();
                printf("������Ч��������%d-%d: ", MIN_MINES, maxMines);
            }
            break;
    }
}

void printBanner() {
    printf("\n");
    printf("  �������������[ �����[   �����[���������������[���������������[�������������[ \n");
    printf("  �����X�T�T�����[�^�����[ �����X�a�����X�T�T�T�T�a�����X�T�T�T�T�a�����X�T�T�����[\n");
    printf("  �������������X�a �^���������X�a ���������������[�����������[  �������������X�a\n");
    printf("  �����X�T�T�����[  �^�����X�a  �^�T�T�T�T�����U�����X�T�T�a  �����X�T�T�����[\n");
    printf("  �������������X�a   �����U   ���������������U���������������[�����U  �����U\n");
    printf("  �^�T�T�T�T�T�a    �^�T�a   �^�T�T�T�T�T�T�a�^�T�T�T�T�T�T�a�^�T�a  �^�T�a\n");
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

        printf("\n�Ƿ�Ҫ����һ�֣� (1: ��, ����: �˳�): ");
        int choice;
        if (scanf("%d", &choice) != 1 || choice != 1) {
            break;
        }

        printf("\n�Ƿ�Ҫ������Ϸ�Ѷȣ� (1: ��, ����: ��): ");
        if (scanf("%d", &choice) == 1 && choice == 1) {
            selectDifficulty(&rows, &cols, &mines);
        }
        clearInputBuffer();
    }

    printf("\n��л���棡�ټ���\n");
    return 0;
}
