#include "library.h"

// Проверяет ввод целого числа
int getValidIntegerInput(const std::string& promMsg) {
    int value;
    while (true) {
        std::cout << promMsg;
        std::cin >> value;
        if (std::cin.fail()) {
            std::cout << "Неверный ввод. Пожалуйста, введите целое число." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
    }
}

// Выводит матрицу
void printMatrix(const std::vector<std::vector<int>>& matr) {
    for (const auto& rowVect : matr) {
        for (int elem : rowVect) {
            std::cout << elem << "\t";
        }
        std::cout << std::endl;
    }
}

// Задание 1: Работа с матрицей M x N
void taskOne() {
    int numRows = getValidIntegerInput("Введите количество строк M (целое число > 0): ");
    while (numRows <= 0) {
        std::cout << "Количество строк должно быть больше 0." << std::endl;
        numRows = getValidIntegerInput("Введите количество строк M (целое число > 0): ");
    }

    int numCols = getValidIntegerInput("Введите количество столбцов N (целое число > 0): ");
    while (numCols <= 0) {
        std::cout << "Количество столбцов должно быть больше 0." << std::endl;
        numCols = getValidIntegerInput("Введите количество столбцов N (целое число > 0): ");
    }

    std::vector<std::vector<int>> initialMatrix(numRows, std::vector<int>(numCols));
    std::random_device devRand;
    std::mt19937 gener(devRand());
    // Использование int для uniform_int_distribution, так как диапазон включает отрицательные числа
    std::uniform_int_distribution<int> dist(-10, 40);

    // Инициализация матрицы случайными числами
    for (int i = 0; i < numRows; ++i) {
        for (int j = 0; j < numCols; ++j) {
            initialMatrix[i][j] = dist(gener);
        }
    }

    std::cout << "Исходная матрица M x N:" << std::endl;
    printMatrix(initialMatrix);

    std::vector<int> negativeElements;
    std::vector<std::vector<int>> newMatrix;

    std::cout << "Номера строк, содержащих не менее трех отрицательных элементов:" << std::endl;
    for (int i = 0; i < numRows; ++i) {
        int negCount = 0;
        for (int elem : initialMatrix[i]) {
            if (elem < 0) {
                negCount++;
            }
        }
        if (negCount >= 3) {
            std::cout << "Строка " << i << std::endl;
            // Инициализация нового массива отрицательными элементами
            for (int elem : initialMatrix[i]) {
                if (elem < 0) {
                    negativeElements.push_back(elem);
                }
            }
            // Создание новой матрицы из найденных строк
            newMatrix.push_back(initialMatrix[i]);
        }
    }

    std::cout << "Новый массив с отрицательными элементами из найденных строк:" << std::endl;
    for (int elem : negativeElements) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    if (!newMatrix.empty()) {
        std::cout << "Новая матрица, состоящая из найденных строк:" << std::endl;
        printMatrix(newMatrix);
    } else {
        std::cout << "Новых строк с тремя и более отрицательными элементами не найдено." << std::endl;
    }
}

// Задание 2: Работа с квадратной матрицей N x N
void taskTwo() {
    int orderN = getValidIntegerInput("Введите порядок N квадратной матрицы (четное целое число > 0): ");
    while (orderN <= 0 || orderN % 2 != 0) {
        std::cout << "Порядок N должен быть четным положительным числом." << std::endl;
        orderN = getValidIntegerInput("Введите порядок N квадратной матрицы (четное целое число > 0): ");
    }

    std::vector<std::vector<int>> squareMatrix(orderN, std::vector<int>(orderN));
    std::random_device devRand;
    std::mt19937 gener(devRand());
    // Использование int для uniform_int_distribution
    std::uniform_int_distribution<int> dist(10, 30);

    // Инициализация квадратной матрицы случайными числами
    for (int i = 0; i < orderN; ++i) {
        for (int j = 0; j < orderN; ++j) {
            squareMatrix[i][j] = dist(gener);
        }
    }

    std::cout << "Исходная квадратная матрица:" << std::endl;
    printMatrix(squareMatrix);

    int halfN = orderN / 2;

    // Замена значений в нижней левой четверти на 0, в нижней правой на 10
    for (int i = halfN; i < orderN; ++i) {
        for (int j = 0; j < halfN; ++j) {
            squareMatrix[i][j] = 0; // Нижняя левая четверть
        }
        for (int j = halfN; j < orderN; ++j) {
            squareMatrix[i][j] = 10; // Нижняя правая четверть
        }
    }

    // Обмен оставшихся четвертей (верхняя левая с верхней правой)
    for (int i = 0; i < halfN; ++i) {
        for (int j = 0; j < halfN; ++j) {
            // Обмен элемента (i, j) верхней левой с (i, j + halfN) верхней правой
            std::swap(squareMatrix[i][j], squareMatrix[i][j + halfN]);
        }
    }

    std::cout << "Преобразованная квадратная матрица:" << std::endl;
    printMatrix(squareMatrix);
}

// Очищает консоль
void clearConsole() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Инициализирует игровое поле для "Жизни"
std::vector<std::vector<int>> initializeGameField(int rows, int cols) {
    std::vector<std::vector<int>> field(rows, std::vector<int>(cols, 0));
    return field;
}

// Выводит игровое поле
void printGameField(const std::vector<std::vector<int>>& field) {
    for (const auto& rowVect : field) {
        for (int cell : rowVect) {
            std::cout << (cell == 1 ? 'O' : ' ') << " "; // 'O' для живой, ' ' для мертвой
        }
        std::cout << std::endl;
    }
}

// Подсчитывает живых соседей для клетки
int countLiveNeighbors(const std::vector<std::vector<int>>& field, int row, int col) {
    int liveNeighbors = 0;
    int numRows = field.size();
    int numCols = field[0].size();

    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i == 0 && j == 0) continue; // Пропускаем саму клетку

            int neighborRow = (row + i + numRows) % numRows; // Замкнутая плоскость
            int neighborCol = (col + j + numCols) % numCols; // Замкнутая плоскость

            if (field[neighborRow][neighborCol] == 1) {
                liveNeighbors++;
            }
        }
    }
    return liveNeighbors;
}

// Обновляет состояние игрового поля
void updateGameField(std::vector<std::vector<int>>& field) {
    int numRows = field.size();
    int numCols = field[0].size();
    std::vector<std::vector<int>> nextGeneration = field; // Создаем копию для следующего поколения

    for (int i = 0; i < numRows; ++i) {
        for (int j = 0; j < numCols; ++j) {
            int liveNeighbors = countLiveNeighbors(field, i, j);

            if (field[i][j] == 1) { // Если клетка жива
                if (liveNeighbors < 2 || liveNeighbors > 3) {
                    nextGeneration[i][j] = 0; // Умирает от одиночества или перенаселения
                }
            } else { // Если клетка мертва
                if (liveNeighbors == 3) {
                    nextGeneration[i][j] = 1; // Рождается
                }
            }
        }
    }
    field = nextGeneration; // Обновляем текущее поле
}

// Инициализирует поле с "ружьем" (Gosper Glider Gun)
void initializeGun(std::vector<std::vector<int>>& field) {
    // Размеры поля должны быть достаточно большими для ружья, например, 38x11 или больше
    // Это относительно центральной точки ружья.
    // Зададим смещение, чтобы ружье было видно
    int rowOffset = 5;
    int colOffset = 5;

    // Левый блок
    field[rowOffset + 1][colOffset + 25] = 1;
    field[rowOffset + 2][colOffset + 23] = 1;
    field[rowOffset + 2][colOffset + 25] = 1;
    field[rowOffset + 3][colOffset + 13] = 1;
    field[rowOffset + 3][colOffset + 14] = 1;
    field[rowOffset + 3][colOffset + 21] = 1;
    field[rowOffset + 3][colOffset + 22] = 1;
    field[rowOffset + 3][colOffset + 35] = 1;
    field[rowOffset + 3][colOffset + 36] = 1;
    field[rowOffset + 4][colOffset + 12] = 1;
    field[rowOffset + 4][colOffset + 16] = 1;
    field[rowOffset + 4][colOffset + 21] = 1;
    field[rowOffset + 4][colOffset + 22] = 1;
    field[rowOffset + 4][colOffset + 35] = 1;
    field[rowOffset + 4][colOffset + 36] = 1;
    field[rowOffset + 5][colOffset + 1] = 1;
    field[rowOffset + 5][colOffset + 2] = 1;
    field[rowOffset + 5][colOffset + 11] = 1;
    field[rowOffset + 5][colOffset + 17] = 1;
    field[rowOffset + 5][colOffset + 21] = 1;
    field[rowOffset + 5][colOffset + 22] = 1;
    field[rowOffset + 6][colOffset + 1] = 1;
    field[rowOffset + 6][colOffset + 2] = 1;
    field[rowOffset + 6][colOffset + 11] = 1;
    field[rowOffset + 6][colOffset + 15] = 1;
    field[rowOffset + 6][colOffset + 17] = 1;
    field[rowOffset + 6][colOffset + 18] = 1;
    field[rowOffset + 6][colOffset + 23] = 1;
    field[rowOffset + 6][colOffset + 25] = 1;
    field[rowOffset + 7][colOffset + 11] = 1;
    field[rowOffset + 7][colOffset + 17] = 1;
    field[rowOffset + 7][colOffset + 25] = 1;
    field[rowOffset + 8][colOffset + 12] = 1;
    field[rowOffset + 8][colOffset + 16] = 1;
    field[rowOffset + 9][colOffset + 13] = 1;
    field[rowOffset + 9][colOffset + 14] = 1;
}

// Задание 3: Реализация игры "Жизнь"
void taskThree() {
    int choice = getValidIntegerInput("Выберите режим:\n1. Ружье Глайдера Госпера\n2. Случайные колонии\nВаш выбор: ");
    while (choice != 1 && choice != 2) {
        std::cout << "Неверный выбор. Пожалуйста, введите 1 или 2." << std::endl;
        choice = getValidIntegerInput("Выберите режим:\n1. Ружье Глайдера Госпера\n2. Случайные колонии\nВаш выбор: ");
    }

    int gameRows = 30; // Установим разумные размеры поля
    int gameCols = 80;

    std::vector<std::vector<int>> gameField = initializeGameField(gameRows, gameCols);

    if (choice == 1) {
        // Инициализация "ружья"
        initializeGun(gameField);
    } else {
        // Инициализация случайных колоний
        std::random_device devRand;
        std::mt19937 gener(devRand());
        // Использование int для uniform_int_distribution
        std::uniform_int_distribution<int> dist(0, 1); // 0 или 1

        for (int i = 0; i < gameRows; ++i) {
            for (int j = 0; j < gameCols; ++j) {
                gameField[i][j] = dist(gener);
            }
        }
    }

    int generations = 0;
    while (true) {
        clearConsole();
        std::cout << "Поколение: " << generations << std::endl;
        printGameField(gameField);
        updateGameField(gameField);
        generations++;
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Задержка для анимации
    }
}
