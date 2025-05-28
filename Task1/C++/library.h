#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <limits>   // Для std::numeric_limits
#include <chrono>   // Для std::chrono::milliseconds
#include <thread>   // Для std::this_thread::sleep_for
#include <algorithm> // Для std::swap

// Проверяет ввод целого числа
int getValidIntegerInput(const std::string& promMsg);
// Выводит матрицу
void printMatrix(const std::vector<std::vector<int>>& matr);
// Задание 1: Работа с матрицей M x N
void taskOne();
// Задание 2: Работа с квадратной матрицей N x N
void taskTwo();

// Очищает консоль
void clearConsole();
// Инициализирует игровое поле для "Жизни"
std::vector<std::vector<int>> initializeGameField(int rows, int cols);
// Выводит игровое поле
void printGameField(const std::vector<std::vector<int>>& field);
// Подсчитывает живых соседей для клетки
int countLiveNeighbors(const std::vector<std::vector<int>>& field, int row, int col);
// Обновляет состояние игрового поля
void updateGameField(std::vector<std::vector<int>>& field);
// Задание 3: Реализация игры "Жизнь"
void taskThree();
// Инициализирует поле с "ружьем"
void initializeGun(std::vector<std::vector<int>>& field);
