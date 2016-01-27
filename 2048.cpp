//
// Created by KING J on 1/3/16.
//

#include <iostream>
#include <iomanip>

using namespace std;

typedef unsigned int uint;

enum directions {
    UP, DOWN, LEFT, RIGHT
};

class Tile {
public:
    Tile() : val(0), blocked(false) { }

    uint val;
    bool blocked;
};

class Game2048 {
    uint score;
    Tile board[4][4];
    bool won, done, moved;

    void drawBoard() {
        cout << string(100, '\n');
        cout << "SCORE: " << score << endl << endl;

        for (int i = 0; i < 4; ++i) {
            cout << "+------+------+------+------+" << endl << "| ";
            for (int j = 0; j < 4; ++j) {
                if (!board[i][j].val) cout << setw(4) << " ";
                else cout << setw(4) << board[i][j].val;
                cout << " | ";
            }
            cout << endl;
        }
        cout << "+------+------+------+------+" << endl << endl;
    }

    bool canMerge(int i, int j, uint v) {
        if (i < 0 || i > 3 || j < 0 || j > 3) return false;
        return board[i][j].val == v;
    }

    bool canMove() {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                if (!board[i][j].val) return true;
            }
        }

        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                if (canMerge(i - 1, j, board[i][j].val)) return true;
                if (canMerge(i + 1, j, board[i][j].val)) return true;
                if (canMerge(i, j - 1, board[i][j].val)) return true;
                if (canMerge(i, j + 1, board[i][j].val)) return true;
            }
        }

        return false;
    }

    void moveHorizontal(int x, int y, int d) {
        if (board[x][y + d].val && board[x][y + d].val == board[x][y].val && !board[x][y].blocked &&
            !board[x][y + d].blocked) {
            board[x][y].val = 0;
            board[x][y + d].val *= 2;
            board[x][y + d].blocked = true;
            score += board[x][y + d].val;
            moved = true;
        }
        else if (!board[x][y + d].val && board[x][y].val) {
            board[x][y + d].val = board[x][y].val;
            board[x][y].val = 0;
            moved = true;
        }

        if (d > 0 && y + d < 3) moveHorizontal(x, y + d, 1);
        if (d < 0 && y + d > 0) moveHorizontal(x, y + d, -1);
    }

    void moveVertical(int x, int y, int d) {
        if (board[x + d][y].val && board[x][y].val == board[x + d][y].val && !board[x][y].blocked &&
            !board[x + d][y].blocked) {
            board[x + d][y].val *= 2;
            board[x][y].val = 0;
            board[x + d][y].blocked = true;
            score += board[x + d][y].val;
            moved = true;
        }
        else if (board[x][y].val && !board[x + d][y].val) {
            board[x + d][y].val = board[x][y].val;
            board[x][y].val = 0;
            moved = true;
        }

        if (d > 0 && x + d < 3) moveVertical(x + d, y, 1);
        if (d < 0 && x + d > 0) moveVertical(x + d, y, -1);
    }

    void move(directions d) {
        switch (d) {
            case UP:
                for (int y = 0; y < 4; ++y) {
                    int x = 1;
                    while (x < 4) {
                        if (board[x][y].val) moveVertical(x, y, -1);
                        ++x;
                    }
                }
                break;
            case DOWN:
                for (int y = 0; y < 4; ++y) {
                    int x = 2;
                    while (x >= 0) {
                        if (board[x][y].val) moveVertical(x, y, 1);
                        --x;
                    }
                }
                break;
            case LEFT:
                for (int x = 0; x < 4; ++x) {
                    int y = 1;
                    while (y < 4) {
                        if (board[x][y].val) moveHorizontal(x, y, -1);
                        ++y;
                    }
                }
                break;
            case RIGHT:
                for (int x = 0; x < 4; ++x) {
                    int y = 2;
                    while (y >= 0) {
                        if (board[x][y].val) moveHorizontal(x, y, 1);
                        --y;
                    }
                }
        }
    }

    void waitKey() {
        moved = false;
        char c;
        cout << "(W)Up (S)Down (A)Left (D)Right" << endl;
        cin >> c;
        c &= 0x5F;

        switch (c) {
            case 'W':
                move(UP);
                break;
            case 'A':
                move(LEFT);
                break;
            case 'S':
                move(DOWN);
                break;
            case 'D':
                move(RIGHT);
        }

        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                board[i][j].blocked = false;
            }
        }
    }

    void addTile() {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                if (!board[i][j].val) {
                    uint a, b;
                    do {
                        a = rand() % 4;
                        b = rand() % 4;
                    }
                    while (board[a][b].val);

                    int s = rand() % 100;
                    if (s > 89) board[a][b].val = 4;
                    else board[a][b].val = 2;
                    if (canMove()) return;
                    else break;
                }
            }
        }
        done = true;
    }

public:
    Game2048() : done(false), won(false), moved(false), score(0) { }

    void startGame() {
        addTile();
        while (true) {
            if (moved) addTile();
            drawBoard();
            if (done) break;
            waitKey();
        }
        string s = "Game Over!";
        if (won) s = "You did it!";
        cout << s << endl << endl;
    }

};

//int main() {
//    srand(static_cast<uint>(time(NULL)));
//    Game2048 game2048;
//    game2048.startGame();
//}