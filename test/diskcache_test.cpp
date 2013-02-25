#include "gtest/gtest.h"
#include "diskcache.hpp"
#include <string>

TEST(DiskCacheTest, Blank)
{
    mr::UnsortedDiskCache cache(std::string("foobar"),10);

    for (int i = 0; i < 100; ++i)
    {
        mr::bytelist key_bytes;
        mr::bytelist value_bytes;
        for (mr::int8 j = 0; j < 13; ++j)
        {
            key_bytes.push_back(j);
            value_bytes.push_back(j);
        }

        cache.submit(key_bytes, value_bytes);
    }
}
