#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>

using namespace std;

bool compareDescending(const std::pair<int, int>& a, const std::pair<int, int>& b) {
    return a.first > b.first; // сортируем по убыванию суммы
}

int main() {
    int N;
    cout << "Введите порядок квадратной матрицы (N): ";
    cin >> N;

    if (N <= 0) {
        cout << "Порядок матрицы должен быть положительным числом.\n";
        return 1;
    }

    vector<vector<int>> matrix(N, vector<int>(N));
    vector<int> rowSums(N);
    vector<pair<int, int>> sumWithIndex; // (сумма, индекс строки)

    srand(static_cast<unsigned>(time(nullptr)));

    // Заполнение и вывод матрицы
    cout << "\nСгенерированная матрица:\n";
    for (int i = 0; i < N; ++i) {
        int sum = 0;
        for (int j = 0; j < N; ++j) {
            matrix[i][j] = rand() % 101 + 100; // от 100 до 200
            sum += matrix[i][j];
            cout << matrix[i][j] << "\t";
        }
        rowSums[i] = sum;
        sumWithIndex.push_back({sum, i});
        cout << "(Сумма: " << sum << ")\n";
    }

    // Сортировка по убыванию суммы
    sort(sumWithIndex.begin(), sumWithIndex.end(), compareDescending);

    // Получаем индекс строки со второй максимальной суммой
    int secondMaxIndex = -1;
    if (N > 1) {
        secondMaxIndex = sumWithIndex[1].second;
    } else {
        cout << "\nНедостаточно строк для определения второй максимальной суммы.\n";
        return 1;
    }

    // Получение строки с нужным индексом
    vector<int> secondMaxRow = matrix[secondMaxIndex];

    // Вывод результата
    cout << "\nСтрока с второй по величине суммой элементов (" << rowSums[secondMaxIndex] << "): ";
    for (int val : secondMaxRow) {
        cout << val << " ";
    }
    cout << "\n";

    return 0;
}