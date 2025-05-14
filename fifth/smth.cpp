#include <iostream>
#include <vector>
#include <cstdlib>  
#include <ctime>   

using namespace std;

typedef vector<vector<int>> Matrix;

// случайной доски
Matrix generateRandomBoard(int n, int m) {
    Matrix board(n, vector<int>(m));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            board[i][j] = rand() % 2;  // 0 или 1
    return board;
}

// Функция генерации эталонной шахматной доски
Matrix generateChessBoard(int n, int m, int start) {
    Matrix board(n, vector<int>(m));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            board[i][j] = (i + j + start) % 2;
    return board;
}

// Подсчет различий между двумя досками
int countDifferences(const Matrix& a, const Matrix& b) {
    int diff = 0;
    for (int i = 0; i < a.size(); ++i)
        for (int j = 0; j < a[0].size(); ++j)
            if (a[i][j] != b[i][j]) diff++;
    return diff;
}

// Печать доски
void printBoard(const Matrix& board) {
    for (const auto& row : board) {
        for (int cell : row)
            cout << cell << " ";
        cout << endl;
    }
}

int main() {
    srand(time(0)); // Инициализация генератора случайных чисел

    int n, m;
    cin >> n >> m;
    Matrix board = generateRandomBoard(n, m);

    cout << "Исходная доска:" << endl;
    printBoard(board);

    Matrix chess0 = generateChessBoard(n, m, 0);  // старт с 0
    Matrix chess1 = generateChessBoard(n, m, 1);  // старт с 1

    int diff0 = countDifferences(board, chess0);
    int diff1 = countDifferences(board, chess1);

    int result = -1;
    if (diff0 % 2 == 0)
        result = diff0 / 2;
    if (diff1 % 2 == 0)
        result = (result == -1) ? diff1 / 2 : min(result, diff1 / 2);

    if (result == -1)
        cout << "Невозможно привести к шахматному порядку." << endl;
    else
        cout << "Минимальное количество ходов: " << result << endl;

    return 0;
}