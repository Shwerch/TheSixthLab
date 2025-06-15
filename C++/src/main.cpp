#include "../include/input.hpp"
#include "../include/main_menu.hpp"

#include "../include/modules/aes_cipher.hpp"
#include "../include/modules/game_of_life.hpp"
#include "../include/modules/gauss_solver.hpp"
#include "../include/modules/iterative_solver.hpp"
#include "../include/modules/matrix_operations.hpp"
#include "../include/modules/matrix_replace.hpp"
#include "../include/modules/maximise_bananas.hpp"

int main() {
	while (true) {
		printMainMenu();
		int value = getValue<int>("Введите раздел программы");
		MainMenu element;
		try {
			element = convertIntToMainMenu(value);
		} catch (...) {
			continue;
		}
		switch (element) {
		case MainMenu::MATRIX_OPERATIONS:
			runMatrixOperations();
			break;
		case MainMenu::MATRIX_REPLACE:
			runMatrixReplace();
			break;
		case MainMenu::GAME_OF_LIFE:
			runGameOfLife();
			break;
		case MainMenu::MAXIMISE_BANANAS:
			runMaximiseBananas();
			break;
		case MainMenu::GAUSS_SOLVER:
			runGaussSolver();
			break;
		case MainMenu::ITERATIVE_SOLVER:
			runIterativeSolver();
			break;
		case MainMenu::AES:
			runAES();
			break;
		case MainMenu::EXIT:
			return 0;
		}
	}
	return 0;
}
