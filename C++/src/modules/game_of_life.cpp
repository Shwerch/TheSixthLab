#include "../../include/modules/game_of_life.hpp"

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <vector>

// Используем структуру вместо класса
struct GameOfLife {
	int width;
	int height;
	std::vector<std::vector<bool>> grid;
	std::vector<std::vector<bool>> nextGrid;

	// Конструктор (в структуре работает аналогично классу)
	GameOfLife(int width, int height);

	// Инициализация сетки случайными значениями
	void initializeRandomly(double density = 0.3);

	// Инициализация сетки по шаблону (например, для ружья)
	void initializePattern(const std::vector<std::string> &pattern, int startX, int startY);

	// Вычисление следующего поколения
	void nextGeneration();

	// Отрисовка текущего состояния сетки
	void draw() const;

	// Запуск симуляции
	void run(int generations, std::chrono::milliseconds delay);

	// Очистка консоли
	void clearConsole() const;

  private:
	// Приватный метод для структуры
	int countLiveNeighbors(int x, int y) const;
};

GameOfLife::GameOfLife(int width, int height)
	: width(width), height(height), grid(height, std::vector<bool>(width, false)),
	  nextGrid(height, std::vector<bool>(width, false)) {}

void GameOfLife::initializeRandomly(double density) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0.0, 1.0);

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			grid[y][x] = (dis(gen) < density);
		}
	}
}

void GameOfLife::initializePattern(const std::vector<std::string> &pattern, int startX,
								   int startY) {
	for (size_t y = 0; y < pattern.size(); ++y) {
		for (size_t x = 0; x < pattern[y].length(); ++x) {
			int gridX = startX + x;
			int gridY = startY + y;
			if (gridX >= 0 && gridX < width && gridY >= 0 && gridY < height) {
				grid[gridY][gridX] = (pattern[y][x] == 'X' || pattern[y][x] == '1');
			}
		}
	}
}

int GameOfLife::countLiveNeighbors(int x, int y) const {
	int liveNeighbors = 0;
	for (int dy = -1; dy <= 1; ++dy) {
		for (int dx = -1; dx <= 1; ++dx) {
			if (dx == 0 && dy == 0)
				continue;

			// Замкнутая плоскость (тор)
			int nx = (x + dx + width) % width;
			int ny = (y + dy + height) % height;

			if (grid[ny][nx]) {
				liveNeighbors++;
			}
		}
	}
	return liveNeighbors;
}

void GameOfLife::nextGeneration() {
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			int liveNeighbors = countLiveNeighbors(x, y);
			bool isAlive = grid[y][x];

			// Правила игры "Жизнь"
			if (isAlive) {
				if (liveNeighbors < 2 || liveNeighbors > 3) {
					nextGrid[y][x] = false; // Умирает от одиночества или перенаселения
				} else {
					nextGrid[y][x] = true; // Выживает
				}
			} else {
				if (liveNeighbors == 3) {
					nextGrid[y][x] = true; // Рождается
				} else {
					nextGrid[y][x] = false; // Остается мертвой
				}
			}
		}
	}
	grid = nextGrid; // Обновляем сетку
}

void GameOfLife::draw() const {
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			std::cout << (grid[y][x] ? 'X' : ' ');
		}
		std::cout << '\n';
	}
}

void GameOfLife::clearConsole() const {
#ifdef _WIN32
	std::system("cls");
#else
	std::system("clear");
#endif
}

void GameOfLife::run(int generations, std::chrono::milliseconds delay) {
	for (int i = 0; i < generations; ++i) {
		clearConsole();
		draw();
		std::cout << "Поколение: " << i + 1 << "\n";
		nextGeneration();
		std::this_thread::sleep_for(delay);
	}
}

void runGameOfLife() {
	// Демонстрация ружья Госпера
	std::cout << "Демонстрация ружья Госпера:\n";
	// Создание экземпляра структуры GameOfLife
	GameOfLife gliderGunGame(80, 40); // Увеличьте размер для ружья

	// Шаблон ружья Госпера
	std::vector<std::string> gosperGliderGun = {
		"........................X...........", "......................X.X...........",
		"............XX......XX............XX", "...........X...X....XX............XX",
		"XX........X.....X...XX..............", "XX........X...X.XX....X.X...........",
		"..........X.....X.......X...........", "...........X...X....................",
		"............XX......................"};
	gliderGunGame.initializePattern(gosperGliderGun, 1, 1); // Размещаем ружье в начале
	gliderGunGame.run(200, std::chrono::milliseconds(100)); // 200 поколений с задержкой 100 мс

	// Демонстрация случайной колонии
	std::cout << "\nДемонстрация случайной колонии:\n";
	// Создание экземпляра структуры GameOfLife
	GameOfLife randomGame(60, 30);						 // Меньший размер для случайной колонии
	randomGame.initializeRandomly(0.2);					 // Плотность 20%
	randomGame.run(150, std::chrono::milliseconds(150)); // 150 поколений с задержкой 150 мс
}
