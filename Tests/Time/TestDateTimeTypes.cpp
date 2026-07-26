#include <gtest/gtest.h>

#include <Time/DateTimeTypes.hpp>

#define FIFO_SIZE 16

class TestDateTimeTypes : public ::testing::Test
{
public:
    void SetUp() override 
    {
    }

    void TearDown() override 
    {
    }
};

/*
    Проверка начальной инициализации
*/
TEST_F(TestDateTimeTypes, ToUnixTime) 
{
    {
        // 10:23:07 26.07.2026
        DateTimeStruct s
        {
            {       10,     23,     7 },
            { 1,    26-1,   7-1,    26 },
        };
        EXPECT_EQ(s.ToUnixTime(), 1785061387);
    }
    {
        // 06:12:34 08.08.2001
        DateTimeStruct s
        {
            {       6,      12,     34 },
            { 1,    8-1,    8-1,    1 },
        };
        EXPECT_EQ(s.ToUnixTime(), 997251154);
    }
    {
        // 09:59:59 21.04.1989
        DateTimeStruct s
        {
            {       9,      59,     59 },
            { 1,    21-1,    4-1,    89 },
        };
        EXPECT_EQ(s.ToUnixTime(), 609155999);
    }
}
