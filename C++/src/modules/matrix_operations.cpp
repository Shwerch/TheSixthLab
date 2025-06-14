#include "../../include/modules/matrix_operations.hpp"

#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <vector>

int get_safe_int_input(const std::string &prompt) {
	int value;
	while (true) {
		std::cout << prompt;
		std::cin >> value;
		if (std::cin.fail()) {
			std::cout << "Некорректный ввод. Пожалуйста, введите целое число.\n";
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		} else {
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
							'\n'); // Очистка буфера ввода
			return value;
		}
	}
}

std::vector<std::vector<int>> initialize_matrix(int rows, int cols, int min_val, int max_val) {
	std::vector<std::vector<int>> matrix(rows, std::vector<int>(cols));
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(min_val, max_val);

	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			matrix[i][j] = distrib(gen);
		}
	}
	return matrix;
}

void print_matrix(const std::vector<std::vector<int>> &matrix) {
	if (matrix.empty()) {
		std::cout << "Матрица пуста.\n";
		return;
	}
	for (const auto &row : matrix) {
		for (int val : row) {
			std::cout << val << "\t";
		}
		std::cout << "\n";
	}
}

std::vector<int> find_rows_with_min_negative(const std::vector<std::vector<int>> &matrix,
											 int min_negatives) {
	std::vector<int> row_indices;
	// Изменено: используем size_t для индекса, чтобы избежать ошибки sign-compare
	for (size_t i = 0; i < matrix.size(); ++i) {
		int negative_count = 0;
		for (int val : matrix[i]) {
			if (val < 0) {
				negative_count++;
			}
		}
		if (negative_count >= min_negatives) {
			row_indices.push_back(static_cast<int>(i));
		}
	}
	return row_indices;
}

std::vector<int> extract_negative_elements(const std::vector<std::vector<int>> &matrix,
										   const std::vector<int> &row_indices) {
	std::vector<int> negative_elements;
	for (int row_index : row_indices) {
		// Изменено: приводим row_index к size_t для сравнения с matrix.size()
		if (row_index >= 0 && static_cast<size_t>(row_index) < matrix.size()) {
			for (int val : matrix[row_index]) {
				if (val < 0) {
					negative_elements.push_back(val);
				}
			}
		}
	}
	return negative_elements;
}

std::vector<std::vector<int>>
create_new_matrix_from_rows(const std::vector<std::vector<int>> &original_matrix,
							const std::vector<int> &row_indices) {
	std::vector<std::vector<int>> new_matrix;
	for (int row_index : row_indices) {
		// Изменено: приводим row_index к size_t для сравнения с original_matrix.size()
		if (row_index >= 0 && static_cast<size_t>(row_index) < original_matrix.size()) {
			new_matrix.push_back(original_matrix[row_index]);
		}
	}
	return new_matrix;
}

void runMatrixOperations() {
	int M, N;

	// Ввод размеров матрицы с проверкой
	while (true) {
		M = get_safe_int_input("Введите количество строк M (M > 0): ");
		if (M > 0) {
			break;
		} else {
			std::cout << "Количество строк должно быть больше 0.\n";
		}
	}

	while (true) {
		N = get_safe_int_input("Введите количество столбцов N (N > 0): ");
		if (N > 0) {
			break;
		} else {
			std::cout << "Количество столбцов должно быть больше 0.\n";
		}
	}

	// Инициализация матрицы
	std::cout << "\nИнициализация матрицы " << M << "x" << N
			  << " случайными числами в диапазоне [-10, 40]:\n";
	std::vector<std::vector<int>> matrix = initialize_matrix(M, N, -10, 40);
	print_matrix(matrix);

	// Поиск строк, содержащих хотя бы три отрицательных элемента
	std::cout << "\nПоиск строк, содержащих хотя бы три отрицательных элемента...\n";
	std::vector<int> rows_with_negatives = find_rows_with_min_negative(matrix, 3);

	if (!rows_with_negatives.empty()) {
		std::cout << "Номера строк, содержащих хотя бы три отрицательных элемента: ";
		for (int row_index : rows_with_negatives) {
			std::cout << row_index + 1 << " ";
		}
		std::cout << "\n";

		// Инициализация нового массива отрицательными элементами из найденных строк
		std::vector<int> negative_elements_array =
			extract_negative_elements(matrix, rows_with_negatives);
		std::cout << "Новый массив, содержащий все отрицательные элементы из найденных строк: \n";
		if (!negative_elements_array.empty()) {
			for (int val : negative_elements_array) {
				std::cout << val << " ";
			}
			std::cout << "\n";
		} else {
			std::cout << "В найденных строках нет отрицательных элементов (это маловероятно, если "
						 "строки были найдены).\n";
		}

		// Создание новой матрицы из найденных строк
		std::vector<std::vector<int>> new_matrix =
			create_new_matrix_from_rows(matrix, rows_with_negatives);
		std::cout << "\nНовая матрица, состоящая из найденных строк:\n";
		print_matrix(new_matrix);

	} else {
		std::cout << "Строки, содержащие хотя бы три отрицательных элемента, не найдены.\n";
	}
}
