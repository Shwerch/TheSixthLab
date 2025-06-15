#include "../../include/modules/matrix_replace.hpp"

#include <iostream>
#include <optional>
#include <random>

std::optional<std::vector<std::vector<int>>> generateRandomMatrix(int N) {
	if (N <= 0 || N % 2 != 0) {
		std::cerr << "Ошибка: N должно быть положительным четным числом." << std::endl;
		return std::nullopt;
	}

	std::vector<std::vector<int>> matrix(N, std::vector<int>(N));

	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution<> distrib(10, 30);

	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			matrix[i][j] = distrib(gen);
		}
	}
	return matrix;
}

void transformMatrix(std::vector<std::vector<int>> &matrix) {
	if (matrix.empty()) {
		return;
	}

	int N = matrix.size();
	if (N % 2 != 0) {
		std::cerr << "Ошибка: Матрица должна быть четного порядка для преобразования четвертей."
				  << std::endl;
		return;
	}

	int halfN = N / 2;

	// Левая нижняя четверть (строки [halfN, N-1], столбцы [0, halfN-1])
	for (int i = halfN; i < N; ++i) {
		for (int j = 0; j < halfN; ++j) {
			matrix[i][j] = 0;
		}
	}

	// Правая нижняя четверть (строки [halfN, N-1], столбцы [halfN, N-1])
	for (int i = halfN; i < N; ++i) {
		for (int j = halfN; j < N; ++j) {
			matrix[i][j] = 10;
		}
	}

	// Поменять местами верхние четверти: левую верхнюю с правой верхней
	// Левая верхняя: строки [0, halfN-1], столбцы [0, halfN-1]
	// Правая верхняя: строки [0, halfN-1], столбцы [halfN, N-1]
	for (int i = 0; i < halfN; ++i) {
		for (int j = 0; j < halfN; ++j) {
			std::swap(matrix[i][j], matrix[i][j + halfN]);
		}
	}
}

void printMatrix(const std::vector<std::vector<int>> &matrix) {
	if (matrix.empty()) {
		std::cout << "Матрица пуста." << std::endl;
		return;
	}

	for (const auto &row : matrix) {
		for (int val : row) {
			std::cout << val << "\t";
		}
		std::cout << std::endl;
	}
}

void runMatrixReplace() {
	int N;
	std::cout << "Введите порядок квадратной матрицы (N - четное число): ";

	while (!(std::cin >> N) || N <= 0 || N % 2 != 0) {
		std::cout << "Некорректный ввод. Пожалуйста, введите положительное четное число для N: ";
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	auto matrix_opt = generateRandomMatrix(N);

	if (matrix_opt) {
		std::vector<std::vector<int>> matrix = *matrix_opt;

		std::cout << "\nИсходная матрица:" << std::endl;
		printMatrix(matrix);

		transformMatrix(matrix);

		std::cout << "\nМатрица после преобразований:" << std::endl;
		printMatrix(matrix);
	} else {
		std::cerr << "Не удалось сгенерировать матрицу. Программа завершена." << std::endl;
		return;
	}
}
