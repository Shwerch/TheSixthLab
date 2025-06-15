package com.example.modules;

import com.example.input.InputUtils;
import java.util.Random;
import java.util.concurrent.TimeUnit;

public class GameOfLife {

	private final int width;
	private final int height;
	private boolean[][] grid;

	public GameOfLife(int width, int height) {
		this.width = width;
		this.height = height;
		this.grid = new boolean[height][width];
	}

	public void initializeRandomly(double density) {
		Random random = new Random();
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				grid[y][x] = random.nextDouble() < density;
			}
		}
	}

	public void initializePattern(String[] pattern, int startX, int startY) {
		for (int y = 0; y < pattern.length; y++) {
			for (int x = 0; x < pattern[y].length(); x++) {
				if (
					pattern[y].charAt(x) == 'X' || pattern[y].charAt(x) == '1'
				) {
					int gridX = startX + x;
					int gridY = startY + y;
					if (
						gridX >= 0 &&
						gridX < width &&
						gridY >= 0 &&
						gridY < height
					) {
						grid[gridY][gridX] = true;
					}
				}
			}
		}
	}

	private int countLiveNeighbors(int x, int y) {
		int count = 0;
		for (int dy = -1; dy <= 1; dy++) {
			for (int dx = -1; dx <= 1; dx++) {
				if (dx == 0 && dy == 0) continue;
				int nx = (x + dx + width) % width; // Замкнутая плоскость (тор)
				int ny = (y + dy + height) % height;
				if (grid[ny][nx]) {
					count++;
				}
			}
		}
		return count;
	}

	public void nextGeneration() {
		boolean[][] nextGrid = new boolean[height][width];
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				int liveNeighbors = countLiveNeighbors(x, y);
				boolean isAlive = grid[y][x];

				if (isAlive) {
					// Выживает с 2 или 3 соседями
					nextGrid[y][x] = (liveNeighbors == 2 || liveNeighbors == 3);
				} else {
					// Рождается с 3 соседями
					nextGrid[y][x] = (liveNeighbors == 3);
				}
			}
		}
		grid = nextGrid;
	}

	public void draw() {
		StringBuilder sb = new StringBuilder();
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				sb.append(grid[y][x] ? 'X' : ' ');
			}
			sb.append('\n');
		}
		System.out.print(sb);
	}

	private static void clearConsole() {
		// Простой и переносимый способ "очистки"
		System.out.print("\033[H\033[2J");
		System.out.flush();
	}

	public void animate(int generations, int delayMs)
		throws InterruptedException {
		for (int i = 0; i < generations; i++) {
			clearConsole();
			System.out.println("Поколение: " + (i + 1));
			draw();
			nextGeneration();
			TimeUnit.MILLISECONDS.sleep(delayMs);
		}
	}

	public static void run() {
		try {
			System.out.println("Демонстрация ружья Госпера:");
			GameOfLife gliderGunGame = new GameOfLife(80, 40);
			String[] gosperGliderGun = {
				"........................X...........",
				"......................X.X...........",
				"............XX......XX............XX",
				"...........X...X....XX............XX",
				"XX........X.....X...XX..............",
				"XX........X...X.XX....X.X...........",
				"..........X.....X.......X...........",
				"...........X...X....................",
				"............XX......................",
			};
			gliderGunGame.initializePattern(gosperGliderGun, 1, 1);
			gliderGunGame.animate(200, 100);

			System.out.println(
				"\nНажмите Enter для демонстрации случайной колонии..."
			);
			InputUtils.getString("");

			System.out.println("\nДемонстрация случайной колонии:");
			GameOfLife randomGame = new GameOfLife(60, 30);
			randomGame.initializeRandomly(0.2);
			randomGame.animate(150, 150);
		} catch (InterruptedException e) {
			System.err.println("Анимация была прервана.");
			Thread.currentThread().interrupt();
		}
	}
}
