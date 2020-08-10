#include "LempelZiv77.h"
#include "HuffmanCompression.h"



class CompressorLZ77Huffman
{
    public:

    static Huffmann::CodeTable compress(unsigned char* data, size_t size)
    {
        size_t compressedSize;
        unsigned char* compressed = LZWcompression::compress((char*)data, size, &compressedSize);
        return Huffmann::compress((char*)compressed, compressedSize, false);
    }

    static unsigned char* decompress(Huffmann::CodeTable& data, size_t* returnSize)
    {
        int sizeHuffmanDecompressed = 0;
        char* decomp = data.decompress(&sizeHuffmanDecompressed);
        size_t sizeDecompressed = 0;
        unsigned char* result = (unsigned char*)LZWcompression::decompress((unsigned char*)decomp, sizeHuffmanDecompressed, returnSize);
        free(decomp);
        return result;
    }

};

