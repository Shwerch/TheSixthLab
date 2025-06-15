#include "../../include/modules/iterative_solver.hpp"
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <vector>

std::vector<double> solve(const std::vector<std::vector<double>> &matrix,
						  const std::vector<double> &solutions, double tolerance = 1e-3,
						  int max_iterations = 500) {
	const size_t n = matrix.size();
	if (n == 0 || matrix.at(0).size() != n || solutions.size() != n) {
		throw std::invalid_argument("Invalid matrix or vector dimensions.");
	}

	std::vector<std::vector<double>> Ap = {
		{matrix.at(0).at(3), matrix.at(0).at(1), matrix.at(0).at(2), matrix.at(0).at(0)},
		{matrix.at(3).at(3), matrix.at(3).at(1), matrix.at(3).at(2), matrix.at(3).at(0)},
		{matrix.at(2).at(3), matrix.at(2).at(1), matrix.at(2).at(2), matrix.at(2).at(0)},
		{matrix.at(1).at(3), matrix.at(1).at(1), matrix.at(1).at(2), matrix.at(1).at(0)}};

	std::vector<double> bp = {solutions.at(0), solutions.at(3), solutions.at(2), solutions.at(1)};

	std::vector<double> y(n, 0.0);

	for (int k = 0; k < max_iterations; ++k) {
		std::vector<double> y_prev = y;

		for (size_t i = 0; i < n; ++i) {
			double sigma = 0.0;
			for (size_t j = 0; j < n; ++j) {
				if (i != j) {
					sigma += Ap.at(i).at(j) * y.at(j);
				}
			}
			if (Ap.at(i).at(i) == 0.0) {
				throw std::runtime_error(
					"Метод не может быть применен, если диагональный элемент равен нулю");
			}
			y.at(i) = (bp.at(i) - sigma) / Ap.at(i).at(i);

			if (std::isnan(y.at(i)) || std::isinf(y.at(i))) {
				throw std::runtime_error("Метод расходится");
			}
		}

		double max_diff = 0.0;
		for (size_t i = 0; i < n; ++i) {
			max_diff = std::max(max_diff, std::abs(y.at(i) - y_prev.at(i)));
		}

		if (max_diff < tolerance) {
			std::vector<double> x(n);
			x.at(0) = y.at(3);
			x.at(1) = y.at(1);
			x.at(2) = y.at(2);
			x.at(3) = y.at(0);
			return x;
		}
	}

	throw std::runtime_error("Достигнут предел итераций");
}

void runIterativeSolver() {
	std::vector<std::vector<double>> matrix = {{0.91, -0.04, 0.21, -18.0},
											   {0.25, -1.23, -0.23, -0.09},
											   {-0.21, -0.23, 0.80, -0.13},
											   {0.15, -1.31, 0.06, -1.04}};

	std::vector<double> solutions = {-1.24, -1.04, 2.56, 0.91};

	try {
		std::vector<double> solution = solve(matrix, solutions, 1e-3);

		std::cout << "Решение найдено приближенным методом:" << std::endl;
		std::cout << std::fixed << std::setprecision(5);
		for (size_t i = 0; i < solution.size(); ++i) {
			std::cout << "x" << i + 1 << " = " << solution.at(i) << std::endl;
		}
	} catch (const std::exception &e) {
		std::cerr << "Ошибка: " << e.what() << std::endl;
	}
}
