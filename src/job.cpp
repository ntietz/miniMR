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
                              , OutputCollector* outputWriter_
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
        //reducerCollector = reducerCollector_;
        outputWriter = outputWriter_;

        for (int i = 0; i < numMappers; ++i) {
            mapperCollectors.push_back(new MapperCollector());
            mappers.push_back(new Mapper(mapFunction, mapperCollectors[i]));
        }

        for (int i = 0; i < numReducers; ++i) {
            reducerCollectors.push_back(new MapperCollector()); // TODO change the name of MapperCollector to be generic
            reducers.push_back(new Reducer(reduceFunction, reducerCollectors[i]));
        }
    }

    void MapReduceJob::run()
    {
        std::cout << "Starting MapReduce job...\n";

        std::thread* mapThreads = new std::thread[numMappers];
        for (int i = 0; i < numMappers; ++i)
        {
            std::cout << "> Created mapper " << i << std::endl;
            mapThreads[i] = std::thread(mapperFunction, mappers[i], inputReader);
        }

        std::cout << "All mappers running...\n";

        for (int i = 0; i < numMappers; ++i)
        {
            mapThreads[i].join();
            std::cout << "> Mapper " << i << " finished" << std::endl;
        }
        /*
            partition the output
            sort each partition
            reduce each partition
        */

    }

    void MapReduceJob::disableSort()
    {
        sortFlag = false;
    }

    void MapReduceJob::disableReduce()
    {
        reduceFlag = false;
    }
}
