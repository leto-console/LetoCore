
#include <gtest/gtest.h>

#include <Data/StaticList.hpp>

static const size_t LIST_SIZE = 32;

template <typename T>
class StaticListTest : public ::testing::Test {
protected:
    using Type = T;

    // Защищённые члены доступны в тестах
    StaticList<T, LIST_SIZE> list;

    inline constexpr T TestValue() 
    { 
        if      constexpr (std::is_same_v<T, float>)      return 3.1415f; 
        else if constexpr (std::is_same_v<T, uint8_t>)    return 0xBA;
        else if constexpr (std::is_same_v<T, int8_t>)     return 0x4A;
        else if constexpr (std::is_same_v<T, uint16_t>)   return 0xBABE;
        else if constexpr (std::is_same_v<T, int16_t>)    return 0x4ABE;
        else if constexpr (std::is_same_v<T, uint32_t>)   return 0xC1A0BABE;
        else if constexpr (std::is_same_v<T, int32_t>)    return 0x41A0BABE;

        return T{};
    }

    void SetUp() override { }
    void TearDown() override { }
};

using StaticListTypes = ::testing::Types<
    int8_t,
    uint8_t,
    // int16_t,
    // uint16_t,
    int32_t,
    uint32_t,
    float
>;

TYPED_TEST_SUITE(StaticListTest, StaticListTypes);

/*
    Проверка начальной инициализации
*/
TYPED_TEST(StaticListTest, InitCheck) 
{
    // Act, assert
    EXPECT_EQ(this->list.Count(), 0);
    EXPECT_EQ(this->list.Size(), LIST_SIZE);
    EXPECT_TRUE(this->list.Empty());
    EXPECT_FALSE(this->list.Full());
}

/*
    Проверка добавления одного элемента
*/
TYPED_TEST(StaticListTest, PushOneCheck) 
{
    // Act
    this->list.Push(this->TestValue());

    // Assert
    EXPECT_EQ(this->list.Count(), 1);
    EXPECT_FALSE(this->list.Empty());
    EXPECT_FALSE(this->list.Full());
    EXPECT_EQ(this->list[0], this->TestValue());
}

/*
    Проверка добавления двух элементов друг за другом
*/
TYPED_TEST(StaticListTest, PushTwoCheck) 
{
    // Act
    this->list.Push(this->TestValue());
    this->list.Push(this->TestValue() + 1);

    // Assert
    EXPECT_EQ(this->list.Count(), 2);
    EXPECT_FALSE(this->list.Empty());
    EXPECT_FALSE(this->list.Full());
    EXPECT_EQ(this->list[0], this->TestValue());
    EXPECT_EQ(this->list[1], this->TestValue() + 1);
}

/*
    Проверка флага заполненности списка
*/
TYPED_TEST(StaticListTest, Full) 
{
    // Act
    for (int i = 0; i < this->list.Size(); ++i)
        this->list.Push(this->TestValue());

    // Assert
    EXPECT_EQ(this->list.Count(), this->list.Size());
    EXPECT_FALSE(this->list.Empty());
    EXPECT_TRUE(this->list.Full());
}

/*
    Проверка очистки списка
*/
TYPED_TEST(StaticListTest, Clear) 
{
    // Act
    for (int i = 0; i < this->list.Size(); ++i)
        this->list.Push(this->TestValue());
    
    // Assert
    EXPECT_EQ(this->list.Count(), this->list.Size());
    EXPECT_FALSE(this->list.Empty());
    EXPECT_TRUE(this->list.Full());

    // Act 
    this->list.Clear();

    // Assert
    EXPECT_EQ(this->list.Count(), 0);
    EXPECT_TRUE(this->list.Empty());
    EXPECT_FALSE(this->list.Full());
}

/*
    Проверка корректной отработки добавления в заполненный список
*/
TYPED_TEST(StaticListTest, PushOnFull) 
{
    // Act
    for (int i = 0; i < this->list.Size(); ++i)
        this->list.Push(this->TestValue());
    
    this->list.Push(this->TestValue() + 1);

    // Assert
    EXPECT_EQ(this->list.Count(), this->list.Size());
    EXPECT_FALSE(this->list.Empty());
    EXPECT_TRUE(this->list.Full());
    for (const TypeParam& val : this->list)
        EXPECT_EQ(val, this->TestValue());
}

/*
    Проверка Front и Back
*/
TYPED_TEST(StaticListTest, FrontAndBack) 
{
    // Act
    this->list.Push(this->TestValue() + 0);
    this->list.Push(this->TestValue() + 1);
    this->list.Push(this->TestValue() + 2);
    
    // Assert
    EXPECT_EQ(this->list.Count(), 3);
    EXPECT_EQ(this->list.Front(), this->TestValue());
    EXPECT_EQ(this->list.Back(), this->TestValue() + 2);
}

/*
    Проверка вставки элемента между двумя другими
*/
TYPED_TEST(StaticListTest, InsertBetween) 
{
    // Act
    this->list.Push(this->TestValue() + 0);
    this->list.Push(this->TestValue() + 1);

    this->list.Insert(&this->list[1], this->TestValue() + 2);
    
    // Assert
    EXPECT_EQ(this->list.Count(), 3);
    EXPECT_EQ(this->list[0], this->TestValue());
    EXPECT_EQ(this->list[1], this->TestValue() + 2);
    EXPECT_EQ(this->list[2], this->TestValue() + 1);
}

/*
    Проверка вставки элемента перед первым
*/
TYPED_TEST(StaticListTest, InsertBeforeFirst) 
{
    // Act
    this->list.Push(this->TestValue() + 0);
    this->list.Push(this->TestValue() + 1);

    this->list.Insert(&this->list[0], this->TestValue() + 2);
    
    // Assert
    EXPECT_EQ(this->list.Count(), 3);
    EXPECT_EQ(this->list[0], this->TestValue() + 2);
    EXPECT_EQ(this->list[1], this->TestValue());
    EXPECT_EQ(this->list[2], this->TestValue() + 1);
}

/*
    Проверка вставки элемента после последнего
*/
TYPED_TEST(StaticListTest, InsertAfterLast) 
{
    // Act
    this->list.Push(this->TestValue() + 0);
    this->list.Push(this->TestValue() + 1);

    this->list.Insert(this->list.end(), this->TestValue() + 2);
    
    // Assert
    EXPECT_EQ(this->list.Count(), 3);
    EXPECT_EQ(this->list[0], this->TestValue());
    EXPECT_EQ(this->list[1], this->TestValue() + 1);
    EXPECT_EQ(this->list[2], this->TestValue() + 2);
}

/*
    Проверка удаления элемента между двумя другими
*/
TYPED_TEST(StaticListTest, EraseBetween) 
{
    // Act
    this->list.Push(this->TestValue() + 0);
    this->list.Push(this->TestValue() + 1);
    this->list.Push(this->TestValue() + 2);

    this->list.Erase(&this->list[1]);
    
    // Assert
    EXPECT_EQ(this->list.Count(), 2);
    EXPECT_EQ(this->list[0], this->TestValue());
    EXPECT_EQ(this->list[1], this->TestValue() + 2);
}

/*
    Проверка удаления первого элемента
*/
TYPED_TEST(StaticListTest, EraseFirst) 
{
    // Act
    this->list.Push(this->TestValue() + 0);
    this->list.Push(this->TestValue() + 1);
    this->list.Push(this->TestValue() + 2);

    this->list.Erase(&this->list[0]);
    
    // Assert
    EXPECT_EQ(this->list.Count(), 2);
    EXPECT_EQ(this->list[0], this->TestValue() + 1);
    EXPECT_EQ(this->list[1], this->TestValue() + 2);
}

/*
    Проверка удаления последнего элемента
*/
TYPED_TEST(StaticListTest, EraseLast) 
{
    // Act
    this->list.Push(this->TestValue() + 0);
    this->list.Push(this->TestValue() + 1);
    this->list.Push(this->TestValue() + 2);

    this->list.Erase(&this->list[2]);
    
    // Assert
    EXPECT_EQ(this->list.Count(), 2);
    EXPECT_EQ(this->list[0], this->TestValue());
    EXPECT_EQ(this->list[1], this->TestValue() + 1);
}

/*
    Проверка отработки for-each
*/
TYPED_TEST(StaticListTest, ForEachTest) 
{
    // Act
    size_t i = 0;
    for (; i < this->list.Size(); ++i)
        this->list.Push(this->TestValue() + i);
    
    // Assert
    i = 0;
    for (TypeParam& elem : this->list)
    {
        EXPECT_EQ(elem, this->TestValue() + i);
        i++;
    }
    i = 0;
    for (const TypeParam& elem : this->list)
    {
        EXPECT_EQ(elem, this->TestValue() + i);
        i++;
    }
}

/*
    Проверка отработки реверсивного for-each
*/
TYPED_TEST(StaticListTest, ReversedForEachTest) 
{
    // Act
    size_t i = 0;
    for (; i < this->list.Size(); ++i)
        this->list.Push(this->TestValue() + i);
    
    // Assert
    i = this->list.Size() - 1;
    for (TypeParam& elem : this->list.reversed())
    {
        EXPECT_EQ(elem, this->TestValue() + i);
        i--;
    }
    i = this->list.Size() - 1;
    for (const TypeParam& elem : this->list.reversed())
    {
        EXPECT_EQ(elem, this->TestValue() + i);
        i--;
    }
}


// Если тестируешь код, зависящий от железа — используй моки
// Пример с моком (нужен gmock):
/*
#include <gmock/gmock.h>

class MockGPIO {
public:
    MOCK_METHOD(void, writePin, (bool state), (virtual));
};

TEST(GPIOTest, WritePinCalled) {
    MockGPIO mock;
    EXPECT_CALL(mock, writePin(true)).Times(1);
    // ... вызов кода, который использует mock
}
*/