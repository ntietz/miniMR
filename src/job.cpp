#include "job.hpp"
#include <thread>
#include <iostream>

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
        numMappers = numMappers_;
        numReducers = numReducers_;

        sortFlag = true;
        reduceFlag = true;

        mapFunction = mapFunction_;
        reduceFunction = reduceFunction_;
        partitionFunction = partitionFunction_;
        inputReader = inputReader_;
        reducerCollector = reducerCollector_;

        mappers = new Mapper*[numMappers];
        mapperCollectors = new OutputCollector*[numMappers];
        for (int i = 0; i < numMappers; ++i) {
            mapperCollectors[i] = new MapperCollector();
            mappers[i] = new Mapper(mapFunction, mapperCollectors[i]);
        }

        reducers = new Reducer*[numReducers];
        for (int i = 0; i < numReducers; ++i) {
            reducers[i] = new Reducer(reduceFunction, reducerCollector);
        }
    }

    void MapReduceJob::run()
    {
        std::cout << "Starting MapReduce job...\n";

        std::thread* mapThreads = new std::thread[numMappers];
        for (int i = 0; i < numMappers; ++i)
        {
            std::cout << "> Created thread " << i << std::endl;
            mapThreads[i] = std::thread(mapperFunction, mappers[i], inputReader);
        }

        std::cout << "All mappers running...\n";

        for (int i = 0; i < numMappers; ++i)
        {
            mapThreads[i].join();
            std::cout << "> Thread " << i << " finished" << std::endl;
        }
        /*
            read the input and distribute it to the mappers as we go (in a circular fashion)
            join all the mappers

            partition the output
            sort each partition
            reduce each partition
        */

    }

    MapReduceJob::disableSort()
    {
        sortFlag = false;
    }

    MapReduceJob::disableReduce()
    {
        reduceFlag = false;
    }
}
