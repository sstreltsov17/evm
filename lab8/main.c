#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <x86intrin.h> // Для функции __rdtsc() (счётчик тактов процессора)
#include "func.h" 

int main(void) {
    srand((unsigned int)time(NULL));

    int nMin = 256;               // 1 КБ (256 элементов int)
    int nMax = 8 * 1024 * 1024;   // 32 МБ (8 388 608 элементов int)

    int *arr = (int *)malloc((size_t)nMax * sizeof(int));
    FILE *file = fopen("results.csv", "w");
    if (!arr || !file) {
        printf("Ошибка выделения памяти или открытия файла\n");
        free(arr);
        if (file) fclose(file);
        return 1;
    }

    fprintf(file, "SizeKB,Forward,Backward,Random\n");
    printf("Size (KB)   | Forward | Backward | Random\n");
    printf("------------------------------------------\n");

    int n = nMin;
    while (n <= nMax) {
        double sizeKB = (n * sizeof(int)) / 1024.0;

        fillForward(arr, n);
        double tForward = measureTime(arr, n);

        fillBackward(arr, n);
        double tBackward = measureTime(arr, n);

        fillRandom(arr, n);
        double tRandom = measureTime(arr, n);

        printf("%11.2f | %7.2f | %8.2f | %6.2f\n", sizeKB, tForward, tBackward, tRandom);
        fprintf(file, "%.2f,%.2f,%.2f,%.2f\n", sizeKB, tForward, tBackward, tRandom);

        // Увеличиваем размер массива на 20% на каждом шаге
        n = (int)(n * 1.2);
    }

    fclose(file);
    free(arr);
    printf("\nГотово! Таблица сохранена в results.csv\n");
    return 0;
}