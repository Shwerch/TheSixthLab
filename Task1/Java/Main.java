public class Main {

	public static void main(String[] args) {
		// Task 1
		System.out.println(new String(new char[30]).replace('\0', '-'));
		System.out.println("Задание 1");
		System.out.println(new String(new char[30]).replace('\0', '-'));
		MatrixOperations.taskOne();
		System.out.println();

		// Task 2
		System.out.println(new String(new char[30]).replace('\0', '-'));
		System.out.println("Задание 2");
		System.out.println(new String(new char[30]).replace('\0', '-'));
		SquareMatrixOperations.taskTwo();
		System.out.println();

		// Task 3
		System.out.println(new String(new char[30]).replace('\0', '-'));
		System.out.println("Задание 3: Игра \"Жизнь\"");
		System.out.println(new String(new char[30]).replace('\0', '-'));
		GameOfLife.taskThree();
	}
}
