#include "gtest/gtest.h"
#include "inputreader.hpp"
#include "outputwriter.hpp"
#include "impl/inputreaders.hpp"

TEST(LineInputReader, GetsLines)
{
    mr::LineInputReader reader("test/data/constitution.txt");
    mr::KeyValuePair* pair = reader.requestNext();
    int numRead = 0;

    ASSERT_STREQ("Provided by USConstitution.net", pair->value.data());

    while (pair != NULL)
    {
        int& lineNumber = *((int*)pair->key.data());
        EXPECT_EQ(numRead, lineNumber);

        ++numRead;
        pair = reader.requestNext();
    }
}

TEST(ParagraphInputReader, GetsLines)
{
    mr::ParagraphInputReader reader("test/data/constitution.txt");
    mr::KeyValuePair* pair = reader.requestNext();
    int numRead = 0;

    while (pair != NULL)
    {
        int& paragraphNumber = *((int*) pair->key.data());
        EXPECT_EQ(numRead, paragraphNumber);

        ++numRead;
        pair = reader.requestNext();
    }
}

TEST(CSVInputReader, GetsData)
{
    mr::CSVInputReader reader("test/data/states.csv", ',');
    mr::KeyValuePair* pair = reader.requestNext();
    int numRead = 0;
    
    while (pair != NULL)
    {
        ++numRead;

        if (numRead == 1)
        {
            EXPECT_STREQ("Alabama", pair->key.data());
            EXPECT_STREQ("AL", pair->value.data());
        }
        else if (numRead == 29)
        {
            EXPECT_STREQ("Ohio", pair->key.data());
            EXPECT_STREQ("OH", pair->value.data());
        }

        pair = reader.requestNext();
    }

    ASSERT_EQ(50, numRead);
}

