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
        KeyValuePair(bytelist, bytelist);
        bytelist key;
        bytelist value;
    };

    KeyValuePair::KeyValuePair( bytelist key_
                              , bytelist value_) {
        key = key_;
        value = value_;
    }

    typedef std::vector<KeyValuePair> KeyValuePairList;

}

#endif
