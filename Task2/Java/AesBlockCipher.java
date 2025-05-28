import java.util.List;

public class AesBlockCipher {

    private final KeyExpansion keyExpansion;

    public AesBlockCipher() {
        this.keyExpansion = new KeyExpansion();
    }

    // Performs byte substitution using the S-box
    private void subBytes(State state) {
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                state.matrix[row][col] = AesTables.S_BOX[state.matrix[row][col] & 0xFF];
            }
        }
    }

    // Performs inverse byte substitution using the inverse S-box
    private void invSubBytes(State state) {
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                state.matrix[row][col] = AesTables.INV_S_BOX[state.matrix[row][col] & 0xFF];
            }
        }
    }

    // Performs cyclic shift of rows
    private void shiftRows(State state) {
        // Row 0 remains unchanged

        // Row 1: cyclic shift left by 1 byte
        byte tempByte = state.matrix[1][0];
        state.matrix[1][0] = state.matrix[1][1];
        state.matrix[1][1] = state.matrix[1][2];
        state.matrix[1][2] = state.matrix[1][3];
        state.matrix[1][3] = tempByte;

        // Row 2: cyclic shift left by 2 bytes
        tempByte = state.matrix[2][0];
        byte tempByte2 = state.matrix[2][1];
        state.matrix[2][0] = state.matrix[2][2];
        state.matrix[2][1] = state.matrix[2][3];
        state.matrix[2][2] = tempByte;
        state.matrix[2][3] = tempByte2;

        // Row 3: cyclic shift left by 3 bytes
        tempByte = state.matrix[3][0];
        tempByte2 = state.matrix[3][1];
        byte tempByte3 = state.matrix[3][2];
        state.matrix[3][0] = state.matrix[3][3];
        state.matrix[3][1] = tempByte;
        state.matrix[3][2] = tempByte2;
        state.matrix[3][3] = tempByte3;
    }

    // Performs inverse cyclic shift of rows
    private void invShiftRows(State state) {
        // Row 0 remains unchanged

        // Row 1: cyclic shift right by 1 byte
        byte tempByte = state.matrix[1][3];
        state.matrix[1][3] = state.matrix[1][2];
        state.matrix[1][2] = state.matrix[1][1];
        state.matrix[1][1] = state.matrix[1][0];
        state.matrix[1][0] = tempByte;

        // Row 2: cyclic shift right by 2 bytes
        tempByte = state.matrix[2][3];
        byte tempByte2 = state.matrix[2][2];
        state.matrix[2][3] = state.matrix[2][1];
        state.matrix[2][2] = state.matrix[2][0];
        state.matrix[2][1] = tempByte;
        state.matrix[2][0] = tempByte2;

        // Row 3: cyclic shift right by 3 bytes
        tempByte = state.matrix[3][3];
        tempByte2 = state.matrix[3][2];
        byte tempByte3 = state.matrix[3][1];
        state.matrix[3][3] = state.matrix[3][0];
        state.matrix[3][2] = tempByte;
        state.matrix[3][1] = tempByte2;
        state.matrix[3][0] = tempByte3;
    }

    // Multiplication in GF(2^8)
    private byte gfMul(byte a, byte b) {
        byte p = 0;
        byte hiBitSet;
        for (int i = 0; i < 8; i++) {
            if ((b & 1) == 1) {
                p ^= a;
            }
            hiBitSet = (byte) (a & 0x80);
            a <<= 1;
            if (hiBitSet == (byte) 0x80) {
                a ^= 0x1b; // XOR with irreducible polynomial x^8 + x^4 + x^3 + x + 1
            }
            b >>= 1;
        }
        return p;
    }

    // Mixes columns
    private void mixColumns(State state) {
        for (int col = 0; col < 4; col++) {
            byte s0 = state.matrix[0][col];
            byte s1 = state.matrix[1][col];
            byte s2 = state.matrix[2][col];
            byte s3 = state.matrix[3][col];

            state.matrix[0][col] = (byte) (gfMul((byte) 0x02, s0) ^ gfMul((byte) 0x03, s1) ^ s2 ^ s3);
            state.matrix[1][col] = (byte) (s0 ^ gfMul((byte) 0x02, s1) ^ gfMul((byte) 0x03, s2) ^ s3);
            state.matrix[2][col] = (byte) (s0 ^ s1 ^ gfMul((byte) 0x02, s2) ^ gfMul((byte) 0x03, s3));
            state.matrix[3][col] = (byte) (gfMul((byte) 0x03, s0) ^ s1 ^ s2 ^ gfMul((byte) 0x02, s3));
        }
    }

    // Performs inverse column mixing
    private void invMixColumns(State state) {
        for (int col = 0; col < 4; col++) {
            byte s0 = state.matrix[0][col];
            byte s1 = state.matrix[1][col];
            byte s2 = state.matrix[2][col];
            byte s3 = state.matrix[3][col];

            state.matrix[0][col] = (byte) (gfMul((byte) 0x0e, s0) ^ gfMul((byte) 0x0b, s1) ^ gfMul((byte) 0x0d, s2) ^ gfMul((byte) 0x09, s3));
            state.matrix[1][col] = (byte) (gfMul((byte) 0x09, s0) ^ gfMul((byte) 0x0e, s1) ^ gfMul((byte) 0x0b, s2) ^ gfMul((byte) 0x0d, s3));
            state.matrix[2][col] = (byte) (gfMul((byte) 0x0d, s0) ^ gfMul((byte) 0x09, s1) ^ gfMul((byte) 0x0e, s2) ^ gfMul((byte) 0x0b, s3));
            state.matrix[3][col] = (byte) (gfMul((byte) 0x0b, s0) ^ gfMul((byte) 0x0d, s1) ^ gfMul((byte) 0x09, s2) ^ gfMul((byte) 0x0e, s3));
        }
    }

    // Adds the round key to the state
    private void addRoundKey(State state, byte[] roundKey) {
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                state.matrix[row][col] ^= roundKey[row + 4 * col];
            }
        }
    }

    // Encrypts a single 128-bit AES block
    public byte[] encryptBlock(byte[] block, byte[] key) {
        State state = new State(block);
        List<byte[]> roundKeys = keyExpansion.expandKey(key);
        final int numRounds = 10;

        System.out.println("Initial encryption block state:");
        state.printState();

        // Initial XOR with round key
        addRoundKey(state, roundKeys.get(0));
        System.out.println("State after AddRoundKey (initial):");
        state.printState();

        // 9 rounds (SubBytes, ShiftRows, MixColumns, AddRoundKey)
        for (int round = 1; round < numRounds; round++) {
            System.out.println("Round " + round + " encryption:");
            subBytes(state);
            System.out.println("After SubBytes:");
            state.printState();

            shiftRows(state);
            System.out.println("After ShiftRows:");
            state.printState();

            mixColumns(state);
            System.out.println("After MixColumns:");
            state.printState();

            addRoundKey(state, roundKeys.get(round));
            System.out.println("After AddRoundKey:");
            state.printState();
        }

        // Final round (SubBytes, ShiftRows, AddRoundKey)
        System.out.println("Final Round " + numRounds + " encryption:");
        subBytes(state);
        System.out.println("After SubBytes (final):");
        state.printState();

        shiftRows(state);
        System.out.println("After ShiftRows (final):");
        state.printState();

        addRoundKey(state, roundKeys.get(numRounds));
        System.out.println("After AddRoundKey (final):");
        state.printState();

        return state.matrixToBytes();
    }

    // Decrypts a single 128-bit AES block
    public byte[] decryptBlock(byte[] block, byte[] key) {
        State state = new State(block);
        List<byte[]> roundKeys = keyExpansion.expandKey(key);
        final int numRounds = 10;

        System.out.println("Initial decryption block state:");
        state.printState();

        // Initial XOR with round key (reverse order)
        addRoundKey(state, roundKeys.get(numRounds));
        System.out.println("State after AddRoundKey (initial reverse):");
        state.printState();

        // 9 rounds (InvShiftRows, InvSubBytes, AddRoundKey, InvMixColumns)
        for (int round = numRounds - 1; round >= 1; round--) {
            System.out.println("Round " + round + " decryption:");
            invShiftRows(state);
            System.out.println("After InvShiftRows:");
            state.printState();

            invSubBytes(state);
            System.out.println("After InvSubBytes:");
            state.printState();

            addRoundKey(state, roundKeys.get(round));
            System.out.println("After AddRoundKey:");
            state.printState();

            invMixColumns(state);
            System.out.println("After InvMixColumns:");
            state.printState();
        }

        // Final round (InvShiftRows, InvSubBytes, AddRoundKey)
        System.out.println("Final Round " + 0 + " decryption:");
        invShiftRows(state);
        System.out.println("After InvShiftRows (final):");
        state.printState();

        invSubBytes(state);
        System.out.println("After InvSubBytes (final):");
        state.printState();

        addRoundKey(state, roundKeys.get(0));
        System.out.println("After AddRoundKey (final):");
        state.printState();

        return state.matrixToBytes();
    }
}
