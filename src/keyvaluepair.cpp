#include "keyvaluepair.hpp"

namespace mr
{
    KeyValuePair::KeyValuePair()
    {
        key = bytelist();
        value = bytelist();
    }

    KeyValuePair::KeyValuePair( bytelist key_
                              , bytelist value_
                              )
    {
        key = key_;
        value = value_;
    }
}
