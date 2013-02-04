#include "mapper.hpp"

namespace mr
{
    Mapper::Mapper( MapFunction mapFunction_
                  , OutputCollector* collector_
                  )
    {
        mapFunction = mapFunction_;
        collector = collector_;
    }

    void Mapper::submit(KeyValuePair& pair)
    {
        mapFunction(pair, collector);
    }

    void mapperFunction(Mapper& mapper, MapperInput& input)
    {
        KeyValuePair* kvpair = input.requestNext();

        while (kvpair != 0)
        {
            mapper.submit(*kvpair);

            delete kvpair;
            kvpair = input.requestNext();
        }

    }

}
