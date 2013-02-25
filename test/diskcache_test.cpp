#include "gtest/gtest.h"
#include "diskcache.hpp"
#include "keyvaluepair.hpp"
#include <string>

TEST(DiskCacheTest, Blank)
{
    mr::UnsortedDiskCache cache(std::string("foobar"),100);

    for (int i = 0; i < 100; ++i)
    {
        mr::bytelist key_bytes;
        mr::bytelist value_bytes;
        for (mr::int8 j = 0; j < (i % 20)+5; ++j)
        {
            key_bytes.push_back('a' + j);
            value_bytes.push_back('A' + j);
        }

        cache.submit(key_bytes, value_bytes);
    }

    mr::UnsortedDiskCache::Iterator iterator = cache.getIterator();
    int numRead = 0;
    while (iterator.hasNext())
    {
        mr::KeyValuePair next = iterator.getNext();

        std::cout << numRead << "\tkey:";
        for (int i = 0; i < next.key.size(); ++i) std::cout << next.key[i];
        std::cout << std::endl;

        std::cout << numRead << "\tvalue:";
        for (int i = 0; i < next.value.size(); ++i) std::cout << next.value[i];
        std::cout << std::endl;

        ++numRead;
    }
}
