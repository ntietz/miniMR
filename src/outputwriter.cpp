#include "outputwriter.hpp"

namespace mr
{
    void MapperCollector::collect(KeyValuePair& pair)
    {
        contents.push_back(pair);
    }

    KeyValuePairList MapperCollector::getContents()
    {
        return contents;
    }
}
