import java.util.InputMismatchException;
import java.util.Scanner;

public class InputUtils {

	private static final Scanner scanner = new Scanner(System.in);

	/**
	 * Prompts the user for an integer input and validates it.
	 *
	 * @param promptMsg The message to display to the user.
	 * @return A valid integer input.
	 */
	public static int getValidIntegerInput(String promptMsg) {
		int value;
		while (true) {
			System.out.print(promptMsg);
			try {
				value = scanner.nextInt();
				scanner.nextLine(); // Consume the rest of the line
				return value;
			} catch (InputMismatchException e) {
				System.out.println(
					"Неверный ввод. Пожалуйста, введите целое число."
				);
				scanner.nextLine(); // Consume the invalid input
			}
		}
	}
}
