#ifndef _KEYVALUEPAIR_HPP_
#define _KEYVALUEPAIR_HPP_

#include <vector>
#include "types.hpp"

namespace mr {

    /*
     * KeyValuePair is our standard byte-pair.
     */
    class KeyValuePair {
      public:
        KeyValuePair(uint8, bytelist, uint8, bytelist);

        uint8 keyLength;
        bytelist key;

        uint8 valueLength;
        bytelist value;
    };

    KeyValuePair::KeyValuePair( uint8 keyLength_
                              , bytelist key_
                              , uint8 valueLength_
                              , bytelist value_) {
        keyLength = keyLength_;
        key = key_;
        valueLength = valueLength_;
        value = value_;
    }

    typedef std::vector<KeyValuePair> KeyValuePairList;

}

#endif
