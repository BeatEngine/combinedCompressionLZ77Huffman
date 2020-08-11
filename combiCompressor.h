#include "LempelZiv77.h"
#include "HuffmanCompression.h"



class CompressorLZ77Huffman
{

    static char* generateTmpFile(char* file)
    {
        int l = strlen(file);
        char* tmp = (char*)calloc(l + 10, 1);
        
        memcpy(tmp, file, l);
        int p = l;
        while (!(tmp[p] == '/' || tmp[p] == '\\'))
        {
            p--;
        }
        p++;
        while(p < l-1)
        {
            tmp[p] = 65 + random() % 25;
            p++;
        }
        tmp[l-1] = 0;
        return tmp;
    }

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

    static unsigned char* compress2(unsigned char* data, size_t size, size_t* returnSizeSymbolHead, size_t* returnSize)
    {
        size_t compressedSize;
        Huffmann::CodeTable table = Huffmann::compress((char*)data, size, false);
        char* comp = table.getFullData(returnSizeSymbolHead);
        size_t retSize = table.resultSize + *returnSizeSymbolHead;
        unsigned char* ret = LZWcompression::compress(comp, retSize, returnSize);
        free(comp);
        return ret;
    }

    static unsigned char* decompress2(unsigned char* data, size_t size, size_t symbolHeadSize, size_t* returnSize)
    {
        size_t sizeHuffmanDecompressed = 0;
        char* decomp = LZWcompression::decompress(data, size, &sizeHuffmanDecompressed);
        Huffmann::CodeTable result((char*)decomp, symbolHeadSize);
        result.result = (char*)(decomp+symbolHeadSize);
        result.resultSize = sizeHuffmanDecompressed - symbolHeadSize;
        unsigned char* res = (unsigned char*)result.decompress((int*)returnSize);
        free(decomp);
        return res;
    }


    static void compressFile(char* inputFile, char* outputFile)
    {
        size_t compressedSize = 0;
        char* tmpFile = generateTmpFile(outputFile);
        LZWcompression::compressFile(inputFile, outputFile, &compressedSize);
        //compressedSize = Huffmann::compressFile(inputFile, outputFile);
        remove(tmpFile);
    }

    static void decompressFile(char* inputFile, char* outputFile)
    {
        size_t compressedSize = 0;
        char* tmpFile = generateTmpFile(outputFile);
        //compressedSize = Huffmann::decompressFile(inputFile, outputFile);
        LZWcompression::decompressFile(inputFile, outputFile, &compressedSize);
        remove(tmpFile);
    }

};

