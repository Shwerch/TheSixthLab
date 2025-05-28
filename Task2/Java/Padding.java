import java.util.ArrayList;
import java.util.List;
import java.util.logging.Logger;

public class Padding {

	private static final Logger LOGGER = Logger.getLogger(
		Padding.class.getName()
	);

	// Adds PKCS#7 padding to the data
	public static List<Byte> addPkcs7Padding(List<Byte> data) {
		int blockSize = 16;
		int paddingBytes = blockSize - (data.size() % blockSize);
		if (paddingBytes == 0) {
			paddingBytes = blockSize; // Add a full block if already a multiple
		}

		List<Byte> paddedData = new ArrayList<>(data);
		for (int i = 0; i < paddingBytes; i++) {
			paddedData.add((byte) paddingBytes);
		}
		return paddedData;
	}

	// Removes PKCS#7 padding from the data
	public static List<Byte> removePkcs7Padding(List<Byte> paddedData) {
		if (paddedData.isEmpty()) {
			return new ArrayList<>();
		}
		byte paddingBytes = paddedData.get(paddedData.size() - 1);

		// Check for valid paddingBytes value (cannot be 0 and not greater than block size)
		if (
			paddingBytes == 0 ||
			paddingBytes > 16 ||
			paddingBytes > paddedData.size()
		) {
			LOGGER.warning(
				"Warning: Invalid padding value (" +
				(paddingBytes & 0xFF) +
				") during decryption."
			);
			return paddedData; // Return as is or handle error
		}
		// Additional check: ensure all padding bytes have the expected value
		for (int i = 1; i <= paddingBytes; i++) {
			if (paddedData.get(paddedData.size() - i) != paddingBytes) {
				LOGGER.warning(
					"Warning: Padding bytes mismatch during decryption. Expected " +
					(paddingBytes & 0xFF) +
					", got " +
					(paddedData.get(paddedData.size() - i) & 0xFF)
				);
				return paddedData; // Return as is or handle error
			}
		}
		return new ArrayList<>(
			paddedData.subList(0, paddedData.size() - paddingBytes)
		);
	}
}
