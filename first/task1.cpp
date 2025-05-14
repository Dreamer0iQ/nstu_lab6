#include <iostream>
#include <ctime>
#include <vector>

using namespace std;

int main(){
    int M, N;
    cout << "Введите количество строк (M): ";
    cin >> M;
    cout << "Введите количество столбцов (N): ";
    cin >> N;

    if (M <= 0 || N <= 0) {
        cout << "Размеры матрицы должны быть положительными числами.\n";
        return 1;
    }

    vector<vector<int>> matrix(M, vector<int>(N));
    vector<int> result;
    int sum = 0;

    srand(static_cast<unsigned>(time(nullptr)));


    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            matrix[i][j] = rand() % 101 - 50;
            cout << matrix[i][j] << "\t";
        }
        cout << "\n";
    }

    // Поиск и суммирование неотрицательных чисел в чётных строках на нечётных местах
    for (int i = 1; i < M; i += 2) {
        for (int j = 0; j < N; j+=2) {
            if (matrix[i][j] >= 0) {
                sum += matrix[i][j];
                result.push_back(matrix[i][j]);
            }
        }
    }
    cout << "\nСумма неотрицательных элементов в чётных строках на нечётных местах: " << sum << "\n";
    cout << "Новый массив из этих элементов: ";
    for (int val : result) {
        cout << val << " ";
    }
    cout << "\n";

    return 0;
}