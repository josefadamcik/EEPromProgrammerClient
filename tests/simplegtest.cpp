#include "gtest/gtest.h"
#include "boost/format.hpp"

TEST(simple_g_test, just_trying)
{
    int i = 1;
    EXPECT_EQ(1, i);
}

TEST(boost_format_hex, learning_test) 
{
    std::string res = str( boost::format("%1$04x") % 15 );
    EXPECT_EQ("000f", res);
}