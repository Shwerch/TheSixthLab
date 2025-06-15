#include "../../include/modules/gauss_solver.hpp"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <vector>

std::vector<double> solve(std::vector<std::vector<double>> matrix,
						  const std::vector<double> &solutions) {
	const size_t n = matrix.size();
	if (n == 0 || matrix.at(0).size() != n || solutions.size() != n) {
		throw std::invalid_argument("Invalid matrix or vector dimensions.");
	}

	for (size_t i = 0; i < n; ++i) {
		matrix.at(i).push_back(solutions.at(i));
	}

	for (size_t k = 0; k < n; ++k) {
		size_t pivot_row = k;
		for (size_t i = k + 1; i < n; ++i) {
			if (std::abs(matrix.at(i).at(k)) > std::abs(matrix.at(pivot_row).at(k))) {
				pivot_row = i;
			}
		}

		if (pivot_row != k) {
			std::swap(matrix.at(k), matrix.at(pivot_row));
		}

		if (std::abs(matrix.at(k).at(k)) < 1e-10) {
			throw std::runtime_error("Matrix is singular or nearly singular.");
		}

		for (size_t i = k + 1; i < n; ++i) {
			const double factor = matrix.at(i).at(k) / matrix.at(k).at(k);
			for (size_t j = k; j < n + 1; ++j) {
				matrix.at(i).at(j) -= factor * matrix.at(k).at(j);
			}
		}
	}

	std::vector<double> x(n);
	for (int i = n - 1; i >= 0; --i) {
		double sum = 0.0;
		for (size_t j = i + 1; j < n; ++j) {
			sum += matrix.at(i).at(j) * x.at(j);
		}
		x.at(i) = (matrix.at(i).at(n) - sum) / matrix.at(i).at(i);
	}

	return x;
}

void runGaussSolver() {
	std::vector<std::vector<double>> matrix = {{0.91, -0.04, 0.21, -18.0},
											   {0.25, -1.23, -0.23, -0.09},
											   {-0.21, -0.23, 0.80, -0.13},
											   {0.15, -1.31, 0.06, -1.04}};

	std::vector<double> solutions = {-1.24, -1.04, 2.56, 0.91};

	try {
		std::vector<double> x = solve(matrix, solutions);

		std::cout << "Решение найдено методом Гаусса:" << std::endl;
		std::cout << std::fixed << std::setprecision(5);
		for (size_t i = 0; i < x.size(); ++i) {
			std::cout << "x" << i + 1 << " = " << x.at(i) << std::endl;
		}
	} catch (const std::exception &e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return;
	}
}
