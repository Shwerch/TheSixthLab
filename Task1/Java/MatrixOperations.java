import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class MatrixOperations {

	/**
	 * Task 1: Performs operations on an M x N matrix.
	 * - Initializes a matrix with random numbers.
	 * - Identifies rows with at least three negative elements.
	 * - Creates a new array of negative elements from identified rows.
	 * - Creates a new matrix from identified rows.
	 */
	public static void taskOne() {
		int numRows = InputUtils.getValidIntegerInput(
			"Введите количество строк M (целое число > 0): "
		);
		while (numRows <= 0) {
			System.out.println("Количество строк должно быть больше 0.");
			numRows = InputUtils.getValidIntegerInput(
				"Введите количество строк M (целое число > 0): "
			);
		}

		int numCols = InputUtils.getValidIntegerInput(
			"Введите количество столбцов N (целое число > 0): "
		);
		while (numCols <= 0) {
			System.out.println("Количество столбцов должно быть больше 0.");
			numCols = InputUtils.getValidIntegerInput(
				"Введите количество столбцов N (целое число > 0): "
			);
		}

		List<List<Integer>> initialMatrix = new ArrayList<>();
		Random random = new Random();

		// Initialize matrix with random numbers
		for (int i = 0; i < numRows; ++i) {
			List<Integer> row = new ArrayList<>();
			for (int j = 0; j < numCols; ++j) {
				row.add(random.nextInt(51) - 10); // Numbers between -10 and 40
			}
			initialMatrix.add(row);
		}

		System.out.println("Исходная матрица M x N:");
		MatrixUtils.printMatrix(initialMatrix);

		List<Integer> negativeElements = new ArrayList<>();
		List<List<Integer>> newMatrix = new ArrayList<>();

		System.out.println(
			"Номера строк, содержащих не менее трех отрицательных элементов:"
		);
		for (int i = 0; i < numRows; ++i) {
			int negCount = 0;
			for (int elem : initialMatrix.get(i)) {
				if (elem < 0) {
					negCount++;
				}
			}
			if (negCount >= 3) {
				System.out.println("Строка " + i);
				// Initialize new array with negative elements
				for (int elem : initialMatrix.get(i)) {
					if (elem < 0) {
						negativeElements.add(elem);
					}
				}
				// Create new matrix from found rows
				newMatrix.add(initialMatrix.get(i));
			}
		}

		System.out.println(
			"Новый массив с отрицательными элементами из найденных строк:"
		);
		for (int elem : negativeElements) {
			System.out.print(elem + " ");
		}
		System.out.println();

		if (!newMatrix.isEmpty()) {
			System.out.println("Новая матрица, состоящая из найденных строк:");
			MatrixUtils.printMatrix(newMatrix);
		} else {
			System.out.println(
				"Новых строк с тремя и более отрицательными элементами не найдено."
			);
		}
	}
}
