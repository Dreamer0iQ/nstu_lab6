#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

const int N = 4;
const double EPS = 1e-3;

typedef vector<double> Vec;
typedef vector<Vec> Mat;

void printSystem(const Mat& A, const Vec& b) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j)
            cout << setw(10) << A[i][j] << " ";
        cout << " = " << b[i] << endl;
    }
    cout << endl;
}

// Метод Гаусса с выбором главного элемента
Vec gaussMain(Mat A, Vec b) {
    for (int i = 0; i < N; ++i) {
        int maxRow = i;
        for (int k = i + 1; k < N; ++k) {
            if (fabs(A[k][i]) > fabs(A[maxRow][i]))
                maxRow = k;
        }
        swap(A[i], A[maxRow]);
        swap(b[i], b[maxRow]);

        for (int k = i + 1; k < N; ++k) {
            double factor = A[k][i] / A[i][i];
            for (int j = i; j < N; ++j)
                A[k][j] -= factor * A[i][j];
            b[k] -= factor * b[i];
        }
    }

    Vec x(N);
    for (int i = N - 1; i >= 0; --i) {
        x[i] = b[i];
        for (int j = i + 1; j < N; ++j)
            x[i] -= A[i][j] * x[j];
        x[i] /= A[i][i];
    }
    return x;
}

// Метод простой итерации
Vec simpleIteration(const Mat& A, const Vec& b) {
    Vec x(N, 0.0), prev(N);
    Mat C(N, Vec(N));
    Vec f(N);

    for (int i = 0; i < N; ++i) {
        f[i] = b[i] / A[i][i];
        for (int j = 0; j < N; ++j) {
            if (i != j)
                C[i][j] = -A[i][j] / A[i][i];
        }
    }

    cout << "\nИтерации метода простой итерации (ε = 1e-3):\n";
    cout << "N\t";
    for (int i = 0; i < N; ++i) cout << "x" << i + 1 << "\t\t";
    cout << "εn" << endl;

    int iter = 0;
    while (true) {
        prev = x;
        double maxDiff = 0.0;

        for (int i = 0; i < N; ++i) {
            x[i] = f[i];
            for (int j = 0; j < N; ++j)
                x[i] += C[i][j] * prev[j];
            maxDiff = max(maxDiff, fabs(x[i] - prev[i]));
        }

        cout << iter << "\t";
        for (double xi : x) cout << fixed << setprecision(6) << xi << "\t";
        cout << maxDiff << endl;

        if (maxDiff < EPS) break;
        iter++;
    }

    return x;
}

int main() {
    Mat A = {
        { 0.25, -1.23, -0.16, -0.09 }, // бывшая строка 1
        {-0.21, -0.16,  0.8,  -0.13 }, // бывшая строка 2
        { 0.15, -1.31,  0.06,  0.84 }, // бывшая строка 3
        { 1.09, -0.04,  0.21, -18   }  // бывшая строка 0
    };

    Vec b = {
        0.84,   // b1
        2.56,   // b2
        1.09,   // b3
        -1.24    // b0
    };

    cout << "Исходная система:" << endl;
    printSystem(A, b);

    cout << "Решение методом Гаусса с выбором главного элемента:" << endl;
    Vec gaussResult = gaussMain(A, b);
    for (int i = 0; i < N; ++i)
        cout << "x" << i + 1 << " = " << fixed << setprecision(6) << gaussResult[i] << endl;

    Vec iterResult = simpleIteration(A, b);

    return 0;
}



    // Mat A = {
    //     { 0.0,    0.0367, -0.1927, 16.5138 },  // коэффициенты для x1
    //     { -0.2033, 0.0,    0.1301,  0.0732  },  // коэффициенты для x2
    //     { 0.2625,  0.2,    0.0,     0.1625  },  // коэффициенты для x3
    //     { -0.1786, 1.5595, -0.0714, 0.0     }   // коэффициенты для x4
    // };

    // Vec b = {
    //     -1.1376,  // свободный член для x1
    //     -0.6829,  // свободный член для x2
    //     3.2,      // свободный член для x3
    //     1.2976    // свободный член для x4
    // };
    


    // Mat A = {
    //     { 4.12, -0.43,  0.54,  0.11 },
    //     { 0.32,  5.22, -0.17,  0.25 },
    //     { -0.12, 0.34,  3.76, -0.18 },
    //     { 0.05, -0.12,  0.23,  5.11 }
    // };

    // Vec b = {
    //     1.54,
    //     -2.12,
    //     3.17,
    //     -0.98
    // };