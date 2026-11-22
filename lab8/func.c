#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <x86intrin.h> // Для функции __rdtsc() (счётчик тактов процессора)

// 1. Прямой обход: 0 -> 1 -> 2 -> ... -> N-1 -> 0
void fillForward(int *arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        arr[i] = i + 1;
    }
    arr[n - 1] = 0;
}

// 2. Обратный обход: 0 -> N-1 -> N-2 -> ... -> 1 -> 0
void fillBackward(int *arr, int n) {
    arr[0] = n - 1;
    for (int i = 1; i < n; i++) {
        arr[i] = i - 1;
    }
}

// 3. Случайный обход (единый цикл по всем элементам)
void fillRandom(int *arr, int n) {
    int *perm = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        perm[i] = i;
    }

    // Перемешиваем индексы случайным образом
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = perm[i];
        perm[i] = perm[j];
        perm[j] = temp;
    }

    // Связываем элементы в один общий замкнутый круг
    for (int i = 0; i < n - 1; i++) {
        arr[perm[i]] = perm[i + 1];
    }
    arr[perm[n - 1]] = perm[0];

    free(perm);
}

// Замер среднего времени чтения одного элемента (в тактах)
double measureTime(const int *arr, int n) {
    // Чтобы замер был точным, делаем не меньше 2 млн шагов
    long long passes = 2000000 / n;
    if (passes < 5) {
        passes = 5;
    }
    long long totalSteps = (long long)n * passes;

    // Примечание 2: однократный проход для «прогрева» кэша
    int k = 0;
    for (int i = 0; i < n; i++) {
        k = arr[k];
    }

    // Примечание 3: делаем 3 замера и берём минимальный (без помех от ОС)
    unsigned long long minCycles = -1; // Максимально возможное число
    for (int run = 0; run < 3; run++) {
        k = 0;
        unsigned long long start = __rdtsc();
        for (long long i = 0; i < totalSteps; i++) {
            k = arr[k];
        }
        unsigned long long end = __rdtsc();

        // Чтобы компилятор с -O1 не выкинул пустой цикл
        volatile int dummy = k;
        (void)dummy;

        if (end - start < minCycles) {
            minCycles = end - start;
        }
    }

    return (double)minCycles / totalSteps;
}