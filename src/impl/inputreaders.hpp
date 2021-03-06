#ifndef _INPUTREADERS_HPP_
#define _INPUTREADERS_HPP_

#include "inputreader.hpp"
#include "diskcache.hpp"
#include <fstream>

namespace mr
{
    class LineInputReader : public MapperInput
    {
      public:
        LineInputReader(std::string filename_);
        ~LineInputReader() { }

      protected:
        KeyValuePair* getNext();

        std::string filename;
        uint32 lineNumber;
        std::ifstream in;
    };

    class ParagraphInputReader : public MapperInput
    {
      public:
        ParagraphInputReader(std::string filename_);
        ~ParagraphInputReader() { };

      protected:
        KeyValuePair* getNext();

        std::string filename;
        std::string delimiter;
        uint32 sentenceNumber;
        std::ifstream in;
    };

    class CSVInputReader : public MapperInput
    {
      public:
        CSVInputReader(std::string filename_);
        CSVInputReader(std::string filename_, char delimiter_);
        ~CSVInputReader() { }

      protected:
        KeyValuePair* getNext();

        std::string filename;
        char delimiter;
        std::ifstream in;
    };

    class DiskCacheReader : public MapperInput
    {
      public:
        DiskCacheReader(DiskCacheIterator* iterator_);
        ~DiskCacheReader();

      protected:
        KeyValuePair* getNext();

        DiskCacheIterator* iterator;
    };
}

#endif
