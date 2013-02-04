#ifndef _JOB_HPP_
#define _JOB_HPP_

#include "mapper.hpp"
#include "reducer.hpp"
#include "partition.hpp"
#include "inputreader.hpp"
#include "outputwriter.hpp"
#include "keyvaluepair.hpp"
#include "types.hpp"

namespace mr {

    class MapReduceJob {
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

    MapReduceJob::MapReduceJob( int numMappers_
                              , MapFunction mapFunction_
                              , int numReducers_
                              , ReduceFunction reduceFunction_
                              , PartitionFunction partitionFunction_
                              , MapperInput* inputReader_
                              , OutputCollector* reducerCollector_
                              ) {
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

    void MapReduceJob::run() {
        /*
            read the input and distribute it to the mappers as we go (in a circular fashion)
            join all the mappers

            partition the output
            sort each partition
            reduce each partition
        */

    }

}

#endif
