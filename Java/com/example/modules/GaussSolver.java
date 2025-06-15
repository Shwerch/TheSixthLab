package com.example.modules;

import java.util.Arrays;

public class GaussSolver {

	public static double[] solve(double[][] matrix, double[] solutions) {
		final int n = matrix.length;
		if (n == 0 || matrix[0].length != n || solutions.length != n) {
			throw new IllegalArgumentException(
				"Invalid matrix or vector dimensions."
			);
		}

		// Создаем расширенную матрицу
		double[][] augmentedMatrix = new double[n][n + 1];
		for (int i = 0; i < n; i++) {
			System.arraycopy(matrix[i], 0, augmentedMatrix[i], 0, n);
			augmentedMatrix[i][n] = solutions[i];
		}

		// Прямой ход (Приведение к верхне-треугольному виду)
		for (int k = 0; k < n; k++) {
			// Поиск ведущего элемента (для стабильности)
			int pivotRow = k;
			for (int i = k + 1; i < n; i++) {
				if (
					Math.abs(augmentedMatrix[i][k]) >
					Math.abs(augmentedMatrix[pivotRow][k])
				) {
					pivotRow = i;
				}
			}

			// Обмен строк
			double[] temp = augmentedMatrix[k];
			augmentedMatrix[k] = augmentedMatrix[pivotRow];
			augmentedMatrix[pivotRow] = temp;

			if (Math.abs(augmentedMatrix[k][k]) < 1e-10) {
				throw new ArithmeticException(
					"Matrix is singular or nearly singular."
				);
			}

			for (int i = k + 1; i < n; i++) {
				double factor = augmentedMatrix[i][k] / augmentedMatrix[k][k];
				for (int j = k; j < n + 1; j++) {
					augmentedMatrix[i][j] -= factor * augmentedMatrix[k][j];
				}
			}
		}

		// Обратный ход
		double[] x = new double[n];
		for (int i = n - 1; i >= 0; i--) {
			double sum = 0.0;
			for (int j = i + 1; j < n; j++) {
				sum += augmentedMatrix[i][j] * x[j];
			}
			x[i] = (augmentedMatrix[i][n] - sum) / augmentedMatrix[i][i];
		}
		return x;
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
			double[] result = solve(matrix, solutions);
			System.out.println("Решение найдено методом Гаусса:");
			for (int i = 0; i < result.length; i++) {
				System.out.printf("x%d = %.5f%n", i + 1, result[i]);
			}
		} catch (Exception e) {
			System.err.println("Ошибка: " + e.getMessage());
		}
	}
}
