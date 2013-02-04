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

        void run();

      private:
        int numMappers;
        int numReducers;

        /*
        MapFunction mapFunction;
        ReduceFunction reduceFunction;
        PartitionFunction partitionFunction;

        Mapper** mappers;
        Reducer** reducers;
        MapperInput* inputReader;
        OutputCollector** mapperCollectors;
        OutputCollector* reducerCollector;
        Partitioner* partitioner;
        */
    };
}

#endif
