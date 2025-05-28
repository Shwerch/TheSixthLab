import java.util.List;

public class MatrixUtils {

	/**
	 * Prints a 2D list (matrix) of integers to the console.
	 *
	 * @param matrix The matrix to print.
	 */
	public static void printMatrix(List<List<Integer>> matrix) {
		for (List<Integer> rowVect : matrix) {
			for (int elem : rowVect) {
				System.out.print(elem + "\t");
			}
			System.out.println();
		}
	}
}
