#ifndef _KEYVALUEPAIR_HPP_
#define _KEYVALUEPAIR_HPP_

#include <vector>
#include "types.hpp"

namespace mr
{
    /*
     * KeyValuePair is our standard byte-pair.
     */
    struct KeyValuePair
    {
        KeyValuePair();
        KeyValuePair(bytelist, bytelist);
        bytelist key;
        bytelist value;
    };

    typedef std::vector<KeyValuePair> KeyValuePairList;
}

#endif
