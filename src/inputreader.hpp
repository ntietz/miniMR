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
        // TODO change to the following:
        //  public:
        //    bytelist* requestNextKey();
        //    std::vector<bytelist>* getValues(bytelist* key);
        // 
        // The private virtual handling will be interesting. Probably this:
        //  private:
        //    virtual void getNext(bytelist*&, std::vector<bytelist>*&) = 0;
    };

}

#endif
