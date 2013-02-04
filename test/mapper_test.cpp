#include "gtest/gtest.h"
#include "mapper.hpp"

TEST(MapperTest, AcceptsSubmissions)
{
    bool success = false;
    mr::MapFunction mapFunction = [&success](mr::KeyValuePair& pair, mr::OutputCollector* collector)
    {
        success = true;
    };

    mr::Mapper mapper(mapFunction, 0);
    mr::KeyValuePair pair;
    mapper.submit(pair);

    ASSERT_TRUE(success);
}

