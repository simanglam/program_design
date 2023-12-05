#include <stdio.h>

void displayMap(int n, char map[][100]);
void moveQueen(int n, char map[][100], int moveCount, char moves[], int pauseCount);

int main() {
    int n;
    char map[100][100];
    int moveCount;
    char moves[100];
    int pauseCount;

    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        scanf("%s", map[i]);
    }

    scanf("%d", &moveCount);
        for (int i = 0; i < moveCount; i++) {
        scanf(" %c", &moves[i]);

        if (moves[i] == 'C') {
            pauseCount = 3;
            }
    }
        moveQueen(n, map, moveCount, moves, pauseCount);
        displayMap(n, map);
        return 0;
    }

    void displayMap(int n, char map[][100]) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                printf("%c ", map[i][j]);
            }
            printf("\n");
        }
    }
 
//皇后移動
    void moveQueen(int n, char map[][100], int moveCount, char moves[], int pauseCount) {
        int Qrow, Qcol, row, col;
        char temp[100][100];
        // 原本的皇后
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                temp[i][j] = map[i][j];
                if (map[i][j] == 'Q') {
                    Qrow = i;
                    Qcol = j;
                }
            }
        }

    
        // 遍歷每一步移動
        for (int i = 0; i < moveCount; i++) {
            // 移動的皇后
            int newQrow = Qrow;
            int newQcol = Qcol;

            switch (moves[i]) {
                case 'w':
                    newQrow--;
                    break;
                case 's':
                    newQrow++;
                    break;
                case 'a':
                    newQcol--;
                    break;
                case 'd':
                    newQcol++;
                    break;
            }

                
                if (map[newQrow][newQcol] == 'C') {
                pauseCount = 3;
                map[newQrow][newQcol] = 'Q';
                }
                map[Qrow][Qcol] = 'O';
    
                // 皇后當前位置變為 'Q'
                map[newQrow][newQcol] = 'Q';
                pauseCount--;
    
                // 更新皇后的位置
                Qrow = newQrow;
                Qcol = newQcol;

                // 擴張霧氣
                if (pauseCount == 0) {
                    for (int x = 0; x < n; x++) {
                        for (int y = 0; y < n; y++) {
                            if (temp[x][y] == 'X') {
                                if (x > 0 && map[x - 1][y] == 'O') map[x - 1][y] = 'X';
                                if (x < n - 1 && map[x + 1][y] == 'O') map[x + 1][y] = 'X';
                                if (y > 0 && map[x][y - 1] == 'O') map[x][y - 1] = 'X';
                                if (y < n - 1 && map[x][y + 1] == 'O') map[x][y + 1] = 'X';
                            }
                        }
                }
            }
        }
}