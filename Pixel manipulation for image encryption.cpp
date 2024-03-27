#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function prototype
void processImage(const char* inputFilePath, const char* outputFilePath, unsigned char key);

int main() {
    char operation[10];
    // Hardcoded file paths and key
    const char* inputFilePath;
    const char* outputFilePathEncrypt = "cybersecurity_encrypted.bmp";
    const char* outputFilePathDecrypt = "cybersecurity_decrypted.bmp";
    unsigned char key = 123; // Example key

    printf("Do you want to encrypt or decrypt the file? (enter 'encrypt' or 'decrypt'): ");
    scanf("%s", operation);

    if(strcmp(operation, "encrypt") == 0) {
        inputFilePath = "cybersecurity.bmp"; // Original file
        processImage(inputFilePath, outputFilePathEncrypt, key);
        printf("Encryption completed. Check the output file: %s\n", outputFilePathEncrypt);
    } else if(strcmp(operation, "decrypt") == 0) {
        inputFilePath = "cybersecurity_encrypted.bmp"; // Encrypted file
        processImage(inputFilePath, outputFilePathDecrypt, key);
        printf("Decryption completed. Check the output file: %s\n", outputFilePathDecrypt);
    } else {
        printf("Invalid operation.\n");
        return 1;
    }

    return 0;
}

void processImage(const char* inputFilePath, const char* outputFilePath, unsigned char key) {
    FILE* inputFile = fopen(inputFilePath, "rb");
    FILE* outputFile = fopen(outputFilePath, "wb");

    if (inputFile == NULL || outputFile == NULL) {
        fprintf(stderr, "Error opening file.\n");
        exit(1);
    }

    unsigned char header[54]; // BMP header is 54 bytes
    fread(header, sizeof(unsigned char), 54, inputFile); // Read the header
    fwrite(header, sizeof(unsigned char), 54, outputFile); // Write the header to output

    int width, height;
    fseek(inputFile, 18, SEEK_SET);
    fread(&width, 4, 1, inputFile);
    fread(&height, 4, 1, inputFile);

    // Calculate padding
    int padding = (4 - (width * 3) % 4) % 4;

    fseek(inputFile, 54, SEEK_SET); // Move the file pointer to the beginning of the bitmap data

    // Process each pixel
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            unsigned char pixel[3];
            fread(pixel, sizeof(unsigned char), 3, inputFile); // Read a pixel (3 bytes)
            
            // XOR each byte of the pixel with the key for encryption/decryption
            for (int k = 0; k < 3; k++) {
                pixel[k] ^= key;
            }

            fwrite(pixel, sizeof(unsigned char), 3, outputFile); // Write the processed pixel
        }
        // Handle padding
        unsigned char paddingBytes[3] = {0};
        fread(paddingBytes, sizeof(unsigned char), padding, inputFile);
        fwrite(paddingBytes, sizeof(unsigned char), padding, outputFile);
    }

    fclose(inputFile);
    fclose(outputFile);
}

