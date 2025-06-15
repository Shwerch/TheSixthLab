package com.example.modules;

import com.example.input.InputUtils;

public class MaximiseBananas {

	// Record для хранения пары чисел
	private record Pair(long a, long b) {}

	private static Pair maximiseBananas(long n) {
		if (n == 0) {
			return new Pair(0, 0);
		}

		long bestA = 0;
		long bestB = n;
		// Long.bitCount() - встроенный и эффективный способ подсчета установленных битов
		int maxBananas = Long.bitCount(bestA) + Long.bitCount(bestB);

		// Итерируемся до n/2, так как пары (a, b) и (b, a) симметричны
		for (long a = 1; a <= n / 2; a++) {
			long b = n - a;
			int currentBananas = Long.bitCount(a) + Long.bitCount(b);

			if (currentBananas > maxBananas) {
				maxBananas = currentBananas;
				bestA = a;
				bestB = b;
			}
			// Условие про наибольшую разность уже выполняется,
			// так как мы идем от a=0, что дает максимальную разность b-a.
			// Первая найденная пара с максимальным числом бананов будет иметь
			// наименьшее 'a' и, следовательно, наибольшую разность.
		}
		return new Pair(bestA, bestB);
	}

	public static void run() {
		long n = InputUtils.getLong("Введите число N");
		Pair result = maximiseBananas(n);
		System.out.printf("Результат: %d %d%n", result.a(), result.b());
		System.out.printf(
			"Количество бананов: %d%n",
			Long.bitCount(result.a()) + Long.bitCount(result.b())
		);
	}
}
