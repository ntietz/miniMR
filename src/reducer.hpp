#ifndef _REDUCER_HPP_
#define _REDUCER_HPP_

#include <functional>
#include <vector>
#include "keyvaluepair.hpp"
#include "outputwriter.hpp"

namespace mr {

    typedef std::function<void(bytelist,std::vector<bytelist>,OutputWriter*)> ReduceFunction;

    class Reducer {
      public:
        Reducer(ReduceFunction,OutputWriter*);

        void submit(bytelist, std::vector<bytelist>);

      private:
        ReduceFunction reduceFunction;
        OutputWriter* collector;
    };

    Reducer::Reducer( ReduceFunction reduceFunction_
                    , OutputWriter* collector_
                    ) {
        reduceFunction = reduceFunction_;
        collector = collector_;
    }

    void Reducer::submit(bytelist key, std::vector<bytelist> values) {
        reduceFunction(key, values, collector);
    }

    void reducerFunction(Reducer& reducer, ReducerInput& input) {
        bytelist key;
        std::vector<bytelist> values;

        while (input.requestNext(key, values)) {
            reducer.submit(key, values);
        }
    }

}

#endif
