import java.util.ArrayList;
import java.util.List;

public class AesCbcCipher {

	private final AesBlockCipher blockCipher;

	public AesCbcCipher() {
		this.blockCipher = new AesBlockCipher();
	}

	// Performs AES-128 encryption in CBC mode
	public List<Byte> encryptCbc(List<Byte> plaintext, byte[] key, byte[] iv) {
		List<Byte> ciphertext = new ArrayList<>();
		byte[] prevBlock = iv; // Initial IV

		System.out.print("Key used: ");
		printBytes(key);
		System.out.print("IV used: ");
		printBytes(iv);

		// Process plaintext in 16-byte blocks
		for (int i = 0; i < plaintext.size(); i += 16) {
			List<Byte> currentBlockList = plaintext.subList(i, i + 16);
			byte[] currentBlock = toPrimitiveByteArray(currentBlockList);

			System.out.println(
				"\n--- Encrypting block " + ((i / 16) + 1) + " ---"
			);
			System.out.print("Original block: ");
			printBytes(currentBlock);
			System.out.print("Previous encrypted block (or IV): ");
			printBytes(prevBlock);

			// XOR with previous encrypted block (or IV for the first block)
			byte[] xoredBlock = new byte[16];
			for (int j = 0; j < 16; j++) {
				xoredBlock[j] = (byte) (currentBlock[j] ^ prevBlock[j]);
			}
			System.out.print("Block after XOR with previous block/IV: ");
			printBytes(xoredBlock);

			// Encrypt the XORed block
			byte[] encryptedBlock = blockCipher.encryptBlock(xoredBlock, key);

			// Add the encrypted block to the resulting ciphertext
			for (byte b : encryptedBlock) {
				ciphertext.add(b);
			}

			// Update prevBlock for the next round
			prevBlock = encryptedBlock;
		}
		return ciphertext;
	}

	// Performs AES-128 decryption in CBC mode
	public List<Byte> decryptCbc(List<Byte> ciphertext, byte[] key, byte[] iv) {
		List<Byte> decryptedText = new ArrayList<>();
		byte[] prevCipherBlock = iv; // Initial IV

		System.out.print("Key used: ");
		printBytes(key);
		System.out.print("IV used: ");
		printBytes(iv);

		// Process ciphertext in 16-byte blocks
		for (int i = 0; i < ciphertext.size(); i += 16) {
			List<Byte> currentCipherBlockList = ciphertext.subList(i, i + 16);
			byte[] currentCipherBlock = toPrimitiveByteArray(
				currentCipherBlockList
			);

			System.out.println(
				"\n--- Decrypting block " + ((i / 16) + 1) + " ---"
			);
			System.out.print("Current encrypted block: ");
			printBytes(currentCipherBlock);
			System.out.print("Previous encrypted block (or IV): ");
			printBytes(prevCipherBlock);

			// Decrypt the current encrypted block
			byte[] decryptedBlock = blockCipher.decryptBlock(
				currentCipherBlock,
				key
			);

			// XOR with previous encrypted block (or IV for the first block)
			byte[] finalDecryptedBlock = new byte[16];
			for (int j = 0; j < 16; j++) {
				finalDecryptedBlock[j] = (byte) (decryptedBlock[j] ^
					prevCipherBlock[j]);
			}
			System.out.print(
				"Block after XOR with previous encrypted block/IV: "
			);
			printBytes(finalDecryptedBlock);

			// Add the decrypted block to the resulting text
			for (byte b : finalDecryptedBlock) {
				decryptedText.add(b);
			}

			// Update prevCipherBlock for the next round
			prevCipherBlock = currentCipherBlock;
		}
		return decryptedText;
	}

	// Helper to convert List<Byte> to byte[]
	private byte[] toPrimitiveByteArray(List<Byte> byteList) {
		byte[] byteArray = new byte[byteList.size()];
		for (int i = 0; i < byteList.size(); i++) {
			byteArray[i] = byteList.get(i);
		}
		return byteArray;
	}

	// Prints bytes in hexadecimal format
	public static void printBytes(byte[] bytes) {
		for (byte b : bytes) {
			System.out.printf("%02x ", b);
		}
		System.out.println();
	}
}
