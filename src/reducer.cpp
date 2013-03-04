#include "reducer.hpp"

namespace mr
{
    Reducer::Reducer( ReduceFunction reduceFunction_
                    , OutputCollector* collector_
                    )
    {
        reduceFunction = reduceFunction_;
        collector = collector_;
    }

    void Reducer::submit(bytelist key, std::vector<bytelist> values)
    {
        reduceFunction(key, values, collector);
    }

    void reducerFunction(Reducer* reducer, ReducerInput* input)
    {
        bytelist key;
        std::vector<bytelist> values;

        while (input->requestNext(key, values))
        {
            reducer->submit(key, values);
        }
    }
}
