#pragma once

#include <stdexcept>
#include <string_view>

enum class MainMenu {
	MATRIX_OPERATIONS,
	MATRIX_REPLACE,
	GAME_OF_LIFE,
	MAXIMISE_BANANAS,
	GAUSS_SOLVER,
	ITERATIVE_SOLVER,
	AES,
	EXIT,
};

constexpr int mainMenuElements = static_cast<int>(MainMenu::EXIT) + 1;

constexpr std::string_view getElementSescription(MainMenu element) {
	switch (element) {
	case MainMenu::MATRIX_OPERATIONS:
		return "Определение матрицы M на N псевдослучайными числами";
	case MainMenu::MATRIX_REPLACE:
		return "Определение матрицы N на N и перестановка значений в ней";
	case MainMenu::GAME_OF_LIFE:
		return "Игра 'Жизнь'";
	case MainMenu::MAXIMISE_BANANAS:
		return "Игра с бананами";
	case MainMenu::GAUSS_SOLVER:
		return "Решение системы уравнений методом Гаусса";
	case MainMenu::ITERATIVE_SOLVER:
		return "Решение системы уравнений приближенным методом";
	case MainMenu::AES:
		return "Алгоритм шифрования AES128 CBC";
	case MainMenu::EXIT:
		return "Выход из программы";
	default:
		throw std::runtime_error("Как ты это сделал");
	}
}

MainMenu convertIntToMainMenu(int element);

void printMainMenu();
