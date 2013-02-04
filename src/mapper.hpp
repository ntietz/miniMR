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

    void mapperFunction(Mapper& mapper, MapperInput& input);
}

#endif
