#include "../../include/modules/maximise_bananas.hpp"
#include "../../include/input.hpp"

#include <cstdint>
#include <iostream>
#include <map>

static inline int countSetBits(std::uint64_t n) {
	int count = 0;
	while (n > 0) {
		n &= (n - 1); // Снимаем младший установленный бит
		count++;
	}
	return count;
}

std::pair<std::uint64_t, std::uint64_t> maximiseBananas(std::uint64_t n) {
	std::uint64_t best_a = 0;
	std::uint64_t best_b = n;
	int max_bananas = countSetBits(0) + countSetBits(n);
	std::uint64_t max_diff = n; // Для случая a=0, b=n

	// Если N == 0, то пара (0, 0)
	if (n == 0) {
		return {0, 0};
	}

	// Итерируемся от 0 до N/2, так как пары (a, b) и (b, a) имеют одинаковое
	// количество бананов. Выбираем ту, у которой разность больше, что
	// соответствует случаю, когда b > a.
	// Если N четное, N/2 будет целочисленно делиться.
	// Если N нечетное, N/2 округлится вниз, и мы не будем рассматривать a > N/2.
	// Например, для N=5, мы проверим a=0, 1, 2.
	for (std::uint64_t a = 0; a <= n / 2; ++a) {
		std::uint64_t b = n - a;
		int current_bananas = countSetBits(a) + countSetBits(b);
		std::uint64_t current_diff = b - a;

		if (current_bananas > max_bananas) {
			max_bananas = current_bananas;
			best_a = a;
			best_b = b;
			max_diff = current_diff;
		} else if (current_bananas == max_bananas) {
			// Если количество бананов одинаково, выбираем ту пару,
			// у которой разность чисел наибольшая.
			if (current_diff > max_diff) {
				best_a = a;
				best_b = b;
				max_diff = current_diff;
			}
		}
	}

	return {best_a, best_b};
}

void runMaximiseBananas() {
	std::pair<std::uint64_t, std::uint64_t> bananas =
		maximiseBananas(getValue<std::uint64_t>("Введите число N"));
	std::cout << bananas.first << " " << bananas.second << std::endl;
}
