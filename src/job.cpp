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
                              , uint64 memoryLimit_
                              )
    {
        numMappers = numMappers_;
        numReducers = numReducers_;
        memoryLimit = memoryLimit_;

        reduceFlag = true;

        mapFunction = mapFunction_;
        reduceFunction = reduceFunction_;
        comparator = comparator_;
        inputReader = inputReader_;

        mapperDiskCache = new SortedDiskCache("mapper_part", (uint64) (0.9f * memoryLimit), comparator);
        mapperDiskCacheMutex = new std::atomic_flag();

        for (uint32 i = 0; i < numMappers; ++i)
        {
            mapperCollectors.push_back(new MapperCollector(mapperDiskCache, mapperDiskCacheMutex));
            mappers.push_back(new Mapper(mapFunction, mapperCollectors[i]));
        }

        reducerDiskCache = new UnsortedDiskCache("reducer_part", (uint64) (0.9f * memoryLimit));
        reducerDiskCacheMutex = new std::atomic_flag();

        for (uint32 i = 0; i < numReducers; ++i)
        {
            reducerCollectors.push_back(new ReducerCollector(reducerDiskCache, reducerDiskCacheMutex));
            reducers.push_back(new Reducer(reduceFunction, reducerCollectors[i]));
        }
    }

    MapReduceJob::~MapReduceJob()
    {
        for (OutputCollector* each : mapperCollectors)
        {
            delete each;
        }

        for (Mapper* each : mappers)
        {
            delete each;
        }

        for (OutputCollector* each : reducerCollectors)
        {
            delete each;
        }

        for (Reducer* each : reducers)
        {
            delete each;
        }

        delete mapperDiskCache;
        delete mapperDiskCacheMutex;
        delete reducerDiskCacheMutex;
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

        delete [] mapThreads;

        std::cout << std::endl;
        std::cout << "All mappers finished. Sorting reducer input...\n";
        std::cout << std::endl;

        //DiskCacheIterator mapperOutputIterator = mapperDiskCache->getIterator( (uint64) (0.05f * memoryLimit));
        DiskCacheIterator mapperOutputIterator = mapperDiskCache->getIterator(memoryLimit);
        ReducerInput reducerInput(&mapperOutputIterator);

        std::cout << "Input sorted. Starting reducers...\n";
        std::cout << std::endl;

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

        delete [] reduceThreads;

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
