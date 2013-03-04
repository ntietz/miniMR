#include "job.hpp"
#include <thread>
#include <iostream>
#include "diskcache.hpp"

namespace mr
{
    MapReduceJob::MapReduceJob( uint32 numMappers_
                              , MapFunction mapFunction_
                              , uint32 numReducers_
                              , ReduceFunction reduceFunction_
                              , Comparator comparator_
                              , MapperInput* inputReader_
                              )
    {
        numMappers = numMappers_;
        numReducers = numReducers_;

        reduceFlag = true;

        mapFunction = mapFunction_;
        reduceFunction = reduceFunction_;
        comparator = comparator_;
        inputReader = inputReader_;

        mapperDiskCache = new SortedDiskCache("foobar", 10240, comparator);
        mapperDiskCacheMutex = new std::mutex();

        for (uint32 i = 0; i < numMappers; ++i)
        {
            mapperCollectors.push_back(new MapperCollector(mapperDiskCache, mapperDiskCacheMutex));
            mappers.push_back(new Mapper(mapFunction, mapperCollectors[i]));
        }

        reducerDiskCache = new UnsortedDiskCache("barfoo", 10240);
        reducerDiskCacheMutex = new std::mutex();

        for (uint32 i = 0; i < numReducers; ++i)
        {
            reducerCollectors.push_back(new ReducerCollector(reducerDiskCache, reducerDiskCacheMutex));
            reducers.push_back(new Reducer(reduceFunction, reducerCollectors[i]));
        }
    }

    void MapReduceJob::run()
    {
        std::cout << "Starting MapReduce job...\n";
        std::cout << std::endl;

        std::thread* mapThreads = new std::thread[numMappers];
        for (uint32 i = 0; i < numMappers; ++i)
        {
            std::cout << "> Created mapper " << i << std::endl;
            mapThreads[i] = std::thread(mapperFunction, mappers[i], inputReader);
        }

        std::cout << std::endl;
        std::cout << "All mappers running...\n";
        std::cout << std::endl;

        for (uint32 i = 0; i < numMappers; ++i)
        {
            mapThreads[i].join();
            std::cout << "> Mapper " << i << " finished" << std::endl;
        }

        std::cout << std::endl;
        std::cout << "All mappers finished. Starting reducers...\n";
        std::cout << std::endl;

        DiskCacheIterator mapperOutputIterator = mapperDiskCache->getIterator();
        ReducerInput reducerInput(&mapperOutputIterator);

        std::thread* reduceThreads = new std::thread[numReducers];
        for (uint32 i = 0; i < numReducers; ++i)
        {
            std::cout << "> Created reducer " << i << std::endl;
            reduceThreads[i] = std::thread(reducerFunction, reducers[i], &reducerInput);
        }

        std::cout << std::endl;
        std::cout << "All reducers running...\n";
        std::cout << std::endl;

        for (uint32 i = 0; i < numReducers; ++i)
        {
            reduceThreads[i].join();
            std::cout << "> Reducer " << i << " finished" << std::endl;
        }

        std::cout << std::endl;
        std::cout << "All reducers finished.\n";
        std::cout << std::endl;
    }

    void MapReduceJob::disableReduce()
    {
        reduceFlag = false;
    }

    UnsortedDiskCache* MapReduceJob::getResults()
    {
        return reducerDiskCache;
    }
}
