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
        // TODO add options for memory limit
        MapReduceJob(uint32,MapFunction,uint32,ReduceFunction,Comparator,MapperInput*);
        // TODO destructor

        void run();

        void disableSort();
        void disableReduce();

        // TODO add getIterator() to get results
        UnsortedDiskCache* getResults();

      private:
        int numMappers;
        int numReducers;

        MapFunction mapFunction;
        ReduceFunction reduceFunction;
        Comparator comparator;

        // TODO delete the pointers in the destructor
        std::vector<Mapper*> mappers;
        std::vector<Reducer*> reducers;
        MapperInput* inputReader;
        std::vector<OutputCollector*> mapperCollectors;
        std::vector<OutputCollector*> reducerCollectors;

        SortedDiskCache* mapperDiskCache;
        std::mutex* mapperDiskCacheMutex;

        UnsortedDiskCache* reducerDiskCache;
        std::mutex* reducerDiskCacheMutex;

        bool reduceFlag;
    };
}

#endif
