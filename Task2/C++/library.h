#pragma once

#include <algorithm>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <vector>

// Таблица S-блока для AES
extern const uint8_t sBox[256];
// Таблица обратного S-блока для AES
extern const uint8_t invSBox[256];
// Таблица для MixColumns
extern const uint8_t gf28MulX[256];
// Таблица для InvMixColumns
extern const uint8_t gf28MulXInv[256];

// Структура для представления состояния AES (4x4 матрица байтов)
struct State {
	uint8_t matrix[4][4];
};

// Вычисляет R_con для заданного раунда
uint8_t getRcon(uint8_t round);

// Преобразует вектор байтов в матрицу состояния
State bytesToMatrix(const std::vector<uint8_t> &bytes);

// Преобразует матрицу состояния в вектор байтов
std::vector<uint8_t> matrixToBytes(const State &state);

// Выполняет побайтовое замещение с использованием S-блока
void subBytes(State &state);

// Выполняет обратное побайтовое замещение с использованием обратного S-блока
void invSubBytes(State &state);

// Выполняет циклический сдвиг строк
void shiftRows(State &state);

// Выполняет обратный циклический сдвиг строк
void invShiftRows(State &state);

// Умножение в поле GF(2^8)
uint8_t gfMul(uint8_t a, uint8_t b);

// Перемешивает столбцы
void mixColumns(State &state);

// Выполняет обратное перемешивание столбцов
void invMixColumns(State &state);

// Добавляет раундовый ключ к состоянию
void addRoundKey(State &state, const std::vector<uint8_t> &roundKey);

// Расширяет 128-битный ключ до всех раундовых ключей
std::vector<std::vector<uint8_t>> expandKey(const std::vector<uint8_t> &key);

// Выводит байты в шестнадцатеричном формате
void printBytes(const std::vector<uint8_t> &bytes);

// Выводит матрицу состояния
void printState(const State &state);

// Генерирует случайный ключ заданной длины
std::vector<uint8_t> generateRandomKey(size_t length);

// Генерирует случайный вектор инициализации заданной длины
std::vector<uint8_t> generateRandomIV(size_t length);

// Записывает ключ в файл
void writeKeyToFile(const std::string &filename, const std::vector<uint8_t> &key);

// Читает ключ из файла
std::vector<uint8_t> readKeyFromFile(const std::string &filename);

// Выполняет шифрование одного 128-битного блока AES
std::vector<uint8_t> aes128EncryptBlock(const std::vector<uint8_t> &block,
										const std::vector<uint8_t> &key);

// Выполняет расшифрование одного 128-битного блока AES
std::vector<uint8_t> aes128DecryptBlock(const std::vector<uint8_t> &block,
										const std::vector<uint8_t> &key);

// Выполняет шифрование AES-128 в режиме CBC
std::vector<uint8_t> aes128EncryptCbc(const std::vector<uint8_t> &plaintext,
									  const std::vector<uint8_t> &key,
									  const std::vector<uint8_t> &iv);

// Выполняет расшифрование AES-128 в режиме CBC
std::vector<uint8_t> aes128DecryptCbc(const std::vector<uint8_t> &ciphertext,
									  const std::vector<uint8_t> &key,
									  const std::vector<uint8_t> &iv);
