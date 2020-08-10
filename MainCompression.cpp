#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "combiCompressor.h"



int main(int args, char** arg)
{

    char* text = "D8FFPA2E3FFA7D0F1FFC1DEF5FFC4E0F6FFC4E0F6FFC4E0F6FFC4E0F6FFC4E0F6FFC4E0F6FFC4E0F6FFC4E0F6FFC4E0F6FFC4E0F6FFC4E0F6FFC4E0F6FFC4E0F6FFC4E0F6FFC4E0F6FFC4E0F6FFC4E0F6FFC4E0F6FFC4E0F6FFC4E0F6FFC4E0F6FFC4E0F6FFC4E0F6FFC4E0F6FFC4E0F6FFC4E0F6FFC4E0F6";

    //CompressedDataLZW data;

    //data.compress(text, strlen(text));
    printf("Original             : %s\n", text);
    printf("Size: %d bytes.\n", strlen(text));
    printf("\n");
    Huffmann::CodeTable compressed = CompressorLZ77Huffman::compress((unsigned char*)text, strlen(text));
    printf("Compressed Size: %d bytes.\n", compressed.size());
    size_t sizeDecompressed = 0;
    char* decompressed = (char*)CompressorLZ77Huffman::decompress(compressed, &sizeDecompressed);
    printf("Decompressed         : %.*s\n", sizeDecompressed, decompressed);

    printf("\n");
    size_t compressedSize2 = 0;
    size_t symbols = 0;
    unsigned char* compressed2 = CompressorLZ77Huffman::compress2((unsigned char*)text, strlen(text), &symbols, &compressedSize2);
    printf("Compressed Size: %d bytes.\n", compressedSize2);
    size_t sizeDecompressed2 = 0;
    unsigned char* decompressed2 = CompressorLZ77Huffman::decompress2(compressed2, compressedSize2, symbols, &sizeDecompressed2);
    printf("Decompressed         : %.*s\n", sizeDecompressed2, decompressed2);
    int dbg = 1;
    return 0;
}

