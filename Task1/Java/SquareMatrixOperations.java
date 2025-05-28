import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class SquareMatrixOperations {

	/**
	 * Task 2: Performs operations on a square N x N matrix.
	 * - Initializes a square matrix with random numbers.
	 * - Replaces values in the bottom-left quarter with 0.
	 * - Replaces values in the bottom-right quarter with 10.
	 * - Swaps the top-left and top-right quarters.
	 */
	public static void taskTwo() {
		int orderN = InputUtils.getValidIntegerInput(
			"Введите порядок N квадратной матрицы (четное целое число > 0): "
		);
		while (orderN <= 0 || orderN % 2 != 0) {
			System.out.println(
				"Порядок N должен быть четным положительным числом."
			);
			orderN = InputUtils.getValidIntegerInput(
				"Введите порядок N квадратной матрицы (четное целое число > 0): "
			);
		}

		List<List<Integer>> squareMatrix = new ArrayList<>();
		Random random = new Random();

		// Initialize square matrix with random numbers
		for (int i = 0; i < orderN; ++i) {
			List<Integer> row = new ArrayList<>();
			for (int j = 0; j < orderN; ++j) {
				row.add(random.nextInt(21) + 10); // Numbers between 10 and 30
			}
			squareMatrix.add(row);
		}

		System.out.println("Исходная квадратная матрица:");
		MatrixUtils.printMatrix(squareMatrix);

		int halfN = orderN / 2;

		// Replace values in the bottom-left quarter with 0, bottom-right with 10
		for (int i = halfN; i < orderN; ++i) {
			for (int j = 0; j < halfN; ++j) {
				squareMatrix.get(i).set(j, 0); // Bottom-left quarter
			}
			for (int j = halfN; j < orderN; ++j) {
				squareMatrix.get(i).set(j, 10); // Bottom-right quarter
			}
		}

		// Swap remaining quarters (top-left with top-right)
		for (int i = 0; i < halfN; ++i) {
			for (int j = 0; j < halfN; ++j) {
				// Swap element (i, j) of top-left with (i, j + halfN) of top-right
				int temp = squareMatrix.get(i).get(j);
				squareMatrix.get(i).set(j, squareMatrix.get(i).get(j + halfN));
				squareMatrix.get(i).set(j + halfN, temp);
			}
		}

		System.out.println("Преобразованная квадратная матрица:");
		MatrixUtils.printMatrix(squareMatrix);
	}
}
