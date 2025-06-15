package com.example.input;

import java.util.InputMismatchException;
import java.util.Scanner;

public final class InputUtils {

	private static Scanner scanner;

	// Запрещаем создание экземпляров
	private InputUtils() {}

	public static void setScanner(Scanner scannerInstance) {
		scanner = scannerInstance;
	}

	public static int getInt(String message) {
		while (true) {
			System.out.print(message + ": ");
			try {
				int value = scanner.nextInt();
				scanner.nextLine(); // Очистка буфера
				return value;
			} catch (InputMismatchException e) {
				System.err.println(
					"Некорректный ввод. Пожалуйста, введите целое число."
				);
				scanner.nextLine(); // Очистка буфера
			}
		}
	}

	public static long getLong(String message) {
		while (true) {
			System.out.print(message + ": ");
			try {
				long value = scanner.nextLong();
				scanner.nextLine(); // Очистка буфера
				return value;
			} catch (InputMismatchException e) {
				System.err.println(
					"Некорректный ввод. Пожалуйста, введите целое число."
				);
				scanner.nextLine(); // Очистка буфера
			}
		}
	}

	public static String getString(String message) {
		System.out.print(message + ": ");
		return scanner.nextLine();
	}
}
