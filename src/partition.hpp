#ifndef _PARTITION_HPP_
#define _PARTITION_HPP_

#include <functional>

namespace mr {

    typedef std::function<int(int,bytelist)> PartitionFunction;

    class Partitioner {
      public:
        Partitioner() { }

      private:
        // TODO
    };

}

#endif
