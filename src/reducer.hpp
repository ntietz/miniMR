#ifndef _REDUCER_HPP_
#define _REDUCER_HPP_

#include <functional>
#include <vector>
#include "keyvaluepair.hpp"
#include "inputreader.hpp"
#include "outputwriter.hpp"

namespace mr
{

    typedef std::function<void(bytelist,std::vector<bytelist>,OutputCollector*)> ReduceFunction;

    class Reducer
    {
      public:
        Reducer(ReduceFunction,OutputCollector*);

        void submit(bytelist, std::vector<bytelist>);

      private:
        ReduceFunction reduceFunction;
        OutputCollector* collector;
    };

    void reducerFunction(Reducer& reducer, ReducerInput& input);
}

#endif
