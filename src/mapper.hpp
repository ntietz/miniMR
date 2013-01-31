#ifndef _MAPPER_HPP_
#define _MAPPER_HPP_

#include <functional>
#include "keyvaluepair.hpp"
#include "outputwriter.hpp"

namespace mr {

    typedef std::function<void(KeyValuePair&,OutputWriter&)> MapFunction;

    class Mapper {
      public:
        Mapper(OutputWriter*);

      private:
        MapFunction mapFunction;
        OutputWriter* collector;
    };

    Mapper::Mapper(OutputWriter* collector_) {
        collector = collector_;
    }

}

#endif
