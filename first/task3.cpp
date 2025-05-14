#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>

using namespace std;

#define CLEAR_COMMAND "clear"

const int WIDTH = 100;
const int HEIGHT = 50;
const char ALIVE = '@';
const char DEAD = ' ';

using Grid = vector<vector<bool>>;

void printGrid(const Grid& grid) {
    system(CLEAR_COMMAND);
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            cout << (grid[y][x] ? ALIVE : DEAD);
        }
        cout << '\n';
    }
    cout << flush;
}

int countNeighbors(const Grid& grid, int x, int y) {
    int count = 0;
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (dx == 0 && dy == 0) continue;
            int nx = (x + dx + WIDTH) % WIDTH;
            int ny = (y + dy + HEIGHT) % HEIGHT;
            count += grid[ny][nx];
        }
    }
    return count;
}

Grid nextGeneration(const Grid& current) {
    Grid next = current;
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            int neighbors = countNeighbors(current, x, y);
            next[y][x] = (current[y][x] ? (neighbors == 2 || neighbors == 3) : (neighbors == 3));
        }
    }
    return next;
}

void addGlider(Grid& grid, int x, int y) {
    // Классический глайдер (лучше видимый)
    vector<pair<int, int>> points = {
        {1, 0}, {2, 1}, {0, 2}, {1, 2}, {2, 2}
    };
    
    for (auto [dx, dy] : points) {
        int nx = (x + dx) % WIDTH;
        int ny = (y + dy) % HEIGHT;
        grid[ny][nx] = true;
    }
}

void randomInit(Grid& grid, int densityPercent = 90) {
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            if (!grid[y][x]) { // Не перезаписываем глайдер
                grid[y][x] = (rand() % 100 < densityPercent);
            }
        }
    }
}

int main() {
    srand(time(nullptr));
    Grid grid(HEIGHT, vector<bool>(WIDTH, false));

    // Яркий глайдер в центре
    addGlider(grid, WIDTH/2, HEIGHT/2);
    
    // Умеренное количество случайных клеток
    randomInit(grid, 15);

    while (true) {
        printGrid(grid);
        grid = nextGeneration(grid);
        this_thread::sleep_for(chrono::milliseconds(100));
    }

    return 0;
}