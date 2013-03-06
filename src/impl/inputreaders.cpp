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
            for (int i = 0; i < valueSize - 1; ++i)
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

    ParagraphInputReader::ParagraphInputReader(std::string filename_, std::string delimiter_)
    {
        filename = filename_;
        delimiter = delimiter_;
        sentenceNumber = 0;
        in.open(filename);
    }

    KeyValuePair* ParagraphInputReader::getNext()
    {
        std::string line;
        std::string buffer;

        while (std::getline(in, line) && !line.empty())
        {
            buffer += line;
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
            for (int i = 0; i < valueSize - 1; ++i)
            {
                value[i] = buffer[i];
            }
            value[valueSize - 1] = '\0';

            return result;
        }
    }
}

