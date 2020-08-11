#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "combiCompressor.h"



int main(int args, char** arg)
{

    CompressorLZ77Huffman::compressFile("/home/david/Dokumente/StructCompression/test.txt", "/home/david/Dokumente/StructCompression/test.lz77h");

    CompressorLZ77Huffman::decompressFile("/home/david/Dokumente/StructCompression/test.lz77h", "/home/david/Dokumente/StructCompression/decompressed.txt");

return 0;
}

