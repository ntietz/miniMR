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
}

