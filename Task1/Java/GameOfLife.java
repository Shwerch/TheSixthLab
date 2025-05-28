import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Random;

public class GameOfLife {

	/**
	 * Clears the console screen.
	 */
	public static void clearConsole() {
		try {
			final String os = System.getProperty("os.name");
			if (os.contains("Windows")) {
				new ProcessBuilder("cmd", "/c", "cls")
					.inheritIO()
					.start()
					.waitFor();
			} else {
				System.out.print("\033[H\033[2J");
				System.out.flush();
			}
		} catch (final Exception e) {
			// Handle exceptions
		}
	}

	/**
	 * Initializes the game field with all cells dead (0).
	 *
	 * @param rows The number of rows.
	 * @param cols The number of columns.
	 * @return An initialized game field.
	 */
	public static List<List<Integer>> initializeGameField(int rows, int cols) {
		List<List<Integer>> field = new ArrayList<>();
		for (int i = 0; i < rows; i++) {
			List<Integer> row = new ArrayList<>(Collections.nCopies(cols, 0));
			field.add(row);
		}
		return field;
	}

	/**
	 * Prints the game field to the console. 'O' for live cells, ' ' for dead cells.
	 *
	 * @param field The game field to print.
	 */
	public static void printGameField(List<List<Integer>> field) {
		for (List<Integer> rowVect : field) {
			for (int cell : rowVect) {
				System.out.print((cell == 1 ? 'O' : ' ') + " ");
			}
			System.out.println();
		}
	}

	/**
	 * Counts the number of live neighbors for a given cell.
	 * The field is treated as a toroidal (wrapped) surface.
	 *
	 * @param field The current game field.
	 * @param row   The row index of the cell.
	 * @param col   The column index of the cell.
	 * @return The number of live neighbors.
	 */
	public static int countLiveNeighbors(
		List<List<Integer>> field,
		int row,
		int col
	) {
		int liveNeighbors = 0;
		int numRows = field.size();
		int numCols = field.get(0).size();

		for (int i = -1; i <= 1; ++i) {
			for (int j = -1; j <= 1; ++j) {
				if (i == 0 && j == 0) continue; // Skip the cell itself

				int neighborRow = (row + i + numRows) % numRows; // Toroidal wrapping
				int neighborCol = (col + j + numCols) % numCols; // Toroidal wrapping

				if (field.get(neighborRow).get(neighborCol) == 1) {
					liveNeighbors++;
				}
			}
		}
		return liveNeighbors;
	}

	/**
	 * Updates the state of the game field based on Conway's Game of Life rules.
	 *
	 * @param field The current game field to be updated.
	 */
	public static void updateGameField(List<List<Integer>> field) {
		int numRows = field.size();
		int numCols = field.get(0).size();
		List<List<Integer>> nextGeneration = new ArrayList<>();

		// Create a deep copy for the next generation
		for (List<Integer> row : field) {
			nextGeneration.add(new ArrayList<>(row));
		}

		for (int i = 0; i < numRows; ++i) {
			for (int j = 0; j < numCols; ++j) {
				int liveNeighbors = countLiveNeighbors(field, i, j);

				if (field.get(i).get(j) == 1) { // If cell is alive
					if (liveNeighbors < 2 || liveNeighbors > 3) {
						nextGeneration.get(i).set(j, 0); // Dies from underpopulation or overpopulation
					}
				} else { // If cell is dead
					if (liveNeighbors == 3) {
						nextGeneration.get(i).set(j, 1); // Becomes alive
					}
				}
			}
		}
		// Update the current field with the next generation
		for (int i = 0; i < numRows; i++) {
			for (int j = 0; j < numCols; j++) {
				field.get(i).set(j, nextGeneration.get(i).get(j));
			}
		}
	}

	/**
	 * Initializes the game field with the Gosper Glider Gun pattern.
	 *
	 * @param field The game field to initialize.
	 */
	public static void initializeGun(List<List<Integer>> field) {
		int rowOffset = 5;
		int colOffset = 5;

		// Left block
		field.get(rowOffset + 1).set(colOffset + 25, 1);
		field.get(rowOffset + 2).set(colOffset + 23, 1);
		field.get(rowOffset + 2).set(colOffset + 25, 1);
		field.get(rowOffset + 3).set(colOffset + 13, 1);
		field.get(rowOffset + 3).set(colOffset + 14, 1);
		field.get(rowOffset + 3).set(colOffset + 21, 1);
		field.get(rowOffset + 3).set(colOffset + 22, 1);
		field.get(rowOffset + 3).set(colOffset + 35, 1);
		field.get(rowOffset + 3).set(colOffset + 36, 1);
		field.get(rowOffset + 4).set(colOffset + 12, 1);
		field.get(rowOffset + 4).set(colOffset + 16, 1);
		field.get(rowOffset + 4).set(colOffset + 21, 1);
		field.get(rowOffset + 4).set(colOffset + 22, 1);
		field.get(rowOffset + 4).set(colOffset + 35, 1);
		field.get(rowOffset + 4).set(colOffset + 36, 1);
		field.get(rowOffset + 5).set(colOffset + 1, 1);
		field.get(rowOffset + 5).set(colOffset + 2, 1);
		field.get(rowOffset + 5).set(colOffset + 11, 1);
		field.get(rowOffset + 5).set(colOffset + 17, 1);
		field.get(rowOffset + 5).set(colOffset + 21, 1);
		field.get(rowOffset + 5).set(colOffset + 22, 1);
		field.get(rowOffset + 6).set(colOffset + 1, 1);
		field.get(rowOffset + 6).set(colOffset + 2, 1);
		field.get(rowOffset + 6).set(colOffset + 11, 1);
		field.get(rowOffset + 6).set(colOffset + 15, 1);
		field.get(rowOffset + 6).set(colOffset + 17, 1);
		field.get(rowOffset + 6).set(colOffset + 18, 1);
		field.get(rowOffset + 6).set(colOffset + 23, 1);
		field.get(rowOffset + 6).set(colOffset + 25, 1);
		field.get(rowOffset + 7).set(colOffset + 11, 1);
		field.get(rowOffset + 7).set(colOffset + 17, 1);
		field.get(rowOffset + 7).set(colOffset + 25, 1);
		field.get(rowOffset + 8).set(colOffset + 12, 1);
		field.get(rowOffset + 8).set(colOffset + 16, 1);
		field.get(rowOffset + 9).set(colOffset + 13, 1);
		field.get(rowOffset + 9).set(colOffset + 14, 1);
	}

	/**
	 * Task 3: Implementation of Conway's Game of Life.
	 * Allows selection between Gosper Glider Gun or random initial colonies.
	 */
	public static void taskThree() {
		int choice = InputUtils.getValidIntegerInput(
			"Выберите режим:\n1. Ружье Глайдера Госпера\n2. Случайные колонии\nВаш выбор: "
		);
		while (choice != 1 && choice != 2) {
			System.out.println("Неверный выбор. Пожалуйста, введите 1 или 2.");
			choice = InputUtils.getValidIntegerInput(
				"Выберите режим:\n1. Ружье Глайдера Госпера\n2. Случайные колонии\nВаш выбор: "
			);
		}

		int gameRows = 30; // Set reasonable field dimensions
		int gameCols = 80;

		List<List<Integer>> gameField = initializeGameField(gameRows, gameCols);

		if (choice == 1) {
			// Initialize Gosper Glider Gun
			initializeGun(gameField);
		} else {
			// Initialize random colonies
			Random random = new Random();
			for (int i = 0; i < gameRows; ++i) {
				for (int j = 0; j < gameCols; ++j) {
					gameField.get(i).set(j, random.nextInt(2)); // 0 or 1
				}
			}
		}

		int generations = 0;
		while (true) {
			clearConsole();
			System.out.println("Поколение: " + generations);
			printGameField(gameField);
			updateGameField(gameField);
			generations++;
			try {
				Thread.sleep(100); // Delay for animation
			} catch (InterruptedException e) {
				Thread.currentThread().interrupt();
				System.out.println("Game interrupted.");
				break;
			}
		}
	}
}
