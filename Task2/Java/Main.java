import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class Main {

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        System.out.println("Enter text for encryption:");
        String inputText = scanner.nextLine();

        // Convert the input string to bytes using UTF-8 encoding
        // This ensures that multi-byte characters are correctly handled from the start
        byte[] inputBytesPrimitive = inputText.getBytes(StandardCharsets.UTF_8);
        List<Byte> inputBytes = new ArrayList<>();
        for (byte b : inputBytesPrimitive) {
            inputBytes.add(b);
        }

        // Add padding to input bytes
        List<Byte> paddedInputBytes = Padding.addPkcs7Padding(inputBytes);

        // Generate random key (16 bytes for AES-128)
        byte[] aesKey = KeyExpansion.generateRandomKey(16);
        System.out.print("Generated AES Key: ");
        AesCbcCipher.printBytes(aesKey);

        // Generate random initialization vector (16 bytes for IV)
        byte[] initVec = KeyExpansion.generateRandomIV(16);
        System.out.print("Generated Initialization Vector (IV): ");
        AesCbcCipher.printBytes(initVec);

        System.out.println("\n" + "-".repeat(50));
        System.out.println("Starting AES-128 (CBC) Encryption");
        System.out.println("-".repeat(50));

        // Encryption
        AesCbcCipher cbcCipher = new AesCbcCipher();
        List<Byte> encryptedBytes = cbcCipher.encryptCbc(paddedInputBytes, aesKey, initVec);

        System.out.println("\n" + "-".repeat(50));
        System.out.println("Encryption Completed");
        System.out.println("-".repeat(50));
        System.out.print("Encrypted text: ");
        AesCbcCipher.printBytes(toPrimitiveByteArray(encryptedBytes));

        System.out.println("\n" + "-".repeat(50));
        System.out.println("Starting AES-128 (CBC) Decryption");
        System.out.println("-".repeat(50));

        // Decryption
        List<Byte> decryptedBytes = cbcCipher.decryptCbc(encryptedBytes, aesKey, initVec);

        // Remove padding from decrypted bytes
        List<Byte> finalDecryptedBytes = Padding.removePkcs7Padding(decryptedBytes);

        System.out.println("\n" + "-".repeat(50));
        System.out.println("Decryption Completed");
        System.out.println("-".repeat(50));
        System.out.print("Decrypted text: ");
        // Convert the final decrypted bytes to a String using UTF-8 encoding
        System.out.println(new String(toPrimitiveByteArray(finalDecryptedBytes), StandardCharsets.UTF_8));

        scanner.close();
    }

    // Helper to convert List<Byte> to byte[]
    private static byte[] toPrimitiveByteArray(List<Byte> byteList) {
        byte[] byteArray = new byte[byteList.size()];
        for (int i = 0; i < byteList.size(); i++) {
            byteArray[i] = byteList.get(i);
        }
        return byteArray;
    }
}
