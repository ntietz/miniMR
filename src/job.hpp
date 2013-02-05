#ifndef _JOB_HPP_
#define _JOB_HPP_

#include "mapper.hpp"
#include "reducer.hpp"
#include "partition.hpp"
#include "inputreader.hpp"
#include "outputwriter.hpp"
#include "keyvaluepair.hpp"
#include "types.hpp"

namespace mr
{

    class MapReduceJob
    {
      public:
        MapReduceJob(int,MapFunction,int,ReduceFunction,PartitionFunction,MapperInput*,OutputCollector*);
        // TODO destructor

        void run();

        void disableSort();
        void disableReduce();

      private:
        int numMappers;
        int numReducers;

        MapFunction mapFunction;
        ReduceFunction reduceFunction;
        PartitionFunction partitionFunction;

        // TODO delete the pointers in the destructor
        Mapper** mappers;
        Reducer** reducers;
        MapperInput* inputReader;
        OutputCollector** mapperCollectors;
        OutputCollector* reducerCollector;
        Partitioner* partitioner;

        bool sortFlag;
        bool reduceFlag;
    };
}

#endif
