package com.example;

public enum MainMenu {
	MATRIX_OPERATIONS("Определение матрицы M на N псевдослучайными числами"),
	MATRIX_REPLACE("Определение матрицы N на N и перестановка значений в ней"),
	GAME_OF_LIFE("Игра 'Жизнь'"),
	MAXIMISE_BANANAS("Игра с бананами"),
	GAUSS_SOLVER("Решение системы уравнений методом Гаусса"),
	ITERATIVE_SOLVER("Решение системы уравнений приближенным методом"),
	AES("Алгоритм шифрования AES128 CBC"),
	EXIT("Выход из программы");

	private final String description;

	MainMenu(String description) {
		this.description = description;
	}

	public static void print() {
		System.out.println("\n--- Главное меню ---");
		for (MainMenu item : values()) {
			System.out.printf(
				"[%d] %s%n",
				item.ordinal() + 1,
				item.description
			);
		}
		System.out.println();
	}

	public static MainMenu fromInt(int value) {
		if (value < 1 || value > values().length) {
			throw new IllegalArgumentException(
				"Некорректный ввод. Такого раздела не существует."
			);
		}
		return values()[value - 1];
	}
}
