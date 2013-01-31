#ifndef _OUTPUTWRITER_HPP_
#define _OUTPUTWRITER_HPP_

namespace mr {

    class OutputWriter {
      public: 
        virtual void collect(KeyValuePair&) = 0;
    }

}

#endif
