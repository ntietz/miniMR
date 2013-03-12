#ifndef _INPUTREADER_HPP_
#define _INPUTREADER_HPP_

#include <mutex>
#include "keyvaluepair.hpp"
#include "diskcache.hpp"

namespace mr
{

    class MapperInput
    {
      public:
        virtual ~MapperInput() { }
        KeyValuePair* requestNext();

      protected:
        virtual KeyValuePair* getNext() = 0;
        std::mutex readLock;
    };

    class ReducerInput
    {
      public:
        virtual ~ReducerInput() { }
        ReducerInput(DiskCacheIterator* iterator_);
        bool requestNext(bytelist&, std::vector<bytelist>&);

      private:
        DiskCacheIterator* iterator;
        std::mutex iteratorLock;
    };

}

#endif
