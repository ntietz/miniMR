#ifndef _INPUTREADER_HPP_
#define _INPUTREADER_HPP_

#include <mutex>
#include "keyvaluepair.hpp"

namespace mr {

    class MapperInput {
      public:
        KeyValuePair* requestNext() {
            readLock.lock();
            KeyValuePair* result = getNext();
            readLock.unlock();
            return result;
        }

      private:
        virtual KeyValuePair* getNext() = 0;
        std::mutex readLock;
    };

    class ReducerInput {
      public:
        virtual bool requestNext(bytelist&, std::vector<bytelist>&) = 0; // Note: this must be synchronized
    };

}

#endif
