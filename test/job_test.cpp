#include "gtest/gtest.h"
#include "job.hpp"
#include "mapper.hpp"
#include "reducer.hpp"
#include "inputreader.hpp"
#include "keyvaluepair.hpp"
#include "diskcache.hpp"
#include "impl/inputreaders.hpp"

namespace mr
{
    class MapIntegerReader : public MapperInput
    {
      public:
        MapIntegerReader(unsigned int limit_)
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

TEST(JobTest, IntegrationTest)
{
    const unsigned int LIMIT = 100000;
    mr::uint32 numMappers = 4;
    mr::uint32 numReducers = 4;

    mr::MapFunction mapFunction = []( mr::KeyValuePair& pair
                                    , mr::OutputCollector* collector
                                    )
    {
        int& keynum = *((int*) pair.key.data());
        int& valuenum = *((int*) pair.value.data());
        collector->collect(pair);
    };

    mr::ReduceFunction reduceFunction = []( mr::bytelist key
                                          , std::vector<mr::bytelist> values
                                          , mr::OutputCollector* collector
                                          )
    {
        for (int i = 0; i < values.size(); ++i)
        {
            mr::KeyValuePair pair(key, values[i]);
            collector->collect(pair);
        }
    };

    mr::Comparator comparator = []( const mr::KeyValuePair& left
                                  , const mr::KeyValuePair& right
                                  )
    {
        return left.key.size() < right.key.size();
    };

    mr::MapperInput* mapperInput = new mr::MapIntegerReader(LIMIT);

    mr::MapReduceJob job(numMappers, mapFunction, numReducers, reduceFunction, comparator, mapperInput, 102400000);
    //job.disableSort();
    //job.disableReduce();
    job.run();

    mr::UnsortedDiskCache* resultCache = job.getResults();
    mr::DiskCacheIterator resultIterator = resultCache->getIterator(102400);

    std::vector<mr::KeyValuePair> results;
    while (resultIterator.hasNext())
    {
        results.push_back(resultIterator.getNext());
    }

    ASSERT_EQ(LIMIT, results.size());

    delete mapperInput;
    delete resultCache;
}
