#include "gtest/gtest.h"
#include "job.hpp"
#include "mapper.hpp"
#include "reducer.hpp"
#include "inputreader.hpp"
#include "keyvaluepair.hpp"

namespace mr
{
    class MapIntegerReader : public MapperInput
    {
      public:
        MapIntegerReader(int limit_)
        {
            limit = limit_;
            last = 0;
        }

      protected:
        KeyValuePair* getNext()
        {
            if (last < limit)
            {
                KeyValuePair* result = new KeyValuePair();
                bytelist& key = result->key;
                bytelist& value = result->value;

                key.resize(4);
                for (int i = 0; i < 4; ++i) key.push_back(0);
                int& keynum = *((int*)key.data());
                keynum = last;
                key.shrink_to_fit();

                value.resize(4);
                for (int i = 0; i < 4; ++i) value.push_back(0);
                int& valuenum = *((int*)value.data());
                valuenum = last;
                value.shrink_to_fit();

                ++last;

                return result;
            }
            else
            {
                return 0;
            }
        }

        int last;
        int limit;
    };
}

TEST(JobTest, Blank)
{
    int numMappers = 4;
    int numReducers = 4;

    mr::MapFunction mapFunction = []( mr::KeyValuePair& pair
                                    , mr::OutputCollector* collector
                                    )
    {
        int& keynum = *((int*) pair.key.data());
        int& valuenum = *((int*) pair.value.data());
    };

    mr::ReduceFunction reduceFunction = []( mr::bytelist key
                                          , std::vector<mr::bytelist> values
                                          , mr::OutputCollector* collector
                                          )
    {
        
    };

    mr::PartitionFunction partitionFunction = []( int numReducers
                                                , mr::bytelist key
                                                )
    {
        unsigned int sum = 0;
        for (int i = 0; i < key.size(); ++i)
        {
            sum += key[i];
        }
        return sum % numReducers;
    };

    mr::MapperInput* mapperInput = new mr::MapIntegerReader(1000);

    mr::MapReduceJob job(numMappers, mapFunction, numReducers, reduceFunction, partitionFunction, mapperInput, 0);
    job.disableSort();
    job.disableReduce();
    job.run();

}
