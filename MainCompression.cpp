#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "combiCompressor.h"



int main(int args, char** arg)
{

    char* text = "You have some kind of memory corruption bug, possibly nowhere near the code you showed us. Run your program under valgrind, fix the first invalid memory access it complains about (all subsequent complaints may just be.";

    //CompressedDataLZW data;

    //data.compress(text, strlen(text));
    printf("Original             : %s\n", text);
    printf("Size: %d bytes.\n", strlen(text));
    Huffmann::CodeTable compressed = CompressorLZ77Huffman::compress((unsigned char*)text, strlen(text));
    printf("Compressed Size: %d bytes.\n", compressed.size());
    size_t sizeDecompressed = 0;
    char* decompressed = (char*)CompressorLZ77Huffman::decompress(compressed, &sizeDecompressed);
    printf("Decompressed         : %.*s\n", sizeDecompressed, decompressed);
    int dbg = 1;


    return 0;
}

