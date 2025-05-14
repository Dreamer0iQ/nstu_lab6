#include <iostream>
#include <algorithm>

using namespace std;

int main() {
    long long n, m, t;
    cin >> n >> m >> t;

    long long max_k = min((n - 1) / 2, (m - 1) / 2);    // плитки дорожки
    long long low = 0, high = max_k;
    long long best_k = 0;

    while (low <= high) {
        long long mid = low + (high - low) / 2;
        long long required = 2 * mid * (n + m - 2 * mid);

        if (required <= t) {
            best_k = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    cout << best_k << endl;

    return 0;
}