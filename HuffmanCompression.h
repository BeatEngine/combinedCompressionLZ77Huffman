#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <vector>
#include <algorithm>
#include <string>

namespace Huffmann
{

class CompressionTableElement
{
    public:

    CompressionTableElement()
    {

    }
    CompressionTableElement(char value, int count_)
    {
        x = value;
        count = count_;
    }

    int operator < (CompressionTableElement& other)
    {
        return count - other.count;
    }

    int operator > (CompressionTableElement& other)
    {
        return other.count - count;
    }

    static bool isless (CompressionTableElement& i,CompressionTableElement& j) { return (i.count>j.count); }


    char x;
    int count;
};


class CodeTable
{

    char* symbols;
    int length;
    public:
    char* result;
    int resultSize;

    CodeTable(char* sortedSymbols, int size)
    {
        resultSize = 0;
        result = 0;
        length = size;
        symbols = (char*)calloc(size, sizeof(char));
        for(int i = 0; i < size; i++)
        {
            symbols[i] = sortedSymbols[i];
        }
    }

    size_t size()
    {
        return length*sizeof(char) + sizeof(int)*2 + resultSize*sizeof(char);
    }

    ~CodeTable()
    {
        free(symbols);
        if(result != 0)
        {
            //free(result);
        }
    }

    void compress(char* input, int inputSz)
    {
        result = (char*)calloc(inputSz+1, 1);
        resultSize = inputSz+1;
        int bitPointer = 0;
        int biggest = 0;
        for(int i = 0; i < inputSz; i++)
        {
            for(int s = 0; s < length; s++)
            {
                if(symbols[s] == input[i])
                {
                    bitPointer += s;
                    int byte = bitPointer / 8;
                    int bit = bitPointer - 8*byte;
                    if(byte > resultSize-1)
                    {
                        resultSize++;
                        char* tmp;
                        char* that;
                        /*try
                        {
                            char* that = (char*)realloc(result, resultSize);
                        }
                        catch(const std::exception& e)
                        {*/
                            tmp = (char*)calloc(resultSize+50, 1);
                            memcpy(tmp, result, resultSize-1);
                            free(result);
                            result = 0;
                            result = (char*)calloc(resultSize+50, 1);
                            memset(result, 0, resultSize+50);
                            memcpy(result, tmp, resultSize-1);
                            free(tmp);
                            //result = tmp;
                            //result = (char*)realloc(result, resultSize+50);
                            resultSize += 50;
                        //}
                        result[resultSize-1] = 0;
                    }
                    biggest = byte;
                    result[byte] |= 1 << (bit);
                    bitPointer++;
                    break;
                }
            }
        }
        if(biggest < resultSize-1)
        {
            result = (char*)realloc(result, biggest+1);
            resultSize = biggest+1;
        }
    }

    char* decompress(int* returnSize)
    {
        char* res = (char*)calloc(1, resultSize*3);
        int mxsz = resultSize*3;
        int resSz = 0;
        int bitPointer = 0;
        int sigCounter = 0;
        if(resultSize > 0)
        {
            while (bitPointer < resultSize*8)
            {
                int byte = bitPointer / 8;
                int bit = bitPointer - 8*byte;
                if(((result[byte] >> bit) & 1) == 1)
                {
                    resSz++;
                    if(resSz >= mxsz)
                    {
                        res = (char*)realloc(res, resSz*sizeof(char));
                    }

                    res[resSz-1] = symbols[sigCounter];

                    sigCounter = -1;
                }
                bitPointer++;
                sigCounter++;
            }
        }
        if(mxsz > resSz)
        {
            res = (char*)realloc(res, resSz*sizeof(char));
        }
        *returnSize = resSz;
        return res;
    }


};

class CompressionTableElementWord
{
    public:

    CompressionTableElementWord()
    {

    }
    CompressionTableElementWord(std::string& value, int count_)
    {
        x = value;
        count = count_;
    }

    int operator < (CompressionTableElementWord& other)
    {
        return count - other.count;
    }

    int operator > (CompressionTableElementWord& other)
    {
        return other.count - count;
    }

    static bool isless (CompressionTableElementWord& i,CompressionTableElementWord& j) { return (i.count>j.count); }


    std::string x;
    int count;
};

class CodeTableWords
{

    std::vector<std::string> symbols;

    int symbolsSize()
    {
        int bytes = 0;
        for(int i = 0; i < symbols.size(); i++)
        {
            bytes += symbols.at(i).length();
            bytes += sizeof(int);
        }
        return bytes;
    }

    public:
    char* result;
    int resultSize;

    CodeTableWords(std::vector<std::string> sortedSymbols)
    {
        resultSize = 0;
        result = 0;
        for(int i = 0; i < sortedSymbols.size(); i++)
        {
            symbols.push_back(sortedSymbols.at(i));
        }
    }

    size_t size()
    {
        int bytes = 0;
        for(int i = 0; i < symbols.size(); i++)
        {
            bytes += symbols.at(i).length();
            bytes += sizeof(int);
        }
        bytes += sizeof(int);
        return bytes + sizeof(int) + resultSize*sizeof(char);
    }

    ~CodeTableWords()
    {
        if(result != 0)
        {
            //free(result);
        }
    }

    void compress(std::vector<std::string>& input)
    {
        resultSize = symbolsSize()+1;
        result = (char*)calloc(resultSize, 1);
        memset(result, 0, resultSize);
        int bitPointer = 0;
        int biggest = 0;
        bool found;
        for(int i = 0; i < input.size(); i++)
        {
            found = false;
            for(int s = 0; s < symbols.size(); s++)
            {
                if(symbols.at(s) == input.at(i))
                {
                    bitPointer += s;
                    int byte = bitPointer / 8;
                    int bit = bitPointer - 8*byte;
                    if(byte > resultSize-1)
                    {
                        resultSize++;
                        char* tmp;
                        char* that;
                        /*try
                        {
                            char* that = (char*)realloc(result, resultSize);
                        }
                        catch(const std::exception& e)
                        {*/
                            tmp = (char*)calloc(resultSize+50, 1);
                            memcpy(tmp, result, resultSize-1);
                            free(result);
                            result = 0;
                            result = (char*)calloc(resultSize+50, 1);
                            memset(result, 0, resultSize+50);
                            memcpy(result, tmp, resultSize-1);
                            free(tmp);
                            //result = tmp;
                            //result = (char*)realloc(result, resultSize+50);
                            resultSize += 50;
                        //}
                        result[resultSize-1] = 0;
                    }
                    biggest = byte;
                    result[byte] |= 1 << (bit);
                    bitPointer++;
                    found = true;
                    break;
                }
            }
        }
        if(biggest < resultSize-1)
        {
            result = (char*)realloc(result, biggest+1);
            resultSize = biggest+1;
        }
    }

    std::string decompress()
    {
        std::string res;
        int bitPointer = 0;
        int sigCounter = 0;
        if(resultSize > 0)
        {
            while (bitPointer < resultSize*8)
            {
                int byte = bitPointer / 8;
                int bit = bitPointer - 8*byte;
                if(((result[byte] >> bit) & 1) == 1)
                {
                    res.append(symbols.at(sigCounter));
                    sigCounter = -1;
                }
                bitPointer++;
                sigCounter++;
            }
        }
        return res;
    }

};




CodeTable compress(char* data, int size, bool printWordBook)
{

    std::vector<CompressionTableElement> wordBook;

    for(int i = 0; i < size; i++)
    {
        int f = 0;
        for(f = 0; f < wordBook.size(); f++)
        {
            if(wordBook.at(f).x == data[i])
            {
                break;
            }
        }
        if(f == wordBook.size())
        {
            int count = 0;
            for(int c = 0; c < size; c++)
            {
                if(data[i] == data[c])
                {
                    count++;
                }
            }
            wordBook.push_back(CompressionTableElement(data[i], count));
        }
    }

    std::sort(wordBook.begin(), wordBook.end(), CompressionTableElement::isless);

    char* symbols = (char*)calloc(1 + wordBook.size(), sizeof(char));

    for(int i = 0; i < wordBook.size(); i++)
    {
        if(printWordBook)
        {
            printf("%c(%d) ", wordBook.at(i).x, wordBook.at(i).count);
        }
        symbols[i] = wordBook.at(i).x;
    }
    if(printWordBook)
    {
        printf("\n");
    }
    
    CodeTable stuff(symbols, wordBook.size());
    free(symbols);

    stuff.compress(data, size);

    return stuff;
}

std::vector<std::string> textToWordFragments(std::string& text)
{
    std::vector<std::string> results;
    int a = 0;
    for(int i = 0; i < text.length(); i++)
    {
        if(text.at(i) == ' ' || text.at(i) == '.' || text.at(i) == ',' || text.at(i) == '!' || text.at(i) == '?' || text.at(i) == ':' || text.at(i) == ';')
        {
            if(a < i)
            {
                results.push_back(text.substr(a, i - a));
                
                
                if(i > 0)
                {
                    std::string tmp = "";
                    tmp.append(1, text.at(i));
                    results.push_back(tmp);
                }
                a = i + 1;
                if(a < text.length())
                {
                    if(text.at(a) == ' ')
                    {
                        std::string tmp = "";
                        tmp.append(1, text.at(a));
                        results.push_back(tmp);
                        a++;
                    }
                }
            }
            /*else
            {
                std::string tmp = "0";
                tmp.at(0) = text.at(0);
                results.push_back(tmp);
                a++;
            }*/
        }
    }
    if(a < text.length())
    {
        results.push_back(text.substr(a));
    }
    return results;
}

CodeTableWords compressText(std::string& text, bool printWordBook)
{

    std::vector<std::string> data;
    data = textToWordFragments(text);
    std::vector<CompressionTableElementWord> wordBook;
    int size = data.size();
    for(int i = 0; i < size; i++)
    {
        int f = 0;
        for(f = 0; f < wordBook.size(); f++)
        {
            if(wordBook.at(f).x == data.at(i))
            {
                break;
            }
        }
        if(f == wordBook.size())
        {
            int count = 0;
            for(int c = 0; c < size; c++)
            {
                if(data.at(i) == data.at(c))
                {
                    count++;
                }
            }
            wordBook.push_back(CompressionTableElementWord(data.at(i), count));
        }
    }

    std::sort(wordBook.begin(), wordBook.end(), CompressionTableElementWord::isless);

    std::vector<std::string> symbols;

    if(printWordBook)
    {
        for(int i = 0; i < wordBook.size(); i++)
        {
            printf("%s(%d) ", wordBook.at(i).x.c_str(), wordBook.at(i).count);
            symbols.push_back(wordBook.at(i).x);
        }
        printf("\n");
    }
    
    CodeTableWords stuff(symbols);
    stuff.compress(data);
    return stuff;
}

}
