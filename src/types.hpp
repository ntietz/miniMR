#ifndef _TYPES_HPP_
#define _TYPES_HPP_

#include <vector>
#include <functional>

namespace mr
{
    class KeyValuePair;

    typedef char int8;
    typedef unsigned char uint8;

    typedef int int32;
    typedef unsigned int uint32;

    typedef long int64;
    typedef unsigned long uint64;

    typedef std::vector<int8> bytelist;

    typedef std::function<bool(const KeyValuePair&, const KeyValuePair&)> Comparator;

    const uint32 KILOBYTE = 1024;
    const uint32 MEGABYTE = 1024 * KILOBYTE;
    const uint32 GIGABYTE = 1024 * MEGABYTE;
}

#endif
