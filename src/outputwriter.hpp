#ifndef _OUTPUTWRITER_HPP_
#define _OUTPUTWRITER_HPP_

namespace mr {

    class OutputWriter {
      public: 
        virtual void collect(KeyValuePair&) = 0;
    };

    class MapperCollector : public OutputWriter {
      public:
        void collect(KeyValuePair& pair) {
            contents.push_back(pair);
        }

        KeyValuePairList getContents();
      private:
        KeyValuePairList contents;
    };

}

#endif
