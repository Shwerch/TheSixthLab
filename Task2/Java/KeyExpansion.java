import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class KeyExpansion {

	// Calculates R_con for a given round
	private byte getRcon(int round) {
		byte rconVal = 1;
		for (int i = 1; i < round; i++) {
			rconVal = AesTables.GF28_MUL_X[rconVal & 0xFF]; // Multiplication by 2 in GF(2^8)
		}
		return rconVal;
	}

	// Expands the 128-bit key to all round keys
	public List<byte[]> expandKey(byte[] key) {
		final int numRounds = 10;
		final int expandedKeySize = 16 * (numRounds + 1);
		byte[] expandedKey = new byte[expandedKeySize];

		// Copy the original key
		System.arraycopy(key, 0, expandedKey, 0, 16);

		byte[] tempBytes = new byte[4];
		for (int i = 16; i < expandedKeySize; i++) {
			tempBytes[0] = expandedKey[i - 4];
			tempBytes[1] = expandedKey[i - 3];
			tempBytes[2] = expandedKey[i - 2];
			tempBytes[3] = expandedKey[i - 1];

			if (i % 16 == 0) {
				// RotWord: cyclic shift
				byte temp = tempBytes[0];
				tempBytes[0] = tempBytes[1];
				tempBytes[1] = tempBytes[2];
				tempBytes[2] = tempBytes[3];
				tempBytes[3] = temp;

				// SubWord: byte substitution using S-box
				tempBytes[0] = AesTables.S_BOX[tempBytes[0] & 0xFF];
				tempBytes[1] = AesTables.S_BOX[tempBytes[1] & 0xFF];
				tempBytes[2] = AesTables.S_BOX[tempBytes[2] & 0xFF];
				tempBytes[3] = AesTables.S_BOX[tempBytes[3] & 0xFF];

				// Rcon XOR
				tempBytes[0] ^= getRcon(i / 16);
			}

			expandedKey[i] = (byte) (expandedKey[i - 16] ^ tempBytes[i % 4]);
		}

		// Divide the expanded key into round keys
		List<byte[]> roundKeys = new ArrayList<>(numRounds + 1);
		for (int round = 0; round <= numRounds; round++) {
			byte[] currentRoundKey = new byte[16];
			System.arraycopy(expandedKey, round * 16, currentRoundKey, 0, 16);
			roundKeys.add(currentRoundKey);
		}
		return roundKeys;
	}

	// Generates a random key of specified length
	public static byte[] generateRandomKey(int length) {
		byte[] key = new byte[length];
		new Random().nextBytes(key);
		return key;
	}

	// Generates a random initialization vector of specified length
	public static byte[] generateRandomIV(int length) {
		return generateRandomKey(length); // IV is generated similar to a key
	}

	// Writes key to a file
	public static void writeKeyToFile(String filename, byte[] key) {
		try (FileOutputStream fos = new FileOutputStream(filename)) {
			fos.write(key);
		} catch (IOException e) {
			System.err.println(
				"Error: Could not open file for writing key: " + e.getMessage()
			);
		}
	}

	// Reads key from a file
	public static byte[] readKeyFromFile(String filename) {
		try {
			return Files.readAllBytes(Paths.get(filename));
		} catch (IOException e) {
			System.err.println(
				"Error: Could not open file for reading key: " + e.getMessage()
			);
			return new byte[0];
		}
	}
}
