#include "library.h"

// Таблица S-блока для AES
const uint8_t sBox[256] = {
	0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
	0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
	0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
	0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
	0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
	0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
	0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
	0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
	0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
	0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
	0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
	0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
	0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
	0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
	0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
	0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16};

// Таблица обратного S-блока для AES
const uint8_t invSBox[256] = {
	0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
	0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
	0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
	0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
	0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
	0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
	0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
	0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
	0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
	0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
	0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
	0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
	0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
	0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
	0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
	0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d};

// Таблица для умножения на x (0x02) в GF(2^8)
const uint8_t gf28MulX[256] = {
	0x00, 0x02, 0x04, 0x06, 0x08, 0x0a, 0x0c, 0x0e, 0x10, 0x12, 0x14, 0x16, 0x18, 0x1a, 0x1c, 0x1e,
	0x20, 0x22, 0x24, 0x26, 0x28, 0x2a, 0x2c, 0x2e, 0x30, 0x32, 0x34, 0x36, 0x38, 0x3a, 0x3c, 0x3e,
	0x40, 0x42, 0x44, 0x46, 0x48, 0x4a, 0x4c, 0x4e, 0x50, 0x52, 0x54, 0x56, 0x58, 0x5a, 0x5c, 0x5e,
	0x60, 0x62, 0x64, 0x66, 0x68, 0x6a, 0x6c, 0x6e, 0x70, 0x72, 0x74, 0x76, 0x78, 0x7a, 0x7c, 0x7e,
	0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c, 0x8e, 0x90, 0x92, 0x94, 0x96, 0x98, 0x9a, 0x9c, 0x9e,
	0xa0, 0xa2, 0xa4, 0xa6, 0xa8, 0xaa, 0xac, 0xae, 0xb0, 0xb2, 0xb4, 0xb6, 0xb8, 0xba, 0xbc, 0xbe,
	0xc0, 0xc2, 0xc4, 0xc6, 0xc8, 0xca, 0xcc, 0xce, 0xd0, 0xd2, 0xd4, 0xd6, 0xd8, 0xda, 0xdc, 0xde,
	0xe0, 0xe2, 0xe4, 0xe6, 0xe8, 0xea, 0xec, 0xee, 0xf0, 0xf2, 0xf4, 0xf6, 0xf8, 0xfa, 0xfc, 0xfe,
	0x1b, 0x19, 0x1f, 0x1d, 0x15, 0x17, 0x11, 0x13, 0x0b, 0x09, 0x0f, 0x0d, 0x05, 0x07, 0x01, 0x03,
	0x3b, 0x39, 0x3f, 0x3d, 0x35, 0x37, 0x31, 0x33, 0x2b, 0x29, 0x2f, 0x2d, 0x25, 0x27, 0x21, 0x23,
	0x5b, 0x59, 0x5f, 0x5d, 0x55, 0x57, 0x51, 0x53, 0x4b, 0x49, 0x4f, 0x4d, 0x45, 0x47, 0x41, 0x43,
	0x7b, 0x79, 0x7f, 0x7d, 0x75, 0x77, 0x71, 0x73, 0x6b, 0x69, 0x6f, 0x6d, 0x65, 0x67, 0x61, 0x63,
	0x9b, 0x99, 0x9f, 0x9d, 0x95, 0x97, 0x91, 0x93, 0x8b, 0x89, 0x8f, 0x8d, 0x85, 0x87, 0x81, 0x83,
	0xbb, 0xb9, 0xbf, 0xbd, 0xb5, 0xb7, 0xb1, 0xb3, 0xab, 0xa9, 0xaf, 0xad, 0xa5, 0xa7, 0xa1, 0xa3,
	0xdb, 0xd9, 0xdf, 0xdd, 0xd5, 0xd7, 0xd1, 0xd3, 0xcb, 0xc9, 0xcf, 0xcd, 0xc5, 0xc7, 0xc1, 0xc3,
	0xfb, 0xf9, 0xff, 0xfd, 0xf5, 0xf7, 0xf1, 0xf3, 0xeb, 0xe9, 0xef, 0xed, 0xe5, 0xe7, 0xe1, 0xe3};

// Таблица для умножения на x^-1 (0x0b) в GF(2^8)
const uint8_t gf28MulXInv[256] = {
	0x00, 0x0B, 0x16, 0x1D, 0x2C, 0x27, 0x3A, 0x31, 0x58, 0x53, 0x4E, 0x45, 0x74, 0x7F, 0x62, 0x69,
	0xB0, 0xBB, 0xA6, 0xAD, 0x9C, 0x97, 0x8A, 0x81, 0xE8, 0xE3, 0xFE, 0xF5, 0xC4, 0xCF, 0xD2, 0xD9,
	0x67, 0x6C, 0x71, 0x7A, 0x4B, 0x40, 0x5D, 0x56, 0x3F, 0x34, 0x29, 0x22, 0x13, 0x18, 0x05, 0x0E,
	0xD7, 0xDC, 0xC1, 0xCA, 0xFB, 0xF0, 0xED, 0xE6, 0x8F, 0x84, 0x99, 0x92, 0xA3, 0xA8, 0xB5, 0xBE,
	0xCE, 0xC5, 0xD8, 0xD3, 0xE2, 0xE9, 0xF4, 0xFF, 0x96, 0x9D, 0x80, 0x8B, 0xBC, 0xB7, 0xAA, 0xA1,
	0x78, 0x73, 0x6E, 0x65, 0x54, 0x5F, 0x42, 0x49, 0x20, 0x2B, 0x36, 0x3D, 0x0C, 0x07, 0x1A, 0x11,
	0xAF, 0xA4, 0xB9, 0xB2, 0x83, 0x88, 0x95, 0x9E, 0xF7, 0xFC, 0xE1, 0xEA, 0xDB, 0xD0, 0xCD, 0xC6,
	0x1F, 0x14, 0x09, 0x02, 0x33, 0x38, 0x25, 0x2E, 0x47, 0x4C, 0x51, 0x5A, 0x6B, 0x60, 0x7D, 0x76,
	0xE1, 0xEA, 0xF7, 0xFC, 0xCD, 0xC6, 0xD1, 0xDA, 0xB3, 0xB8, 0xA5, 0xAE, 0x9F, 0x94, 0x89, 0x82,
	0x5B, 0x50, 0x4D, 0x46, 0x77, 0x7C, 0x61, 0x6A, 0x03, 0x08, 0x15, 0x1E, 0x2F, 0x24, 0x39, 0x32,
	0x8C, 0x87, 0x9A, 0x91, 0xA0, 0xAB, 0xB6, 0xBD, 0xD4, 0xDF, 0xC2, 0xC9, 0xF8, 0xF3, 0xEE, 0xE5,
	0x3C, 0x37, 0x2A, 0x21, 0x10, 0x1B, 0x06, 0x0D, 0x64, 0x6F, 0x72, 0x79, 0x48, 0x43, 0x5E, 0x55,
	0x26, 0x2D, 0x30, 0x3B, 0x0A, 0x01, 0x1C, 0x17, 0x7E, 0x75, 0x68, 0x63, 0x52, 0x59, 0x44, 0x4F,
	0x96, 0x9D, 0x80, 0x8B, 0xBC, 0xB7, 0xAA, 0xA1, 0xC8, 0xC3, 0xDE, 0xD5, 0xE4, 0xEF, 0xF2, 0xF9,
	0x45, 0x4E, 0x53, 0x58, 0x69, 0x62, 0x7F, 0x74, 0x1D, 0x16, 0x0B, 0x00, 0x31, 0x3A, 0x27, 0x2C,
	0xF5, 0xFE, 0xE3, 0xE8, 0xD9, 0xD2, 0xCF, 0xC4, 0xAD, 0xA6, 0xBB, 0xB0, 0x81, 0x8A, 0x97, 0x9C};

// Определение addPkcs7Padding
std::vector<uint8_t> addPkcs7Padding(const std::vector<uint8_t> &data) {
	size_t blockSize = 16;
	size_t paddingBytes = blockSize - (data.size() % blockSize);
	if (paddingBytes == 0) {
		paddingBytes = blockSize; // Добавляем целый блок, если уже кратно
	}

	std::vector<uint8_t> paddedData = data;
	paddedData.reserve(data.size() + paddingBytes);
	for (size_t i = 0; i < paddingBytes; ++i) {
		paddedData.push_back(static_cast<uint8_t>(paddingBytes));
	}
	return paddedData;
}

// Определение removePkcs7Padding
std::vector<uint8_t> removePkcs7Padding(const std::vector<uint8_t> &paddedData) {
	if (paddedData.empty()) {
		return {};
	}
	uint8_t paddingBytes = paddedData.back();
	// Проверка на корректность значения paddingBytes (не может быть 0 и не должно быть больше
	// размера блока)
	if (paddingBytes == 0 || paddingBytes > 16 || paddingBytes > paddedData.size()) {
		std::cerr << "Предупреждение: Некорректное значение отступа ("
				  << static_cast<int>(paddingBytes) << ") при расшифровке." << std::endl;
		return paddedData; // Возвращаем как есть или обрабатываем ошибку
	}
	// Дополнительная проверка: все ли байты отступа имеют ожидаемое значение
	for (size_t i = 1; i <= paddingBytes; ++i) {
		if (paddedData[paddedData.size() - i] != paddingBytes) {
			std::cerr << "Предупреждение: Несоответствие байтов отступа при расшифровке. Ожидалось "
					  << static_cast<int>(paddingBytes) << ", получено "
					  << static_cast<int>(paddedData[paddedData.size() - i]) << std::endl;
			return paddedData; // Возвращаем как есть или обрабатываем ошибку
		}
	}
	return std::vector<uint8_t>(paddedData.begin(), paddedData.end() - paddingBytes);
}

// Вычисляет R_con для заданного раунда
uint8_t getRcon(uint8_t round) {
	uint8_t rconVal = 1;
	for (uint8_t i = 1; i < round; ++i) {
		rconVal = gf28MulX[rconVal]; // Умножение на 2 в GF(2^8)
	}
	return rconVal;
}

// Преобразует вектор байтов в матрицу состояния
State bytesToMatrix(const std::vector<uint8_t> &bytes) {
	State state;
	for (size_t i = 0; i < 16; ++i) {
		state.matrix[i % 4][i / 4] = bytes[i];
	}
	return state;
}

// Преобразует матрицу состояния в вектор байтов
std::vector<uint8_t> matrixToBytes(const State &state) {
	std::vector<uint8_t> bytes(16);
	for (size_t i = 0; i < 16; ++i) {
		bytes[i] = state.matrix[i % 4][i / 4];
	}
	return bytes;
}

// Выполняет побайтовое замещение с использованием S-блока
void subBytes(State &state) {
	for (size_t row = 0; row < 4; ++row) {
		for (size_t col = 0; col < 4; ++col) {
			state.matrix[row][col] = sBox[state.matrix[row][col]];
		}
	}
}

// Выполняет обратное побайтовое замещение с использованием обратного S-блока
void invSubBytes(State &state) {
	for (size_t row = 0; row < 4; ++row) {
		for (size_t col = 0; col < 4; ++col) {
			state.matrix[row][col] = invSBox[state.matrix[row][col]];
		}
	}
}

// Выполняет циклический сдвиг строк
void shiftRows(State &state) {
	// Первая строка остается без изменений
	// Вторая строка: сдвиг на 1 байт влево
	uint8_t tempByte = state.matrix[1][0];
	state.matrix[1][0] = state.matrix[1][1];
	state.matrix[1][1] = state.matrix[1][2];
	state.matrix[1][2] = state.matrix[1][3];
	state.matrix[1][3] = tempByte;

	// Третья строка: сдвиг на 2 байта влево
	tempByte = state.matrix[2][0];
	uint8_t tempByte2 = state.matrix[2][1];
	state.matrix[2][0] = state.matrix[2][2];
	state.matrix[2][1] = state.matrix[2][3];
	state.matrix[2][2] = tempByte;
	state.matrix[2][3] = tempByte2;

	// Четвертая строка: сдвиг на 3 байта влево
	tempByte = state.matrix[3][0];
	tempByte2 = state.matrix[3][1];
	uint8_t tempByte3 = state.matrix[3][2];
	state.matrix[3][0] = state.matrix[3][3];
	state.matrix[3][1] = tempByte;
	state.matrix[3][2] = tempByte2;
	state.matrix[3][3] = tempByte3;
}

// Выполняет обратный циклический сдвиг строк
void invShiftRows(State &state) {
	// Первая строка остается без изменений
	// Вторая строка: сдвиг на 1 байт вправо
	uint8_t tempByte = state.matrix[1][3];
	state.matrix[1][3] = state.matrix[1][2];
	state.matrix[1][2] = state.matrix[1][1];
	state.matrix[1][1] = state.matrix[1][0];
	state.matrix[1][0] = tempByte;

	// Третья строка: сдвиг на 2 байта вправо
	tempByte = state.matrix[2][3];
	uint8_t tempByte2 = state.matrix[2][2];
	state.matrix[2][3] = state.matrix[2][1];
	state.matrix[2][2] = state.matrix[2][0];
	state.matrix[2][1] = tempByte;
	state.matrix[2][0] = tempByte2;

	// Четвертая строка: сдвиг на 3 байта вправо
	tempByte = state.matrix[3][3];
	tempByte2 = state.matrix[3][2];
	uint8_t tempByte3 = state.matrix[3][1];
	state.matrix[3][3] = state.matrix[3][0];
	state.matrix[3][2] = tempByte;
	state.matrix[3][1] = tempByte2;
	state.matrix[3][0] = tempByte3;
}

// Умножение в поле GF(2^8)
uint8_t gfMul(uint8_t a, uint8_t b) {
	uint8_t p = 0;
	uint8_t hiBitSet;
	for (int i = 0; i < 8; ++i) {
		if ((b & 1) == 1) {
			p ^= a;
		}
		hiBitSet = (a & 0x80);
		a <<= 1;
		if (hiBitSet == 0x80) {
			a ^= 0x1b; // XOR с неприводимым полиномом x^8 + x^4 + x^3 + x + 1
		}
		b >>= 1;
	}
	return p;
}

// Перемешивает столбцы
void mixColumns(State &state) {
	for (size_t col = 0; col < 4; ++col) {
		uint8_t s0 = state.matrix[0][col];
		uint8_t s1 = state.matrix[1][col];
		uint8_t s2 = state.matrix[2][col];
		uint8_t s3 = state.matrix[3][col];

		state.matrix[0][col] = gfMul(0x02, s0) ^ gfMul(0x03, s1) ^ s2 ^ s3;
		state.matrix[1][col] = s0 ^ gfMul(0x02, s1) ^ gfMul(0x03, s2) ^ s3;
		state.matrix[2][col] = s0 ^ s1 ^ gfMul(0x02, s2) ^ gfMul(0x03, s3);
		state.matrix[3][col] = gfMul(0x03, s0) ^ s1 ^ s2 ^ gfMul(0x02, s3);
	}
}

// Выполняет обратное перемешивание столбцов
void invMixColumns(State &state) {
	for (size_t col = 0; col < 4; ++col) {
		uint8_t s0 = state.matrix[0][col];
		uint8_t s1 = state.matrix[1][col];
		uint8_t s2 = state.matrix[2][col];
		uint8_t s3 = state.matrix[3][col];

		state.matrix[0][col] =
			gfMul(0x0e, s0) ^ gfMul(0x0b, s1) ^ gfMul(0x0d, s2) ^ gfMul(0x09, s3);
		state.matrix[1][col] =
			gfMul(0x09, s0) ^ gfMul(0x0e, s1) ^ gfMul(0x0b, s2) ^ gfMul(0x0d, s3);
		state.matrix[2][col] =
			gfMul(0x0d, s0) ^ gfMul(0x09, s1) ^ gfMul(0x0e, s2) ^ gfMul(0x0b, s3);
		state.matrix[3][col] =
			gfMul(0x0b, s0) ^ gfMul(0x0d, s1) ^ gfMul(0x09, s2) ^ gfMul(0x0e, s3);
	}
}

// Добавляет раундовый ключ к состоянию
void addRoundKey(State &state, const std::vector<uint8_t> &roundKey) {
	for (size_t row = 0; row < 4; ++row) {
		for (size_t col = 0; col < 4; ++col) {
			state.matrix[row][col] ^= roundKey[row + 4 * col];
		}
	}
}

// Расширяет 128-битный ключ до всех раундовых ключей
std::vector<std::vector<uint8_t>> expandKey(const std::vector<uint8_t> &key) {
	// Количество раундов для 128-битного ключа
	const size_t numRounds = 10;
	// Количество байтов в расширенном ключе (16 байт * (10 раундов + 1 начальный ключ))
	const size_t expandedKeySize = 16 * (numRounds + 1);
	std::vector<uint8_t> expandedKey(expandedKeySize);

	// Копируем исходный ключ
	for (size_t i = 0; i < 16; ++i) {
		expandedKey[i] = key[i];
	}

	uint8_t tempBytes[4];
	for (size_t i = 16; i < expandedKeySize; ++i) {
		tempBytes[0] = expandedKey[i - 4];
		tempBytes[1] = expandedKey[i - 3];
		tempBytes[2] = expandedKey[i - 2];
		tempBytes[3] = expandedKey[i - 1];

		if (i % 16 == 0) {
			// RotWord: циклический сдвиг
			uint8_t temp = tempBytes[0];
			tempBytes[0] = tempBytes[1];
			tempBytes[1] = tempBytes[2];
			tempBytes[2] = tempBytes[3];
			tempBytes[3] = temp;

			// SubWord: замена байтов через S-блок
			tempBytes[0] = sBox[tempBytes[0]];
			tempBytes[1] = sBox[tempBytes[1]];
			tempBytes[2] = sBox[tempBytes[2]];
			tempBytes[3] = sBox[tempBytes[3]];

			// Rcon XOR
			tempBytes[0] ^= getRcon(i / 16);
		}

		expandedKey[i] = expandedKey[i - 16] ^ tempBytes[i % 4];
	}

	// Разделяем расширенный ключ на раундовые ключи
	std::vector<std::vector<uint8_t>> roundKeys(numRounds + 1, std::vector<uint8_t>(16));
	for (size_t round = 0; round <= numRounds; ++round) {
		for (size_t j = 0; j < 16; ++j) {
			roundKeys[round][j] = expandedKey[round * 16 + j];
		}
	}
	return roundKeys;
}

// Выводит байты в шестнадцатеричном формате
void printBytes(const std::vector<uint8_t> &bytes) {
	for (uint8_t byte : bytes) {
		std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte) << " ";
	}
	std::cout << std::dec << std::endl;
}

// Выводит матрицу состояния
void printState(const State &state) {
	for (size_t row = 0; row < 4; ++row) {
		for (size_t col = 0; col < 4; ++col) {
			std::cout << std::hex << std::setw(2) << std::setfill('0')
					  << static_cast<int>(state.matrix[row][col]) << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::dec << std::endl;
}

// Генерирует случайный ключ заданной длины
std::vector<uint8_t> generateRandomKey(size_t length) {
	std::vector<uint8_t> key(length);
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 255);

	for (size_t i = 0; i < length; ++i) {
		key[i] = static_cast<uint8_t>(dis(gen));
	}
	return key;
}

// Генерирует случайный вектор инициализации заданной длины
std::vector<uint8_t> generateRandomIV(size_t length) {
	return generateRandomKey(length); // IV генерируется так же, как и ключ
}

// Записывает ключ в файл
void writeKeyToFile(const std::string &filename, const std::vector<uint8_t> &key) {
	std::ofstream outFile(filename, std::ios::out | std::ios::binary);
	if (outFile.is_open()) {
		outFile.write(reinterpret_cast<const char *>(key.data()), key.size());
		outFile.close();
	} else {
		std::cerr << "Ошибка: Не удалось открыть файл для записи ключа." << std::endl;
	}
}

// Читает ключ из файла
std::vector<uint8_t> readKeyFromFile(const std::string &filename) {
	std::ifstream inFile(filename, std::ios::in | std::ios::binary | std::ios::ate);
	std::vector<uint8_t> key;
	if (inFile.is_open()) {
		std::streampos fileSize = inFile.tellg();
		key.resize(static_cast<size_t>(fileSize));
		inFile.seekg(0, std::ios::beg);
		inFile.read(reinterpret_cast<char *>(key.data()), fileSize);
		inFile.close();
	} else {
		std::cerr << "Ошибка: Не удалось открыть файл для чтения ключа." << std::endl;
	}
	return key;
}

// Выполняет шифрование одного 128-битного блока AES
std::vector<uint8_t> aes128EncryptBlock(const std::vector<uint8_t> &block,
										const std::vector<uint8_t> &key) {
	State state = bytesToMatrix(block);
	std::vector<std::vector<uint8_t>> roundKeys = expandKey(key);
	const size_t numRounds = 10;

	std::cout << "Начальное состояние блока шифрования:" << std::endl;
	printState(state);

	// Начальное XOR с раундовым ключом
	addRoundKey(state, roundKeys[0]);
	std::cout << "Состояние после AddRoundKey (начальный):" << std::endl;
	printState(state);

	// 9 раундов (SubBytes, ShiftRows, MixColumns, AddRoundKey)
	for (size_t round = 1; round < numRounds; ++round) {
		std::cout << "Раунд " << round << " шифрования:" << std::endl;
		subBytes(state);
		std::cout << "После SubBytes:" << std::endl;
		printState(state);

		shiftRows(state);
		std::cout << "После ShiftRows:" << std::endl;
		printState(state);

		mixColumns(state);
		std::cout << "После MixColumns:" << std::endl;
		printState(state);

		addRoundKey(state, roundKeys[round]);
		std::cout << "После AddRoundKey:" << std::endl;
		printState(state);
	}

	// Финальный раунд (SubBytes, ShiftRows, AddRoundKey)
	std::cout << "Финальный раунд " << numRounds << " шифрования:" << std::endl;
	subBytes(state);
	std::cout << "После SubBytes (финальный):" << std::endl;
	printState(state);

	shiftRows(state);
	std::cout << "После ShiftRows (финальный):" << std::endl;
	printState(state);

	addRoundKey(state, roundKeys[numRounds]);
	std::cout << "После AddRoundKey (финальный):" << std::endl;
	printState(state);

	return matrixToBytes(state);
}

// Выполняет расшифрование одного 128-битного блока AES
std::vector<uint8_t> aes128DecryptBlock(const std::vector<uint8_t> &block,
										const std::vector<uint8_t> &key) {
	State state = bytesToMatrix(block);
	std::vector<std::vector<uint8_t>> roundKeys = expandKey(key);
	const size_t numRounds = 10;

	std::cout << "Начальное состояние блока расшифрования:" << std::endl;
	printState(state);

	// Начальное XOR с раундовым ключом (обратный порядок)
	addRoundKey(state, roundKeys[numRounds]);
	std::cout << "Состояние после AddRoundKey (начальный обратный):" << std::endl;
	printState(state);

	// 9 раундов (InvShiftRows, InvSubBytes, InvAddRoundKey, InvMixColumns)
	for (size_t round = numRounds - 1; round >= 1; --round) {
		std::cout << "Раунд " << round << " расшифрования:" << std::endl;
		invShiftRows(state);
		std::cout << "После InvShiftRows:" << std::endl;
		printState(state);

		invSubBytes(state);
		std::cout << "После InvSubBytes:" << std::endl;
		printState(state);

		addRoundKey(state, roundKeys[round]);
		std::cout << "После AddRoundKey:" << std::endl;
		printState(state);

		invMixColumns(state);
		std::cout << "После InvMixColumns:" << std::endl;
		printState(state);
	}

	// Финальный раунд (InvShiftRows, InvSubBytes, AddRoundKey)
	std::cout << "Финальный раунд " << 0 << " расшифрования:" << std::endl;
	invShiftRows(state);
	std::cout << "После InvShiftRows (финальный):" << std::endl;
	printState(state);

	invSubBytes(state);
	std::cout << "После InvSubBytes (финальный):" << std::endl;
	printState(state);

	addRoundKey(state, roundKeys[0]);
	std::cout << "После AddRoundKey (финальный):" << std::endl;
	printState(state);

	return matrixToBytes(state);
}

// Выполняет шифрование AES-128 в режиме CBC
std::vector<uint8_t> aes128EncryptCbc(const std::vector<uint8_t> &plaintext,
									  const std::vector<uint8_t> &key,
									  const std::vector<uint8_t> &iv) {
	std::vector<uint8_t> ciphertext;
	std::vector<uint8_t> prevBlock = iv; // Начальный вектор инициализации

	std::cout << "Используемый ключ: ";
	printBytes(key);
	std::cout << "Используемый IV: ";
	printBytes(iv);

	// Проход по блокам текста (по 16 байт)
	for (size_t i = 0; i < plaintext.size(); i += 16) {
		std::vector<uint8_t> currentBlock(plaintext.begin() + i, plaintext.begin() + i + 16);

		std::cout << std::endl << "--- Шифрование блока " << (i / 16) + 1 << " ---" << std::endl;
		std::cout << "Исходный блок: ";
		printBytes(currentBlock);
		std::cout << "Предыдущий зашифрованный блок (или IV): ";
		printBytes(prevBlock);

		// XOR с предыдущим зашифрованным блоком (или IV для первого блока)
		std::vector<uint8_t> xoredBlock(16);
		for (size_t j = 0; j < 16; ++j) {
			xoredBlock[j] = currentBlock[j] ^ prevBlock[j];
		}
		std::cout << "Блок после XOR с предыдущим блоком/IV: ";
		printBytes(xoredBlock);

		// Шифрование XORed блока
		std::vector<uint8_t> encryptedBlock = aes128EncryptBlock(xoredBlock, key);

		// Добавляем зашифрованный блок к результирующему шифротексту
		ciphertext.insert(ciphertext.end(), encryptedBlock.begin(), encryptedBlock.end());

		// Обновляем prevBlock для следующего раунда
		prevBlock = encryptedBlock;
	}
	return ciphertext;
}

// Выполняет расшифрование AES-128 в режиме CBC
std::vector<uint8_t> aes128DecryptCbc(const std::vector<uint8_t> &ciphertext,
									  const std::vector<uint8_t> &key,
									  const std::vector<uint8_t> &iv) {
	std::vector<uint8_t> decryptedText;
	std::vector<uint8_t> prevCipherBlock = iv; // Начальный вектор инициализации

	std::cout << "Используемый ключ: ";
	printBytes(key);
	std::cout << "Используемый IV: ";
	printBytes(iv);

	// Проход по блокам шифротекста (по 16 байт)
	for (size_t i = 0; i < ciphertext.size(); i += 16) {
		std::vector<uint8_t> currentCipherBlock(ciphertext.begin() + i,
												ciphertext.begin() + i + 16);

		std::cout << std::endl << "--- Расшифрование блока " << (i / 16) + 1 << " ---" << std::endl;
		std::cout << "Текущий зашифрованный блок: ";
		printBytes(currentCipherBlock);
		std::cout << "Предыдущий зашифрованный блок (или IV): ";
		printBytes(prevCipherBlock);

		// Расшифрование текущего зашифрованного блока
		std::vector<uint8_t> decryptedBlock = aes128DecryptBlock(currentCipherBlock, key);

		// XOR с предыдущим зашифрованным блоком (или IV для первого блока)
		std::vector<uint8_t> finalDecryptedBlock(16);
		for (size_t j = 0; j < 16; ++j) {
			finalDecryptedBlock[j] = decryptedBlock[j] ^ prevCipherBlock[j];
		}
		std::cout << "Блок после XOR с предыдущим зашифрованным блоком/IV: ";
		printBytes(finalDecryptedBlock);

		// Добавляем расшифрованный блок к результирующему тексту
		decryptedText.insert(decryptedText.end(), finalDecryptedBlock.begin(),
							 finalDecryptedBlock.end());

		// Обновляем prevCipherBlock для следующего раунда
		prevCipherBlock = currentCipherBlock;
	}
	return decryptedText;
}
