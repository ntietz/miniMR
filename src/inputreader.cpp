#include "inputreader.hpp"
#include "keyvaluepair.hpp"

namespace mr
{
        KeyValuePair* MapperInput::requestNext()
        {
            //readLock.lock();
            while (readLock.test_and_set()) { }
            KeyValuePair* result = getNext();
            readLock.clear();
            //readLock.unlock();
            return result;
        }

        MapperInput::MapperInput()
        {
            readLock.clear();
        }

        ReducerInput::ReducerInput(DiskCacheIterator* iterator_)
        {
            iterator = iterator_;
        }

        bool ReducerInput::requestNext(bytelist& key, std::vector<bytelist>& values)
        {
            iteratorLock.lock();

            bool result = false;
            if (iterator->hasNext())
            {
                values.clear();
                key = iterator->peek().key;

                while (iterator->hasNext() && iterator->peek().key == key)
                {
                    values.push_back(iterator->getNext().value);
                }

                result = true;
            }

            iteratorLock.unlock();
            return result;
        }
}
