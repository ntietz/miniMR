#ifndef _MAPPER_HPP_
#define _MAPPER_HPP_

#include <functional>
#include "keyvaluepair.hpp"
#include "outputwriter.hpp"

namespace mr {

    typedef std::function<void(KeyValuePair&,OutputWriter*)> MapFunction;

    class Mapper {
      public:
        Mapper(MapFunction,OutputWriter*);

        void submitValue(KeyValuePair&);
        void submitValues(KeyValuePairList&);

      private:
        MapFunction mapFunction;
        OutputWriter* collector;
    };

    Mapper::Mapper( MapFunction mapFunction_
                  , OutputWriter* collector_
                  ) {
        mapFunction = mapFunction_;
        collector = collector_;
    }

    void Mapper::submitValue(KeyValuePair& pair) {
        mapFunction(pair, collector);
    }

    void Mapper::submitValues(KeyValuePairList& pairs) {
        for (KeyValuePair pair : pairs) {
            mapFunction(pair, collector);
        }
    }

}

#endif
