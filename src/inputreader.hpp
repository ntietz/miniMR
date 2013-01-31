#ifndef _INPUTREADER_HPP_
#define _INPUTREADER_HPP_

namespace mr {

    class InputReader {
      public:
        virtual KeyValuePair& getNext() = 0;
        virtual bool hasNext() = 0;
    };

}

#endif
