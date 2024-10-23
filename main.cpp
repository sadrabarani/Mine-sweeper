#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "string"
#include "fstream"

using namespace std;

int arr[30][30];
int revealedarr[30][30];
int chosenarr[30][30];

void placeZero(int arr[30][30], int rowsize, int hightsize);

void play(int arr[30][30], int revealedArr[30][30], int rowsize, int hightsize, int mines, int chosenarr[30][30]);

void firstChoose(int arr[30][30], int revealedarr[30][30], int rowsize, int hightsize, int chosenX, int choasenY,
                 int chosenarr[30][30]);

void placeBomb(int arr[30][30], int revealedarr[30][30], int rowsize, int hightsize, int mines, int chosenarr[30][30]);

void print(int arr[30][30], int rowsize, int hightsize);

void printBoard(int arr[30][30], int revealedarr[30][30], int rowsize, int hightsize);

struct player {
    string username;
    int wins;
    int losses;
};

struct gameBoard {
    int rows;
    int hight;
    int mines;
};
player bazikon[100];
int shomarande = 0;

void jadvalemtiaz(string name, int winOrLose) {
    int i;
    for (i = 0; i <= shomarande; i++) {
        if (name == bazikon[i].username) {
            if (winOrLose == 1) {
                bazikon[i].wins += 1;
            } else
                bazikon[i].losses += 1;
            break;
        }
    }
    if (i == shomarande + 1) {
        shomarande++;
        bazikon[i].username = name;
        if (winOrLose == 1) {
            bazikon[i].wins = 1;
            bazikon[i].losses = 0;
        } else if (winOrLose == 2) {
            bazikon[i].wins = 0;
            bazikon[i].losses = 1;
        }
    }
    for (int i = 0; i < shomarande - 1; i++) {
        for (int j = 0; j < shomarande - 1 - i; ++j) {
            if (bazikon[j].wins > bazikon[j + 1].wins) {
                swap(bazikon[j], bazikon[j + 1]);
            }
            if (bazikon[j].wins == bazikon[j + 1].wins) {
                if (bazikon[j].losses < bazikon[j + 1].losses) {
                    swap(bazikon[j], bazikon[j + 1]);
                }
            }
        }
    }
}

void saveAndSortLEADERBOARD(int winOrLose) {// win=1 ; lose=2
    player players[1000];
    fstream leaderf("leaderboard.txt");
    if (leaderf.is_open()) {
        int counter = 1, i;
        string str, name;
        cout << "input your username : " << endl;
        cin >> name;
        jadvalemtiaz(name, winOrLose);
        leaderf >> players[0].username >> players[0].wins >> players[0].losses;
        while (getline(leaderf, str)) {
            leaderf >> players[counter].username >> players[counter].wins >> players[counter].losses;
            counter++;
        }
        for (i = 0; i < counter; i++) {
            if (name == players[i].username) {
                if (winOrLose == 1) {
                    players[i].wins += 1;
                } else
                    players[i].losses += 1;
                break;
            }
        }
        if (i == counter) {
            counter++;
            players[i].username = name;
            if (winOrLose == 1) {
                players[i].wins = 1;
                players[i].losses = 0;
            } else if (winOrLose == 2) {
                players[i].wins = 0;
                players[i].losses = 1;
            }
        }
        for (int i = 0; i < counter - 1; i++) {
            for (int j = 0; j < counter - 1 - i; ++j) {
                if (players[j].wins > players[j + 1].wins) {
                    swap(players[j], players[j + 1]);
                }
                if (players[j].wins == players[j + 1].wins) {
                    if (players[j].losses < players[j + 1].losses) {
                        swap(players[j], players[j + 1]);
                    }
                }
            }
        }
        leaderf.close();
        fstream leaderf1("leaderboard.txt");
        if (leaderf1.is_open()) {
            leaderf.open("leaderboard.txt", std::ofstream::out | std::ofstream::trunc);
            leaderf1.close();
            fstream leaderf2("leaderboard.txt");
            if (!(leaderf2.is_open()))
                cout << "error 1";
            int counter1 = counter;
            counter = 0;
            while (counter < counter1) {
                leaderf2 << players[counter].username << " " << players[counter].wins << " " << players[counter].losses
                         << endl;
                counter++;
            }
        } else cout << "error";
    } else
        cout << "error file does not open";
}

bool checkWin(int mines, int rows, int hights, int revealedarr[30][30]) {
    int numberOfrevealed = 0;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < hights; ++j) {
            if (revealedarr[i][j] == 1)
                numberOfrevealed++;
        }
    }
    if (numberOfrevealed == rows * hights - mines) {
        cout << "you win" << endl;
        return true;
    } else
        return false;
}

void play(int arr[30][30], int revealedArr[30][30], int rowsize, int hightsize, int mines, int chosenarr[30][30]) {
    int dastur, placeX, placeY, numberOfrevealed = 0;
    placeZero(revealedArr, rowsize, hightsize);
    placeBomb(arr, revealedArr, rowsize, hightsize, mines, chosenarr);
    if (checkWin(mines, rowsize, hightsize, revealedArr)) {
        print(arr, rowsize, hightsize);
        saveAndSortLEADERBOARD(1);
        return;
    }
    do {
        cout << "1. reveal cell" << endl << "2. flag cell " << endl << "3. unflag cell" << endl << "4.exit" << endl
             << "choose : " << endl;
        cin >> dastur;
        if (dastur != 4) {
            cout << "choose cell " << endl << "x : " << endl;
            cin >> placeX;
            cout << "y : " << endl;
            cin >> placeY;
            placeY -= 1;
            placeX = rowsize - placeX;
        }
        if (!((placeX >= 0 && placeX <= rowsize) && (placeY >= 0 && placeY <= hightsize))) {
            cout << "ERROR INPUT  please enter one of the option you have .";
            continue;
        }
        if (dastur == 1) {
            if (revealedArr[placeX][placeY] == 2) {
                cout << " this cell is flaged are you sure you want to reveal it ? " << endl << " 1. yes" << endl
                     << "2.no" << endl;
                int answ;
                cin >> answ;
                if (answ == 1) {
                    revealedArr[placeX][placeY] = 1;
                    if (arr[placeX][placeY] == 0)
                        firstChoose(arr, revealedArr, rowsize, hightsize, placeX, placeY, chosenarr);
                } else if (answ == 2) {}
                else
                    cout << "ERROR INPUT  please enter one of the option you have .";
            } else {
                if (revealedArr[placeX][placeY] == 1) {
                    cout << "its already revealed" << endl;
                } else {
                    revealedArr[placeX][placeY] = 1;
                    if (arr[placeX][placeY] == 0)
                        firstChoose(arr, revealedArr, rowsize, hightsize, placeX, placeY, chosenarr);
                }
            }
            if (arr[placeX][placeY] >= 9) {
                cout << "game over " << endl;
                saveAndSortLEADERBOARD(2);
                return;
            }
            if (checkWin(mines, rowsize, hightsize, revealedArr)) {
                print(arr, rowsize, hightsize);
                saveAndSortLEADERBOARD(1);
                return;
            }
        } else if (dastur == 2) {
            if (revealedArr[placeX][placeY] == 1)
                cout << "its already reaveled" << endl;
            else
                revealedArr[placeX][placeY] = 2;
        } else if (dastur == 3) {
            if (revealedArr[placeX][placeY] != 2) {
                cout << "this cell is not flaged ." << endl;
            } else
                revealedArr[placeX][placeY] = 0;
            if (revealedArr[placeX][placeY] == 1)
                cout << "its already reaveled" << endl;
        } else if (dastur == 4) {
            return;
        } else cout << "ERROR INPUT  please enter one of the option you have  " << endl;
        printBoard(arr, revealedArr, rowsize, hightsize);
    } while (1);

}

void placeZero(int arr[30][30], int rowsize, int hightsize) {
    for (int i = 0; i < rowsize; ++i) {
        for (int j = 0; j < hightsize; ++j) {
            arr[i][j] = 0;
        }
    }
}

void firstChoose(int arr[30][30], int revealedarr[30][30], int rowsize, int hightsize, int chosenX, int choasenY,
                 int chosenarr[30][30]) {
    int i = chosenX;
    int j = choasenY;
    if (chosenarr[chosenX][choasenY] == 1) {
        return;
    } else {
        chosenarr[chosenX][choasenY] = 1;
        revealedarr[chosenX][choasenY] = 1;
        if (i + 1 < rowsize && j + 1 < hightsize) {
            if (!(arr[i + 1][j + 1] > 8 || arr[j + 1][i + 1] <= 0)) {
                revealedarr[i + 1][j + 1] = 1;
            } else if (arr[i + 1][j + 1] == 0)
                firstChoose(arr, revealedarr, rowsize, hightsize, i + 1, j + 1, chosenarr);
        }
        if (i + 1 < rowsize && j - 1 > 0) {
            if (!(arr[i + 1][j - 1] > 8 || arr[i + 1][j - 1] <= 0)) {
                revealedarr[i + 1][j - 1] = 1;
            } else if (arr[i + 1][j - 1] == 0) {
                firstChoose(arr, revealedarr, rowsize, hightsize, i + 1, j - 1, chosenarr);
            }
        }
        if (i + 1 < rowsize) {
            if (!(arr[i + 1][j] > 8 || arr[i + 1][j] <= 0)) { revealedarr[i + 1][j] = 1; }
            else if (arr[i + 1][j] == 0)firstChoose(arr, revealedarr, rowsize, hightsize, i + 1, j, chosenarr);
        }
        if (j + 1 < hightsize) {
            if (!(arr[i][j + 1] > 8 || arr[i][j + 1] <= 0)) { revealedarr[i][j + 1] = 1; }
            else if (arr[i][j + 1] == 0)firstChoose(arr, revealedarr, rowsize, hightsize, i, j + 1, chosenarr);
        }
        if (j - 1 >= 0) {
            if (!(arr[i][j - 1] > 8 || arr[i][j - 1] <= 0)) { revealedarr[i][j - 1] = 1; }
            else if (arr[i][j - 1] == 0)firstChoose(arr, revealedarr, rowsize, hightsize, i, j - 1, chosenarr);
        }
        if (j + 1 < hightsize && i - 1 >= 0) {
            if (!(arr[i - 1][j + 1] > 8 || arr[i - 1][j + 1] <= 0)) { revealedarr[i - 1][j + 1] = 1; }
            else if (arr[i - 1][j + 1] == 0)firstChoose(arr, revealedarr, rowsize, hightsize, i - 1, j + 1, chosenarr);
        }
        if ((i - 1 >= 0 && j - 1 >= 0)) {
            if (!(arr[i - 1][j - 1] > 8 || arr[i - 1][j - 1] <= 0)) { revealedarr[i - 1][j - 1] = 1; }
            else if (arr[i - 1][j - 1] == 0)firstChoose(arr, revealedarr, rowsize, hightsize, i - 1, j - 1, chosenarr);
        }
        if (i - 1 >= 0) {
            if (!(arr[i - 1][j] > 8 || arr[i - 1][j] <= 0)) { revealedarr[i - 1][j] = 1; }
            else if (arr[i - 1][j] == 0)firstChoose(arr, revealedarr, rowsize, hightsize, i - 1, j, chosenarr);
        }
    }
}

void placeBomb(int arr[30][30], int revealedarr[30][30], int rowsize, int hightsize, int mines, int chosenarr[30][30]) {
    int chosencelX, chosenCelY, x, y, counter = 0;
    cout << "choose cell :";
    cin >> chosencelX >> chosenCelY;
    chosenCelY -= 1;
    chosencelX = rowsize - chosencelX;
    revealedarr[chosencelX][chosenCelY] = 1;
    placeZero(arr, rowsize, hightsize);
    while (counter < mines) {
        int x = (1.0 * rand() / RAND_MAX) * rowsize;
        int y = (1.0 * rand() / RAND_MAX) * hightsize;
        if (x == chosencelX && y == chosenCelY)continue;
        else if (arr[x][y] == 9) { continue; }
        else {
            counter++;
            arr[x][y] = 9;
        }
    }
    for (int i = 0; i < rowsize; ++i) {
        for (int j = 0; j < hightsize; j++) {
            if (i + 1 < rowsize && j + 1 < hightsize) { if ((arr[i + 1][j + 1] > 8)) { arr[i][j]++; }}
            if (i + 1 < rowsize && j - 1 >= 0) { if (arr[i + 1][j - 1] > 8) { arr[i][j]++; }}
            if (i + 1 < rowsize) { if (arr[i + 1][j] > 8) { arr[i][j]++; }}
            if (j + 1 < hightsize) { if (arr[i][j + 1] > 8) { arr[i][j]++; }}
            if (j - 1 >= 0) { if (arr[i][j - 1] > 8) { arr[i][j]++; }}
            if (j + 1 < hightsize && i - 1 >= 0) { if (arr[i - 1][j + 1] > 8) { arr[i][j]++; }}
            if ((i - 1 >= 0 && j - 1 >= 0)) { if (arr[i - 1][j - 1] > 8) { arr[i][j]++; }}
            if (i - 1 >= 0) { if (arr[i - 1][j] > 8) { arr[i][j]++; }}
        }
    }
    firstChoose(arr, revealedarr, rowsize, hightsize, chosencelX, chosenCelY, chosenarr);
    printBoard(arr, revealedarr, rowsize, hightsize);
}

void print(int arr[30][30], int rowsize, int hightsize) {
    for (int i = 0; i < rowsize; i++) {
        if (rowsize - i <= 8)cout << (rowsize - i) << "  | ";
        else cout << rowsize - i << " | ";
        for (int j = 0; j < hightsize; ++j) {
            if (arr[i][j] <= 8 && arr[i][j] >= 0)
                cout << arr[i][j] << " ";
            else {
                cout << "* ";
            }
        }
        cout << endl;
    }
}

void printBoard(int arr[30][30], int revealedarr[30][30], int rowsize, int hightsize) {
    // system("cls");

    for (int i = 0; i < rowsize; i++) {
        if (rowsize - i <= 9)
            cout << rowsize - i << "  | ";
        else
            cout << rowsize - i << " | ";
        for (int j = 0; j < hightsize; ++j) {
            if (revealedarr[i][j] == 1) {
                if (arr[i][j] <= 8 && arr[i][j] >= 0)
                    cout << arr[i][j] << " ";
                else
                    cout << "* ";
            } else if (revealedarr[i][j] == 2)
                cout << "^ ";
            else cout << ". ";
        }
        cout << endl;
    }
}

int **generateArr(int rows, int hights) {
    int **arr = (int **) malloc(rows * sizeof(int));
    for (int i = 0; i < rows; i++)
        arr[i] = (int *) malloc(hights * sizeof(int));
    return arr;
}

void playGame() {
    gameBoard board;
    int command, row, hight, mines;
    cout << "difficulty : " << endl << "1. easy" << endl << "2. medium" << endl << "3. hard" << endl
         << "4. self made board" << endl;
    int dif;
    cin >> dif;
    if (dif == 1) {
        board.rows = 9;
        board.hight = 9;
        board.mines = 10;
    } else if (dif == 2) {
        board.rows = 16;
        board.hight = 16;
        board.mines = 40;
    } else if (dif == 3) {
        board.rows = 16;
        board.hight = 30;
        board.mines = 99;
    } else if (dif == 4) {
        cout << "row (maximum:30) : " << endl;
        cin >> board.rows;
        cout << "hight (maximum:30) : " << endl;
        cin >> board.hight;
        cout << "mines : " << endl;
        cin >> board.mines;
        while (mines >= board.hight * board.rows) {
            cout << "error too mich mine" << endl;
            cout << "mines : " << endl;
            cin >> board.mines;
        }
    } else
        cout << "ERROR INPUT  please enter one of the option you have .";
    placeZero(chosenarr, board.rows, board.hight);
    play(arr, revealedarr, board.rows, board.hight, board.mines, chosenarr);
}

void showleaderboard() {
    fstream leaderf("leaderboard.txt");
    player s;
    string str;
    cout << "user name  win  losses" << endl;
    leaderf >> s.username >> s.wins >> s.losses;
    cout << s.username << " " << s.wins << " " << s.losses << endl;
    while (getline(leaderf, str)) {
        leaderf >> s.username >> s.wins >> s.losses;
        cout << s.username << " " << s.wins << " " << s.losses << endl;
    }
    leaderf.close();
}

void showJadval() {
    cout << "user name  win  losses" << endl;
    for (int i = 1; i <= shomarande; ++i)
        cout << bazikon[i].username << " " << bazikon[i].wins << " " << bazikon[i].losses << endl;
}

int main() {
    srand(time(0));
    int command, row, hight, mines;
    do {
        cout << "1. play game" << endl << "2. leader board" << endl << "3. exit" << endl << "4. jadval emtiazat"
             << endl;
        cin >> command;
        if (command == 1) {
            playGame();
        } else if (command == 2) {
            showleaderboard();
        } else if (command == 3) {
            return 0;
        } else if (command == 4) {
            showJadval();
        } else
            cout << "ERROR INPUT  please enter one of the option you have .";
    } while (command != 3);
}