#ifndef _MAPPER_HPP_
#define _MAPPER_HPP_

#include <functional>
#include "keyvaluepair.hpp"
#include "inputreader.hpp"
#include "outputwriter.hpp"

namespace mr
{

    typedef std::function<void(KeyValuePair&,OutputCollector*)> MapFunction;

    class Mapper
    {
      public:
        Mapper(MapFunction,OutputCollector*);

        void submit(KeyValuePair&);

      private:
        MapFunction mapFunction;
        OutputCollector* collector;
    };

    Mapper::Mapper( MapFunction mapFunction_
                  , OutputCollector* collector_
                  )
    {
        mapFunction = mapFunction_;
        collector = collector_;
    }

    void Mapper::submit(KeyValuePair& pair)
    {
        mapFunction(pair, collector);
    }

    void mapperFunction(Mapper& mapper, MapperInput& input)
    {
        KeyValuePair* kvpair = input.requestNext();

        while (kvpair != 0)
        {
            mapper.submit(*kvpair);

            delete kvpair;
            kvpair = input.requestNext();
        }

    }

}

#endif
