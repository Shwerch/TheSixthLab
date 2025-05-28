public class State {

	public byte[][] matrix;

	public State() {
		matrix = new byte[4][4];
	}

	// Constructor to create a State from a byte array
	public State(byte[] bytes) {
		matrix = new byte[4][4];
		bytesToMatrix(bytes);
	}

	// Converts a byte array to the state matrix
	public void bytesToMatrix(byte[] bytes) {
		for (int i = 0; i < 16; i++) {
			matrix[i % 4][i / 4] = bytes[i];
		}
	}

	// Converts the state matrix to a byte array
	public byte[] matrixToBytes() {
		byte[] bytes = new byte[16];
		for (int i = 0; i < 16; i++) {
			bytes[i] = matrix[i % 4][i / 4];
		}
		return bytes;
	}

	// Prints the state matrix in hexadecimal format
	public void printState() {
		for (int row = 0; row < 4; row++) {
			for (int col = 0; col < 4; col++) {
				System.out.printf("%02x ", matrix[row][col]);
			}
			System.out.println();
		}
		System.out.println();
	}
}
