#ifndef _INPUTREADER_HPP_
#define _INPUTREADER_HPP_

namespace mr {

    class InputReader {
      public:
        virtual KeyValuePair& getNext() = 0;
    };

}

#endif
