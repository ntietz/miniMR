#ifndef _JOB_HPP_
#define _JOB_HPP_

#include "mapper.hpp"
#include "reducer.hpp"
#include "inputreader.hpp"
#include "outputwriter.hpp"
#include "keyvaluepair.hpp"
#include "types.hpp"

namespace mr
{

    class MapReduceJob
    {
      public:
        MapReduceJob(uint32,MapFunction,uint32,ReduceFunction,Comparator,MapperInput*,uint64);
        ~MapReduceJob();

        void run();

        void disableReduce();

        UnsortedDiskCache* getResults();

      private:
        uint32 numMappers;
        uint32 numReducers;
        uint64 memoryLimit;

        MapFunction mapFunction;
        ReduceFunction reduceFunction;
        Comparator comparator;

        std::vector<Mapper*> mappers;
        std::vector<Reducer*> reducers;
        MapperInput* inputReader;
        std::vector<OutputCollector*> mapperCollectors;
        std::vector<OutputCollector*> reducerCollectors;

        SortedDiskCache* mapperDiskCache;
        std::atomic_flag* mapperDiskCacheMutex;

        UnsortedDiskCache* reducerDiskCache;
        std::atomic_flag* reducerDiskCacheMutex;

        bool reduceFlag;
    };
}

#endif
