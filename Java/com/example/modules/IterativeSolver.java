package com.example.modules;

import java.util.Arrays;

public class IterativeSolver {

	public static double[] solve(
		double[][] matrix,
		double[] solutions,
		double tolerance,
		int maxIterations
	) {
		final int n = matrix.length;
		if (n == 0 || matrix[0].length != n || solutions.length != n) {
			throw new IllegalArgumentException(
				"Invalid matrix or vector dimensions."
			);
		}

		// Преобразование матрицы и вектора, как в C++ коде
		double[][] Ap = {
			{ matrix[0][3], matrix[0][1], matrix[0][2], matrix[0][0] },
			{ matrix[3][3], matrix[3][1], matrix[3][2], matrix[3][0] },
			{ matrix[2][3], matrix[2][1], matrix[2][2], matrix[2][0] },
			{ matrix[1][3], matrix[1][1], matrix[1][2], matrix[1][0] },
		};
		double[] bp = {
			solutions[0],
			solutions[3],
			solutions[2],
			solutions[1],
		};

		double[] y = new double[n];
		Arrays.fill(y, 0.0);

		for (int k = 0; k < maxIterations; k++) {
			double[] y_prev = Arrays.copyOf(y, n);

			for (int i = 0; i < n; i++) {
				double sigma = 0.0;
				for (int j = 0; j < n; j++) {
					if (i != j) {
						sigma += Ap[i][j] * y_prev[j]; // Используем значения с предыдущей итерации
					}
				}
				if (Math.abs(Ap[i][i]) < 1e-10) {
					throw new ArithmeticException(
						"Метод не может быть применен, если диагональный элемент равен нулю."
					);
				}
				y[i] = (bp[i] - sigma) / Ap[i][i];

				if (Double.isNaN(y[i]) || Double.isInfinite(y[i])) {
					throw new ArithmeticException("Метод расходится.");
				}
			}

			double maxDiff = 0.0;
			for (int i = 0; i < n; i++) {
				maxDiff = Math.max(maxDiff, Math.abs(y[i] - y_prev[i]));
			}

			if (maxDiff < tolerance) {
				// Восстанавливаем исходный порядок переменных
				return new double[] { y[3], y[1], y[2], y[0] };
			}
		}
		throw new RuntimeException(
			"Достигнут предел итераций, решение не найдено."
		);
	}

	public static void run() {
		double[][] matrix = {
			{ 0.91, -0.04, 0.21, -18.0 },
			{ 0.25, -1.23, -0.23, -0.09 },
			{ -0.21, -0.23, 0.80, -0.13 },
			{ 0.15, -1.31, 0.06, -1.04 },
		};
		double[] solutions = { -1.24, -1.04, 2.56, 0.91 };

		try {
			double[] result = solve(matrix, solutions, 1e-3, 500);
			System.out.println("Решение найдено приближенным методом:");
			for (int i = 0; i < result.length; i++) {
				System.out.printf("x%d = %.5f%n", i + 1, result[i]);
			}
		} catch (Exception e) {
			System.err.println("Ошибка: " + e.getMessage());
		}
	}
}
