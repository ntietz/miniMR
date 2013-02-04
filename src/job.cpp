#include "job.hpp"

namespace mr
{
    MapReduceJob::MapReduceJob( int numMappers_
                              , MapFunction mapFunction_
                              , int numReducers_
                              , ReduceFunction reduceFunction_
                              , PartitionFunction partitionFunction_
                              , MapperInput* inputReader_
                              , OutputCollector* reducerCollector_
                              )
    {
        /*
        numMappers = numMappers_;
        numReducers = numReducers_;

        mapFunction = mapFunction_;
        reduceFunction = reduceFunction_;
        partitionFunction = partitionFunction_;
        inputReader = inputReader_;
        reducerCollector = reducerCollector_;

        mapperCollectors = new OutputCollector*[numMappers];
        mappers = new Mapper*[numMappers];
        for (int i = 0; i < numMappers; ++i) {
            mapperCollectors[i] = new MapperCollector();
            mappers[i] = new Mapper(mapFunction, mapperCollectors[i]);
        }

        reducers = new Reducer*[numReducers];
        for (int i = 0; i < numReducers; ++i) {
            reducers[i] = new Reducer(reduceFunction, reducerCollector);
        }
        */
    }

    void MapReduceJob::run()
    {
        /*
            read the input and distribute it to the mappers as we go (in a circular fashion)
            join all the mappers

            partition the output
            sort each partition
            reduce each partition
        */

    }
}
