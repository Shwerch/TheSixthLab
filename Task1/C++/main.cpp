#include "library.h"

int main() {
    // Задача 1
    // Инициализация матрицы и поиск строк с отрицательными элементами
    std::cout << std::string(30, '-') << std::endl;
    std::cout << "Задание 1" << std::endl;
    std::cout << std::string(30, '-') << std::endl;
    taskOne();
    std::cout << std::endl;

    // Задача 2
    // Работа с квадратной матрицей
    std::cout << std::string(30, '-') << std::endl;
    std::cout << "Задание 2" << std::endl;
    std::cout << std::string(30, '-') << std::endl;
    taskTwo();
    std::cout << std::endl;

    // Задача 3
    // Игра Жизнь
    std::cout << std::string(30, '-') << std::endl;
    std::cout << "Задание 3: Игра \"Жизнь\"" << std::endl;
    std::cout << std::string(30, '-') << std::endl;
    taskThree();

    return 0;
}
