#include "../include/input.hpp"
#include "../include/main_menu.hpp"

#include "../include/modules/matrix_operations.hpp"
#include "../include/modules/matrix_replace.hpp"

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
		case MainMenu::EXIT:
			return 0;
		}
	}
	return 0;
}
