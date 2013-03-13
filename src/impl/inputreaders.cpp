#include "impl/inputreaders.hpp"
#include <fstream>
#include <string>

namespace mr
{
    LineInputReader::LineInputReader(std::string filename_)
    {
        lineNumber = 0;
        filename = filename_;
        in.open(filename);
    }

    KeyValuePair* LineInputReader::getNext()
    {
        std::string line;

        if (std::getline(in, line))
        {
            KeyValuePair* result = new KeyValuePair();
            bytelist& key = result->key;
            bytelist& value = result->value;

            key.resize(sizeof(uint32));
            int& keyValue = *((int*) key.data());
            keyValue = lineNumber;
            ++lineNumber;

            uint32 valueSize = line.size() + 1;
            value.resize(valueSize);
            for (uint32 i = 0; i < valueSize - 1; ++i)
            {
                value[i] = line[i];
            }
            value[valueSize - 1] = '\0';

            return result;
        }
        else
        {
            return NULL;
        }
    }

    ParagraphInputReader::ParagraphInputReader(std::string filename_)
    {
        filename = filename_;
        sentenceNumber = 0;
        in.open(filename);
    }

    KeyValuePair* ParagraphInputReader::getNext()
    {
        std::string line;
        std::string buffer;

        while (std::getline(in, line) && !line.empty())
        {
            buffer += line + '\n';
        }

        if (buffer.empty())
        {
            return NULL;
        }
        else
        {
            KeyValuePair* result = new KeyValuePair();
            bytelist& key = result->key;
            bytelist& value = result->value;

            key.resize(sizeof(uint32));
            int& keyValue = *((int*) key.data());
            keyValue = sentenceNumber;
            ++sentenceNumber;

            uint32 valueSize = buffer.size() + 1;
            value.resize(valueSize);
            for (uint32 i = 0; i < valueSize - 1; ++i)
            {
                value[i] = buffer[i];
            }
            value[valueSize - 1] = '\0';

            return result;
        }
    }

    CSVInputReader::CSVInputReader(std::string filename_)
    {
        filename = filename_;
        delimiter = ',';
        in.open(filename);
    }

    CSVInputReader::CSVInputReader(std::string filename_, char delimiter_)
    {
        filename = filename_;
        delimiter = delimiter_;
        in.open(filename);
    }

    KeyValuePair* CSVInputReader::getNext()
    {
        std::string line;

        if (std::getline(in, line))
        {
            std::string::size_type delimiterIndex = line.find(delimiter);
            if (delimiterIndex == std::string::npos)
            {
                return NULL;
            }
            
            std::string left = line.substr(0, delimiterIndex);
            std::string right = line.substr(delimiterIndex+1);

            KeyValuePair* result = new KeyValuePair();
            bytelist& key = result->key;
            bytelist& value = result->value;

            uint32 keySize = left.size() + 1;
            key.resize(keySize);
            for (uint32 i = 0; i < keySize - 1; ++i)
            {
                key[i] = left[i];
            }
            key[keySize - 1] = '\0';

            uint32 valueSize = right.size() + 1;
            value.resize(valueSize);
            for (uint32 i = 0; i < valueSize - 1; ++i)
            {
                value[i] = right[i];
            }
            value[valueSize - 1] = '\0';

            return result;
        }
        else
        {
            return NULL;
        }
    }

    DiskCacheReader::DiskCacheReader(DiskCacheIterator* iterator_)
    {
        iterator = iterator_;
    }

    DiskCacheReader::~DiskCacheReader()
    {
        // ...
    }

    KeyValuePair* DiskCacheReader::getNext()
    {
        if (!iterator->hasNext())
        {
            return NULL;
        }
        else
        {
            return iterator->getNext();
        }
    }
}

