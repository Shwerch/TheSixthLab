#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <random>
#include <string>
#include <vector>

using byte = unsigned char;

using AesBlock = std::vector<byte>;

const byte s_box[256] = {
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
const byte inv_s_box[256] = {
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
const byte Rcon[11] = {0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};

AesBlock hex_string_to_bytes(const std::string &hex) {
	AesBlock bytes;
	std::string current_hex = hex;
	current_hex.erase(std::remove_if(current_hex.begin(), current_hex.end(), ::isspace),
					  current_hex.end());
	if (current_hex.length() % 2 != 0)
		return {};
	for (size_t i = 0; i < current_hex.length(); i += 2) {
		std::string byte_string = current_hex.substr(i, 2);
		try {
			bytes.push_back(static_cast<byte>(std::stoul(byte_string, nullptr, 16)));
		} catch (const std::exception &e) {
			return {};
		}
	}
	return bytes;
}
void print_hex(const AesBlock &data, const std::string &title) {
	std::cout << title << ": ";
	for (const auto &byte : data) {
		std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
	}
	std::cout << std::dec << std::endl;
}
void print_state(const AesBlock &state, const std::string &title) {
	std::cout << title << ":\n";
	for (int r = 0; r < 4; ++r) {
		for (int c = 0; c < 4; ++c) {
			std::cout << std::hex << std::setw(2) << std::setfill('0')
					  << static_cast<int>(state[r + c * 4]) << " ";
		}
		std::cout << "\n";
	}
	std::cout << std::dec;
}
AesBlock generate_random_block(size_t size) {
	AesBlock block(size);
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(0, 255);
	for (size_t i = 0; i < size; ++i) {
		block[i] = static_cast<byte>(distrib(gen));
	}
	return block;
}

void sub_bytes(AesBlock &state) {
	for (auto &byte : state)
		byte = s_box[byte];
}
void inv_sub_bytes(AesBlock &state) {
	for (auto &byte : state)
		byte = inv_s_box[byte];
}
void shift_rows(AesBlock &state) {
	byte temp;
	temp = state[1];
	state[1] = state[5];
	state[5] = state[9];
	state[9] = state[13];
	state[13] = temp;
	temp = state[2];
	state[2] = state[10];
	state[10] = temp;
	temp = state[6];
	state[6] = state[14];
	state[14] = temp;
	temp = state[15];
	state[15] = state[11];
	state[11] = state[7];
	state[7] = state[3];
	state[3] = temp;
}
void inv_shift_rows(AesBlock &state) {
	byte temp;
	temp = state[13];
	state[13] = state[9];
	state[9] = state[5];
	state[5] = state[1];
	state[1] = temp;
	temp = state[2];
	state[2] = state[10];
	state[10] = temp;
	temp = state[6];
	state[6] = state[14];
	state[14] = temp;
	temp = state[3];
	state[3] = state[7];
	state[7] = state[11];
	state[11] = state[15];
	state[15] = temp;
}
byte gmul(byte a, byte b) {
	byte p = 0;
	for (int i = 0; i < 8; ++i) {
		if (b & 1)
			p ^= a;
		bool hi_bit_set = (a & 0x80);
		a <<= 1;
		if (hi_bit_set)
			a ^= 0x1b;
		b >>= 1;
	}
	return p;
}
void mix_columns(AesBlock &state) {
	AesBlock temp_state = state;
	for (int i = 0; i < 4; ++i) {
		state[i * 4 + 0] = gmul(temp_state[i * 4 + 0], 2) ^ gmul(temp_state[i * 4 + 1], 3) ^
						   temp_state[i * 4 + 2] ^ temp_state[i * 4 + 3];
		state[i * 4 + 1] = temp_state[i * 4 + 0] ^ gmul(temp_state[i * 4 + 1], 2) ^
						   gmul(temp_state[i * 4 + 2], 3) ^ temp_state[i * 4 + 3];
		state[i * 4 + 2] = temp_state[i * 4 + 0] ^ temp_state[i * 4 + 1] ^
						   gmul(temp_state[i * 4 + 2], 2) ^ gmul(temp_state[i * 4 + 3], 3);
		state[i * 4 + 3] = gmul(temp_state[i * 4 + 0], 3) ^ temp_state[i * 4 + 1] ^
						   temp_state[i * 4 + 2] ^ gmul(temp_state[i * 4 + 3], 2);
	}
}
void inv_mix_columns(AesBlock &state) {
	AesBlock temp_state = state;
	for (int i = 0; i < 4; ++i) {
		state[i * 4 + 0] = gmul(temp_state[i * 4 + 0], 14) ^ gmul(temp_state[i * 4 + 1], 11) ^
						   gmul(temp_state[i * 4 + 2], 13) ^ gmul(temp_state[i * 4 + 3], 9);
		state[i * 4 + 1] = gmul(temp_state[i * 4 + 0], 9) ^ gmul(temp_state[i * 4 + 1], 14) ^
						   gmul(temp_state[i * 4 + 2], 11) ^ gmul(temp_state[i * 4 + 3], 13);
		state[i * 4 + 2] = gmul(temp_state[i * 4 + 0], 13) ^ gmul(temp_state[i * 4 + 1], 9) ^
						   gmul(temp_state[i * 4 + 2], 14) ^ gmul(temp_state[i * 4 + 3], 11);
		state[i * 4 + 3] = gmul(temp_state[i * 4 + 0], 11) ^ gmul(temp_state[i * 4 + 1], 13) ^
						   gmul(temp_state[i * 4 + 2], 9) ^ gmul(temp_state[i * 4 + 3], 14);
	}
}
void add_round_key(AesBlock &state, const AesBlock &round_key) {
	for (int i = 0; i < 16; ++i)
		state[i] ^= round_key[i];
}

std::vector<AesBlock> key_expansion(const AesBlock &key) {
	std::vector<AesBlock> round_keys(11, AesBlock(16));
	round_keys[0] = key;
	for (int i = 1; i <= 10; ++i) {
		AesBlock temp(4);
		for (int k = 0; k < 4; k++)
			temp[k] = round_keys[i - 1][12 + k];
		byte t = temp[0];
		temp[0] = temp[1];
		temp[1] = temp[2];
		temp[2] = temp[3];
		temp[3] = t;
		for (int k = 0; k < 4; k++)
			temp[k] = s_box[temp[k]];
		temp[0] ^= Rcon[i];
		for (int j = 0; j < 4; ++j)
			round_keys[i][j] = round_keys[i - 1][j] ^ temp[j];
		for (int j = 4; j < 16; ++j)
			round_keys[i][j] = round_keys[i - 1][j] ^ round_keys[i][j - 4];
	}
	return round_keys;
}

void block_to_state(const AesBlock &block, AesBlock &state) {
	for (int r = 0; r < 4; ++r)
		for (int c = 0; c < 4; ++c)
			state[r + c * 4] = block[r * 4 + c];
}
void state_to_block(const AesBlock &state, AesBlock &block) {
	for (int r = 0; r < 4; ++r)
		for (int c = 0; c < 4; ++c)
			block[r * 4 + c] = state[r + c * 4];
}
void encrypt_block(AesBlock &block, const std::vector<AesBlock> &round_keys) {
	AesBlock state(16);
	block_to_state(block, state);
	add_round_key(state, round_keys[0]);
	print_state(state, "State after Initial Round");
	for (int round = 1; round < 10; ++round) {
		sub_bytes(state);
		shift_rows(state);
		mix_columns(state);
		add_round_key(state, round_keys[round]);
		print_state(state, "State after Round " + std::to_string(round));
	}
	sub_bytes(state);
	shift_rows(state);
	add_round_key(state, round_keys[10]);
	print_state(state, "State after Final Round");
	state_to_block(state, block);
}

void decrypt_block(AesBlock &block, const std::vector<AesBlock> &round_keys) {
	AesBlock state(16);
	block_to_state(block, state);
	add_round_key(state, round_keys[10]);
	print_state(state, "State after Initial Round (Decryption)");
	for (int round = 9; round >= 1; --round) {
		inv_shift_rows(state);
		inv_sub_bytes(state);
		add_round_key(state, round_keys[round]);
		print_state(state, "State before InvMixColumns (Round " + std::to_string(round) + ")");
		inv_mix_columns(state);
	}
	inv_shift_rows(state);
	inv_sub_bytes(state);
	add_round_key(state, round_keys[0]);
	print_state(state, "State after Final Round (Decryption)");
	state_to_block(state, block);
}

AesBlock pkcs7_pad(const AesBlock &data) {
	AesBlock padded_data = data;
	size_t pad_size = 16 - (data.size() % 16);
	for (size_t i = 0; i < pad_size; ++i)
		padded_data.push_back(static_cast<byte>(pad_size));
	return padded_data;
}
AesBlock pkcs7_unpad(const AesBlock &data) {
	if (data.empty())
		return {};
	byte pad_size = data.back();
	if (pad_size == 0 || pad_size > 16 || pad_size > data.size())
		return data;
	for (size_t i = 0; i < pad_size; ++i)
		if (data[data.size() - 1 - i] != pad_size)
			return data;
	return AesBlock(data.begin(), data.end() - pad_size);
}

AesBlock encrypt_cbc(AesBlock &plaintext, const std::vector<AesBlock> &round_keys, AesBlock &iv) {
	AesBlock padded_plaintext = pkcs7_pad(plaintext);
	AesBlock ciphertext;
	AesBlock prev_block = iv;
	for (size_t i = 0; i < padded_plaintext.size(); i += 16) {
		AesBlock block(padded_plaintext.begin() + i, padded_plaintext.begin() + i + 16);
		add_round_key(block, prev_block);
		std::cout << "\n--- Encrypting Block " << (i / 16) + 1 << " ---\n";
		encrypt_block(block, round_keys);
		ciphertext.insert(ciphertext.end(), block.begin(), block.end());
		prev_block = block;
	}
	return ciphertext;
}
AesBlock decrypt_cbc(AesBlock &ciphertext, const std::vector<AesBlock> &round_keys, AesBlock &iv) {
	AesBlock plaintext;
	AesBlock prev_block = iv;
	for (size_t i = 0; i < ciphertext.size(); i += 16) {
		AesBlock block(ciphertext.begin() + i, ciphertext.begin() + i + 16);
		AesBlock temp_block = block;
		std::cout << "\n--- Decrypting Block " << (i / 16) + 1 << " ---\n";
		decrypt_block(block, round_keys);
		add_round_key(block, prev_block);
		plaintext.insert(plaintext.end(), block.begin(), block.end());
		prev_block = temp_block;
	}
	return pkcs7_unpad(plaintext);
}

void runAES() {
	AesBlock key = generate_random_block(16);
	std::ofstream key_file("key.txt", std::ios::out | std::ios::binary);
	if (key_file.is_open()) {
		key_file.write(reinterpret_cast<const char *>(key.data()), key.size());
		key_file.close();
		std::cout << "Сгенерирован ключ и сохранен в key.txt" << std::endl;
	} else {
		std::cerr << "Ошибка: Не удалось открыть key.txt для записи." << std::endl;
		return;
	}
	print_hex(key, "Сгенерированный ключ (Key)");

	std::vector<AesBlock> round_keys = key_expansion(key);
	std::cout << "\n--- Сгенерированные ключи раундов ---\n";
	for (size_t i = 0; i < round_keys.size(); ++i) {
		print_hex(round_keys[i], "Round Key " + std::to_string(i));
	}

	do {
		std::cout << "\nВыберите, что обработать:\n1. Текст\n2. Файл\n> ";
		int mode_choice;
		std::cin >> mode_choice;
		std::cin.ignore();

		std::cout << "Выберите действие:\n1. Зашифровать\n2. Расшифровать\n> ";
		int action_choice;
		std::cin >> action_choice;
		std::cin.ignore();

		AesBlock data_to_process;

		if (mode_choice == 1) { // Текст
			if (action_choice == 1) {
				std::cout << "Введите текст для шифрования: ";
				std::string text_input;
				std::getline(std::cin, text_input);
				data_to_process.assign(text_input.begin(), text_input.end());
			} else {
				std::cout << "Вставьте HEX-строку для расшифровки: ";
				std::string hex_input;
				std::getline(std::cin, hex_input);
				data_to_process = hex_string_to_bytes(hex_input);
			}
		} else if (mode_choice == 2) {
			std::cout << "Введите путь к файлу: ";
			std::string file_path;
			std::getline(std::cin, file_path);
			std::ifstream input_file(file_path, std::ios::binary);
			if (!input_file) {
				std::cerr << "Ошибка: Не удалось открыть файл " << file_path << std::endl;
				return;
			}
			data_to_process.assign(std::istreambuf_iterator<char>(input_file), {});
			input_file.close();
		} else {
			std::cerr << "Неверный выбор." << std::endl;
			return;
		}

		if (action_choice == 1) { // Шифрование
			AesBlock iv = generate_random_block(16);
			print_hex(iv, "Вектор инициализации (IV), используемый для шифрования");

			AesBlock ciphertext = encrypt_cbc(data_to_process, round_keys, iv);

			AesBlock output_with_iv = iv;
			output_with_iv.insert(output_with_iv.end(), ciphertext.begin(), ciphertext.end());

			print_hex(output_with_iv, "\nРезультат (IV + Шифротекст в HEX)");

			if (mode_choice == 2) {
				std::ofstream output_file("encrypted_file.bin", std::ios::binary);
				output_file.write(reinterpret_cast<const char *>(output_with_iv.data()),
								  output_with_iv.size());
				output_file.close();
				std::cout << "Зашифрованный файл (IV + шифротекст) сохранен как encrypted_file.bin"
						  << std::endl;
			}
		} else if (action_choice == 2) { // Дешифрование
			if (data_to_process.size() < 16) {
				std::cerr << "Ошибка: Входные данные слишком малы для расшифровки (отсутствует IV)."
						  << std::endl;
				return;
			}

			AesBlock iv(data_to_process.begin(), data_to_process.begin() + 16);
			AesBlock ciphertext(data_to_process.begin() + 16, data_to_process.end());

			if (ciphertext.empty() || ciphertext.size() % 16 != 0) {
				std::cerr << "Ошибка: Длина шифротекста некорректна (пуста или не кратна 16)."
						  << std::endl;
				return;
			}

			print_hex(iv, "Извлеченный вектор инициализации (IV) для дешифрования");

			AesBlock decrypted_text = decrypt_cbc(ciphertext, round_keys, iv);

			std::cout << "\nРезультат (открытый текст): ";
			for (const auto &c : decrypted_text)
				std::cout << c;
			std::cout << std::endl;
			print_hex(decrypted_text, "Результат в HEX");

			if (mode_choice == 2) {
				std::string out_filename = "decrypted_file.txt";
				std::ofstream output_file(out_filename, std::ios::binary);
				output_file.write(reinterpret_cast<const char *>(decrypted_text.data()),
								  decrypted_text.size());
				output_file.close();
				std::cout << "Расшифрованный файл сохранен как " << out_filename << std::endl;
			}
		} else {
			std::cerr << "Неверный выбор действия." << std::endl;
			return;
		}
	} while (true);
}
