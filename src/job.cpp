#include "job.hpp"
#include <thread>
#include <iostream>

namespace mr
{
    MapReduceJob::MapReduceJob( uint32 numMappers_
                              , MapFunction mapFunction_
                              , uint32 numReducers_
                              , ReduceFunction reduceFunction_
                              , Comparator comparator_
                              , MapperInput* inputReader_
                              , OutputCollector* outputWriter_
                              )
    {
        numMappers = numMappers_;
        numReducers = numReducers_;

        reduceFlag = true;

        mapFunction = mapFunction_;
        reduceFunction = reduceFunction_;
        comparator = comparator_;
        inputReader = inputReader_;
        //reducerCollector = reducerCollector_;
        outputWriter = outputWriter_;

        mapperDiskCache = new SortedDiskCache("foobar", 10240, comparator);
        mapperDiskCacheMutex = new std::mutex();

        for (uint32 i = 0; i < numMappers; ++i) {
            //mapperCollectors.push_back(new MapperCollector("foobar", 10240, comparator));
            mapperCollectors.push_back(new MapperCollector(mapperDiskCache, mapperDiskCacheMutex));
            mappers.push_back(new Mapper(mapFunction, mapperCollectors[i]));
        }

        for (uint32 i = 0; i < numReducers; ++i) {
            //reducerCollectors.push_back(new MapperCollector()); // TODO change the name of MapperCollector to be generic
            //reducers.push_back(new Reducer(reduceFunction, reducerCollectors[i]));
        }
    }

    void MapReduceJob::run()
    {
        std::cout << "Starting MapReduce job...\n";

        std::thread* mapThreads = new std::thread[numMappers];
        for (uint32 i = 0; i < numMappers; ++i)
        {
            std::cout << "> Created mapper " << i << std::endl;
            mapThreads[i] = std::thread(mapperFunction, mappers[i], inputReader);
        }

        std::cout << "All mappers running...\n";

        for (uint32 i = 0; i < numMappers; ++i)
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

    void MapReduceJob::disableReduce()
    {
        reduceFlag = false;
    }
}
