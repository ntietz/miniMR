#include "inputreader.hpp"
#include "keyvaluepair.hpp"

namespace mr
{
        KeyValuePair* MapperInput::requestNext()
        {
            readLock.lock();
            KeyValuePair* result = getNext();
            readLock.unlock();
            return result;
        }
}
