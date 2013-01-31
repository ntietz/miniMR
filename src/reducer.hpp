#ifndef _REDUCER_HPP_
#define _REDUCER_HPP_

#include <functional>
#include "keyvaluepair.hpp"
#include "outputwriter.hpp"

namespace mr {

    typedef std::function<void(KeyValuePairList&,OutputWriter&)> ReduceFunction;

    class Reducer {
      public:
       Reducer() { }

      private:
        ReduceFunction reduceFunction;
    };

}

#endif
