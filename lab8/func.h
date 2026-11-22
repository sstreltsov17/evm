#ifndef FUNC_H
#define FUNC_H

// Заполнение массива в виде циклического списка для трёх видов обхода
void fillForward(int *arr, int n);
void fillBackward(int *arr, int n);
void fillRandom(int *arr, int n);

// Замер среднего времени чтения одного элемента (в тактах процессора)
double measureTime(const int *arr, int n);
#endif