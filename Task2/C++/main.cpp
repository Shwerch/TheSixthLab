#include "library.h"

int main() {
	std::srand(
		static_cast<unsigned int>(std::time(nullptr))); // Инициализация генератора случайных чисел

	std::string inputText;
	std::cout << "Введите текст для шифрования (кратный 16 символам): " << std::endl;
	std::getline(std::cin, inputText);
	/*size_t length = inputText.length();
	if (static_cast<int>(length) % 16 != 0)
	inputText.append(length, '\0');*/

	std::vector<uint8_t> inputBytes(inputText.begin(), inputText.end());

	// Добавление отступа к входным байтам
	std::vector<uint8_t> paddedInputBytes = addPkcs7Padding(inputBytes);

	// Генерация случайного ключа и запись его в файл
	std::vector<uint8_t> aesKey = generateRandomKey(16); // 16 байт для AES-128
	printBytes(aesKey);

	// Генерация случайного вектора инициализации
	std::vector<uint8_t> initVec = generateRandomIV(16); // 16 байт для IV
	std::cout << "Сгенерированный вектор инициализации (IV): ";
	printBytes(initVec);

	std::cout << std::endl << std::string(50, '-') << std::endl;
	std::cout << "Начало шифрования AES-128 (CBC)" << std::endl;
	std::cout << std::string(50, '-') << std::endl;

	// Шифрование
	std::vector<uint8_t> encryptedBytes = aes128EncryptCbc(paddedInputBytes, aesKey, initVec);

	std::cout << std::endl << std::string(50, '-') << std::endl;
	std::cout << "Шифрование завершено" << std::endl;
	std::cout << std::string(50, '-') << std::endl;
	std::cout << "Зашифрованный текст: ";
	printBytes(encryptedBytes);

	std::cout << std::endl << std::string(50, '-') << std::endl;
	std::cout << "Начало расшифрования AES-128 (CBC)" << std::endl;
	std::cout << std::string(50, '-') << std::endl;

	// Расшифрование
	std::vector<uint8_t> decryptedBytes = aes128DecryptCbc(encryptedBytes, aesKey, initVec);

	// Удаление отступа из расшифрованных байтов
	std::vector<uint8_t> finalDecryptedBytes = removePkcs7Padding(decryptedBytes);

	std::cout << std::endl << std::string(50, '-') << std::endl;
	std::cout << "Расшифрование завершено" << std::endl;
	std::cout << std::string(50, '-') << std::endl;
	std::cout << "Расшифрованный текст: " << std::endl;
	for (uint8_t byte : finalDecryptedBytes) {
		std::cout << static_cast<char>(byte);
	}
	std::cout << std::endl;

	return 0;
}
