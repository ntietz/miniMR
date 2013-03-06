#ifndef _INPUTREADERS_HPP_
#define _INPUTREADERS_HPP_

#include "inputreader.hpp"
#include <fstream>

namespace mr
{
    class LineInputReader : public MapperInput
    {
      public:
        LineInputReader(std::string filename_);

      protected:
        KeyValuePair* getNext();

        std::string filename;
        uint32 lineNumber;
        std::ifstream in;
    };

    class ParagraphInputReader : public MapperInput
    {
      public:
        ParagraphInputReader(std::string filename_, std::string delimiter_);

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
        CSVInputReader(std::string filename_, std::string delimiter_);

      protected:
        KeyValuePair* getNext();

        std::string filename;
        std::string delimiter;
    };
}

#endif
