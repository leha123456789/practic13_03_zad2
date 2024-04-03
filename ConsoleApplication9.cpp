#include <iostream>
#include <omp.h>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
using namespace std;
const int SIZE = 100;
const int NUM_THREADS = 4;
void initialize(bool grid[][SIZE]) 
{
 
    srand(time(NULL));
    for (int i = 0; i < SIZE; ++i) 
    {
        for (int j = 0; j < SIZE; ++j) 
        {
            grid[i][j] = rand() % 2;
        }
    }
}

void printGrid(bool grid[][SIZE]) 
{
    for (int i = 0; i < SIZE; ++i) 
    {
        for (int j = 0; j < SIZE; ++j) 
        {
            cout << (grid[i][j] ? "*" : " ");
        }
        cout << endl;
    }
}

void update(bool currentGen[][SIZE], bool nextGen[][SIZE], int start, int end) 
{
    for (int i = start; i < end; ++i) 
    {
        for (int j = 0; j < SIZE; ++j) 
        {
            int neighbors = 0;
            for (int x = -1; x <= 1; ++x) 
            {
                for (int y = -1; y <= 1; ++y) 
                {
                    if (x == 0 && y == 0) continue;
                    int ni = (i + x + SIZE) % SIZE;
                    int nj = (j + y + SIZE) % SIZE;
                    if (currentGen[ni][nj]) 
                    {
                        ++neighbors;
                    }
                }
            }
            if (currentGen[i][j]) 
            {
                nextGen[i][j] = (neighbors == 2 || neighbors == 3);
            }
            else 
            {
                nextGen[i][j] = (neighbors == 3);
            }
        }
    }
}

int main()
{
    setlocale(LC_ALL, "Russian");
    bool currentGeneration[SIZE][SIZE];
    bool nextGeneration[SIZE][SIZE];
    initialize(currentGeneration);
    printGrid(currentGeneration);
    omp_set_num_threads(NUM_THREADS); 
    for (int generation = 0; generation < 100; ++generation) 
    {
        auto start = chrono::steady_clock::now(); 
#pragma omp parallel for schedule(static)
        for (int i = 0; i < SIZE; ++i) 
        {
            update(currentGeneration, nextGeneration, i, i + 1);
        }
#pragma omp parallel for schedule(static)
        for (int i = 0; i < SIZE; ++i) 
        {
            for (int j = 0; j < SIZE; ++j) 
            {
                currentGeneration[i][j] = nextGeneration[i][j];
            }
        }
        auto end = chrono::steady_clock::now();
        auto diff = end - start;
        cout << "Генерация " << generation + 1 << " длилась " << chrono::duration <double, milli>(diff).count() << " мс" << endl;
        this_thread::sleep_for(chrono::milliseconds(100));
        cout << "Генерация № " << generation + 1 << ":" << endl;
        printGrid(currentGeneration);
    }

    return 0;
}
