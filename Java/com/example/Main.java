package com.example;

import com.example.input.InputUtils;
import com.example.modules.*;
import java.util.Scanner;

public class Main {

	public static void main(String[] args) {
		try (Scanner scanner = new Scanner(System.in)) {
			InputUtils.setScanner(scanner); // Инициализация сканера для утилит

			while (true) {
				MainMenu.print();
				int choice = InputUtils.getInt("Введите раздел программы");

				try {
					MainMenu option = MainMenu.fromInt(choice);
					switch (option) {
						case MATRIX_OPERATIONS -> MatrixOperations.run();
						case MATRIX_REPLACE -> MatrixReplace.run();
						case GAME_OF_LIFE -> GameOfLife.run();
						case MAXIMISE_BANANAS -> MaximiseBananas.run();
						case GAUSS_SOLVER -> GaussSolver.run();
						case ITERATIVE_SOLVER -> IterativeSolver.run();
						case AES -> AesCipher.run();
						case EXIT -> {
							System.out.println("Выход из программы.");
							return;
						}
					}
				} catch (IllegalArgumentException e) {
					System.err.println("Ошибка: " + e.getMessage() + "\n");
				}
			}
		}
	}
}
