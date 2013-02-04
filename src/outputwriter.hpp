#ifndef _OUTPUTWRITER_HPP_
#define _OUTPUTWRITER_HPP_

#include <mutex>

namespace mr
{

    class OutputCollector
    {
      public: 
        virtual void collect(KeyValuePair&) = 0;
    };

    // MapperCollector is not synchronized, since each should only be used by one mapper.
    class MapperCollector : public OutputCollector
    {
      public:
        void collect(KeyValuePair& pair)
        {
            contents.push_back(pair);
        }

        KeyValuePairList getContents();
        // TODO change ^ to receive an iterator, to handle disk caching
      private:
        KeyValuePairList contents;
        // TODO add flush-to-disk capability
    };

}

#endif
