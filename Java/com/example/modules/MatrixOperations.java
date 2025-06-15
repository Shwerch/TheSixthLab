package com.example.modules;

import com.example.input.InputUtils;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class MatrixOperations {

	public static void run() {
		int M = InputUtils.getInt("Введите количество строк M (M > 0)");
		int N = InputUtils.getInt("Введите количество столбцов N (N > 0)");
		if (M <= 0 || N <= 0) {
			System.err.println("Размеры должны быть положительными.");
			return;
		}

		System.out.println(
			"\nИнициализация матрицы " +
			M +
			"x" +
			N +
			" случайными числами в диапазоне [-10, 40]:"
		);
		int[][] matrix = initializeMatrix(M, N, -10, 40);
		printMatrix(matrix);

		System.out.println(
			"\nПоиск строк, содержащих хотя бы три отрицательных элемента..."
		);
		List<Integer> rowIndices = findRowsWithMinNegative(matrix, 3);

		if (!rowIndices.isEmpty()) {
			System.out.print(
				"Номера строк, содержащих хотя бы три отрицательных элемента: "
			);
			rowIndices.forEach(index -> System.out.print((index + 1) + " "));
			System.out.println();

			List<Integer> negativeElements = extractNegativeElements(
				matrix,
				rowIndices
			);
			System.out.println(
				"Новый массив отрицательных элементов из найденных строк:"
			);
			negativeElements.forEach(val -> System.out.print(val + " "));
			System.out.println("\n");

			int[][] newMatrix = createNewMatrixFromRows(matrix, rowIndices);
			System.out.println("Новая матрица, состоящая из найденных строк:");
			printMatrix(newMatrix);
		} else {
			System.out.println(
				"Строки, содержащие хотя бы три отрицательных элемента, не найдены."
			);
		}
	}

	private static int[][] initializeMatrix(
		int rows,
		int cols,
		int minVal,
		int maxVal
	) {
		int[][] matrix = new int[rows][cols];
		Random random = new Random();
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				matrix[i][j] = random.nextInt(maxVal - minVal + 1) + minVal;
			}
		}
		return matrix;
	}

	private static List<Integer> findRowsWithMinNegative(
		int[][] matrix,
		int minNegatives
	) {
		List<Integer> indices = new ArrayList<>();
		for (int i = 0; i < matrix.length; i++) {
			long negativeCount = 0;
			for (int val : matrix[i]) {
				if (val < 0) {
					negativeCount++;
				}
			}
			if (negativeCount >= minNegatives) {
				indices.add(i);
			}
		}
		return indices;
	}

	private static List<Integer> extractNegativeElements(
		int[][] matrix,
		List<Integer> rowIndices
	) {
		List<Integer> elements = new ArrayList<>();
		for (int rowIndex : rowIndices) {
			for (int val : matrix[rowIndex]) {
				if (val < 0) {
					elements.add(val);
				}
			}
		}
		return elements;
	}

	private static int[][] createNewMatrixFromRows(
		int[][] original,
		List<Integer> rowIndices
	) {
		int[][] newMatrix = new int[rowIndices.size()][];
		for (int i = 0; i < rowIndices.size(); i++) {
			newMatrix[i] = original[rowIndices.get(i)];
		}
		return newMatrix;
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
