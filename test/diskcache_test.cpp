#include "gtest/gtest.h"
#include "diskcache.hpp"
#include "keyvaluepair.hpp"
#include <string>

TEST(DiskCacheTest, UnsortedDiskCacheProperties)
{
    mr::UnsortedDiskCache cache(std::string("foobar"),250);

    std::vector<mr::KeyValuePair> submittedPairs;

    for (int i = 0; i < 2048; ++i)
    {
        mr::bytelist keyBytes;
        mr::bytelist valueBytes;
        for (mr::int8 j = 0; j < (i % 20)+5; ++j)
        {
            keyBytes.push_back('a' + j);
            valueBytes.push_back('A' + j);
        }

        submittedPairs.push_back(mr::KeyValuePair(keyBytes, valueBytes));
        cache.submit(keyBytes, valueBytes);
    }

    mr::UnsortedDiskCache::Iterator iterator = cache.getIterator();

    for (int index = 0; index < submittedPairs.size(); ++index)
    {
        ASSERT_TRUE(iterator.hasNext());
        mr::KeyValuePair pair = iterator.getNext();
        EXPECT_EQ(submittedPairs[index], pair);
    }

    ASSERT_FALSE(iterator.hasNext());
}
