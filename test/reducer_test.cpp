#include "gtest/gtest.h"
#include "reducer.hpp"

TEST(ReducerTest, AcceptsSubmissions)
{
    bool success = false;
    mr::ReduceFunction reduceFunction = [&success]( mr::bytelist key
                                                  , std::vector<mr::bytelist> values
                                                  , mr::OutputCollector* collector
                                                  )
    {
        success = true;
    };

    mr::Reducer reducer(reduceFunction, 0);
    mr::bytelist key {'a','b','c','d'};

}

