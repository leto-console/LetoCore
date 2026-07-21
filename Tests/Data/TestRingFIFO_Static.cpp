#include <gtest/gtest.h>

#include <Data/RingFIFO_Static.hpp>

#define FIFO_SIZE 16

class TestRingFIFO_Static : public ::testing::Test
{
public:
    RingFIFO_Static<int, FIFO_SIZE> fifo;

    int TestValue()
    {
        return 123456789;
    }

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
TEST_F(TestRingFIFO_Static, InitCheck) 
{
    // Act, assert
    EXPECT_EQ(fifo.Count(), 0);
    EXPECT_EQ(fifo.Size(), FIFO_SIZE);
}

/*
    Проверка добавления одного элемента
*/
TEST_F(TestRingFIFO_Static, PushOneCheck) 
{
    // Act
    fifo.Push(TestValue());

    // Assert
    int check = 0;
    EXPECT_EQ(fifo.Count(), 1);
    EXPECT_TRUE(fifo.Front(check));
    EXPECT_EQ(check, TestValue());
}

/*
    Проверка добавления двух элементов друг за другом
*/
TEST_F(TestRingFIFO_Static, PushTwoCheck) 
{
    // Act
    fifo.Push(TestValue());
    fifo.Push(TestValue() + 1);

    // Assert
    int check = 0;
    EXPECT_EQ(fifo.Count(), 2);
    EXPECT_TRUE(fifo.Pop(check));
    EXPECT_EQ(check, TestValue());
    EXPECT_TRUE(fifo.Pop(check));
    EXPECT_EQ(check, TestValue() + 1);
    EXPECT_FALSE(fifo.Pop(check));
}

/*
    Проверка очистки очереди
*/
TEST_F(TestRingFIFO_Static, Clear) 
{
    // Act
    for (int i = 0; i < FIFO_SIZE; ++i)
        fifo.Push(TestValue() + i);
    
    // Assert
    EXPECT_EQ(fifo.Count(), FIFO_SIZE);

    // Act 
    fifo.Clear();

    // Assert
    EXPECT_EQ(fifo.Count(), 0);
}

/*
    Проверка корректной отработки добавления в заполненную очередь
*/
TEST_F(TestRingFIFO_Static, PushOnFull) 
{
    // Act
    for (int i = 0; i < FIFO_SIZE; ++i)
        fifo.Push(TestValue() + i);
    
    fifo.Push(TestValue() - 1);

    // Assert
    EXPECT_EQ(fifo.Count(), FIFO_SIZE);

    int check = 0;
    for (int i = 2; i < FIFO_SIZE; ++i)
    {
        EXPECT_TRUE(fifo.Pop(check));
        EXPECT_EQ(check, TestValue() + i);
    }

    EXPECT_TRUE(fifo.Pop(check));
    EXPECT_EQ(check, TestValue() - 1);

    EXPECT_FALSE(fifo.Pop(check));
}

/*
    Проверка отработки for-each
*/
TEST_F(TestRingFIFO_Static, ForEachTest) 
{
    // Act
    size_t i = 0;
    for (; i < fifo.Size(); ++i)
        fifo.Push(TestValue() + i);
    
    // Assert
    i = 0;
    for (int& elem : fifo)
    {
        EXPECT_EQ(elem, TestValue() + i);
        i++;
    }
    i = 0;
    for (const int& elem : fifo)
    {
        EXPECT_EQ(elem, TestValue() + i);
        i++;
    }
}
