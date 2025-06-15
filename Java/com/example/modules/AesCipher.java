package com.example.modules;

import com.example.input.InputUtils;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.security.SecureRandom;
import java.util.Arrays;
import java.util.HexFormat;
import javax.crypto.Cipher;
import javax.crypto.KeyGenerator;
import javax.crypto.SecretKey;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;

public class AesCipher {

	private static final String ALGORITHM = "AES";
	private static final String TRANSFORMATION = "AES/CBC/PKCS5Padding"; // Совместимо с PKCS7
	private static final int KEY_SIZE = 128; // биты
	private static final int IV_SIZE = 16; // байты
	private static final String KEY_FILE = "key.bin";

	public static void run() {
		try {
			SecretKey key = generateAndSaveKey();
			System.out.println("Сгенерирован ключ и сохранен в " + KEY_FILE);
			System.out.println(
				"Сгенерированный ключ (Key): " +
				HexFormat.of().formatHex(key.getEncoded())
			);

			while (true) {
				System.out.println(
					"\nВыберите, что обработать:\n1. Текст\n2. Файл\n3. Вернуться в меню"
				);
				int modeChoice = InputUtils.getInt("> ");
				if (modeChoice == 3) break;

				System.out.println(
					"Выберите действие:\n1. Зашифровать\n2. Расшифровать"
				);
				int actionChoice = InputUtils.getInt("> ");

				if (actionChoice == 1) { // Шифрование
					handleEncrypt(modeChoice, key);
				} else if (actionChoice == 2) { // Расшифровка
					handleDecrypt(modeChoice, key);
				} else {
					System.err.println("Неверный выбор действия.");
				}
			}
		} catch (Exception e) {
			System.err.println(
				"Произошла критическая ошибка в модуле AES: " + e.getMessage()
			);
			e.printStackTrace();
		}
	}

	private static void handleEncrypt(int modeChoice, SecretKey key)
		throws Exception {
		byte[] dataToProcess;
		if (modeChoice == 1) { // Текст
			String text = InputUtils.getString("Введите текст для шифрования");
			dataToProcess = text.getBytes();
		} else { // Файл
			String path = InputUtils.getString("Введите путь к файлу");
			dataToProcess = Files.readAllBytes(Paths.get(path));
		}

		byte[] iv = generateIv();
		System.out.println(
			"Вектор инициализации (IV): " + HexFormat.of().formatHex(iv)
		);
		byte[] cipherText = encrypt(dataToProcess, key, iv);

		// Объединяем IV и шифротекст для хранения/передачи
		byte[] outputWithIv = new byte[iv.length + cipherText.length];
		System.arraycopy(iv, 0, outputWithIv, 0, iv.length);
		System.arraycopy(
			cipherText,
			0,
			outputWithIv,
			iv.length,
			cipherText.length
		);

		System.out.println(
			"\nРезультат (IV + Шифротекст в HEX): " +
			HexFormat.of().formatHex(outputWithIv)
		);

		if (modeChoice == 2) {
			try (
				FileOutputStream fos = new FileOutputStream(
					"encrypted_file.bin"
				)
			) {
				fos.write(outputWithIv);
			}
			System.out.println(
				"Зашифрованный файл сохранен как encrypted_file.bin"
			);
		}
	}

	private static void handleDecrypt(int modeChoice, SecretKey key)
		throws Exception {
		byte[] dataToProcess;
		if (modeChoice == 1) { // HEX-строка
			String hex = InputUtils.getString(
				"Вставьте HEX-строку для расшифровки"
			);
			dataToProcess = HexFormat.of().parseHex(hex);
		} else { // Файл
			String path = InputUtils.getString("Введите путь к файлу");
			dataToProcess = Files.readAllBytes(Paths.get(path));
		}

		if (dataToProcess.length < IV_SIZE) {
			throw new IllegalArgumentException(
				"Входные данные слишком малы (отсутствует IV)."
			);
		}

		// Извлекаем IV и шифротекст
		byte[] iv = Arrays.copyOfRange(dataToProcess, 0, IV_SIZE);
		byte[] cipherText = Arrays.copyOfRange(
			dataToProcess,
			IV_SIZE,
			dataToProcess.length
		);

		System.out.println("Извлеченный IV: " + HexFormat.of().formatHex(iv));

		byte[] decryptedBytes = decrypt(cipherText, key, iv);
		String decryptedText = new String(decryptedBytes);

		System.out.println("\nРезультат (открытый текст): " + decryptedText);
		System.out.println(
			"Результат в HEX: " + HexFormat.of().formatHex(decryptedBytes)
		);

		if (modeChoice == 2) {
			try (
				FileOutputStream fos = new FileOutputStream(
					"decrypted_file.txt"
				)
			) {
				fos.write(decryptedBytes);
			}
			System.out.println(
				"Расшифрованный файл сохранен как decrypted_file.txt"
			);
		}
	}

	private static SecretKey generateAndSaveKey() throws Exception {
		KeyGenerator keyGen = KeyGenerator.getInstance(ALGORITHM);
		keyGen.init(KEY_SIZE);
		SecretKey key = keyGen.generateKey();
		try (FileOutputStream fos = new FileOutputStream(KEY_FILE)) {
			fos.write(key.getEncoded());
		}
		return key;
	}

	private static byte[] generateIv() {
		byte[] iv = new byte[IV_SIZE];
		new SecureRandom().nextBytes(iv);
		return iv;
	}

	private static byte[] encrypt(byte[] plaintext, SecretKey key, byte[] iv)
		throws Exception {
		Cipher cipher = Cipher.getInstance(TRANSFORMATION);
		cipher.init(Cipher.ENCRYPT_MODE, key, new IvParameterSpec(iv));
		return cipher.doFinal(plaintext);
	}

	private static byte[] decrypt(byte[] ciphertext, SecretKey key, byte[] iv)
		throws Exception {
		Cipher cipher = Cipher.getInstance(TRANSFORMATION);
		cipher.init(Cipher.DECRYPT_MODE, key, new IvParameterSpec(iv));
		return cipher.doFinal(ciphertext);
	}
}
