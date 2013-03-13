#include "gtest/gtest.h"
#include "diskcache.hpp"
#include "keyvaluepair.hpp"
#include <string>

bool compare(const mr::KeyValuePair& left, const mr::KeyValuePair& right)
{
    return (left.key.size() < right.key.size());
}

bool equal(const mr::KeyValuePair& left, const mr::KeyValuePair& right)
{
    return left.key.size() == right.key.size();
}

TEST(DiskCacheTest, UnsortedDiskCacheProperties)
{
    mr::UnsortedDiskCache cache(std::string("foobar"),1024);

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

    mr::UnsortedDiskCache::Iterator iterator = cache.getIterator(1024);

    for (int index = 0; index < submittedPairs.size(); ++index)
    {
        ASSERT_TRUE(iterator.hasNext());
        mr::KeyValuePair* pair = iterator.getNext();
        EXPECT_EQ(submittedPairs[index], *pair);
        delete pair;
    }

    ASSERT_FALSE(iterator.hasNext());
}

TEST(DiskCacheTest, SortedDiskCacheProperties)
{
    mr::SortedDiskCache cache(std::string("barfoo"), 1024, compare);

    std::vector<mr::KeyValuePair> submittedPairs;
    std::vector<mr::KeyValuePair*> receivedPairs;

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

    mr::SortedDiskCache::Iterator iterator = cache.getIterator(1024);

    while (iterator.hasNext())
    {
        receivedPairs.push_back(iterator.getNext());
    }

    EXPECT_EQ(submittedPairs.size(), receivedPairs.size());

    int unsorted = 0;
    int sorted = 0;
    for (int index = 0; index < receivedPairs.size() - 1; ++index)
    {
        EXPECT_TRUE(compare(*receivedPairs[index], *receivedPairs[index+1]) || equal(*receivedPairs[index], *receivedPairs[index+1]));
    }

    for (auto*& each : receivedPairs)
    {
        delete each;
    }
}
