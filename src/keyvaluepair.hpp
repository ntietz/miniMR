#ifndef _KEYVALUEPAIR_HPP_
#define _KEYVALUEPAIR_HPP_

#include <vector>
#include "types.hpp"

namespace mr
{
    /*
     * KeyValuePair is our standard byte-pair.
     */
    class KeyValuePair
    {
      public:
        KeyValuePair();
        KeyValuePair(bytelist, bytelist);
        bytelist key;
        bytelist value;

        bool operator==(const KeyValuePair& other) const;
    };

    typedef std::vector<KeyValuePair> KeyValuePairList;
}

#endif
