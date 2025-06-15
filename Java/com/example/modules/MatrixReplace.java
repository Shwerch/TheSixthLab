package com.example.modules;

import com.example.input.InputUtils;
import java.util.Random;

public class MatrixReplace {

	public static void run() {
		int N = InputUtils.getInt(
			"Введите порядок квадратной матрицы (N - четное число)"
		);
		if (N <= 0 || N % 2 != 0) {
			System.err.println(
				"Ошибка: N должно быть положительным четным числом."
			);
			return;
		}

		int[][] matrix = generateRandomMatrix(N, 10, 30);
		System.out.println("\nИсходная матрица:");
		printMatrix(matrix);

		transformMatrix(matrix);
		System.out.println("\nМатрица после преобразований:");
		printMatrix(matrix);
	}

	private static int[][] generateRandomMatrix(int N, int minVal, int maxVal) {
		int[][] matrix = new int[N][N];
		Random random = new Random();
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				matrix[i][j] = random.nextInt(maxVal - minVal + 1) + minVal;
			}
		}
		return matrix;
	}

	private static void transformMatrix(int[][] matrix) {
		int n = matrix.length;
		int halfN = n / 2;

		// Левая нижняя -> 0; Правая нижняя -> 10
		for (int i = halfN; i < n; i++) {
			for (int j = 0; j < n; j++) {
				matrix[i][j] = (j < halfN) ? 0 : 10;
			}
		}

		// Поменять местами верхние четверти
		for (int i = 0; i < halfN; i++) {
			for (int j = 0; j < halfN; j++) {
				int temp = matrix[i][j];
				matrix[i][j] = matrix[i][j + halfN];
				matrix[i][j + halfN] = temp;
			}
		}
	}

	private static void printMatrix(int[][] matrix) {
		for (int[] row : matrix) {
			for (int val : row) {
				System.out.print(val + "\t");
			}
			System.out.println();
		}
	}
}
