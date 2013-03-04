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
        KeyValuePair* requestNext();

      protected:
        virtual KeyValuePair* getNext() = 0;
        std::mutex readLock;
    };

    class ReducerInput
    {
      public:
        ReducerInput(DiskCacheIterator* iterator_);
        bool requestNext(bytelist&, std::vector<bytelist>&); // Note: this must be synchronized

      private:
        DiskCacheIterator* iterator;
        std::mutex iteratorLock;
    };

}

#endif
