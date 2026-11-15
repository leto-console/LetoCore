#include <gtest/gtest.h>

#include <Data/ArenaAllocator.hpp>
#include <Data/List.hpp>

static ArenaAllocator<1024> allocator;

static void* Alloc(uint32_t size) { return allocator.Alloc(size); }
static void Free(const void* ptr) { return allocator.Free(ptr); }

class TestList : public ::testing::Test
{
public:
    LetoAllocator_V1 leto_alloc { &Alloc, &Free };

    int TestValue()
    {
        return 123456789;
    }

    void SetUp() override 
    {
        allocator.Clear();
    }

    void TearDown() override 
    {

    }
};

/*
    Проверка начальной инициализации
*/
TEST_F(TestList, InitCheck) 
{
    // Arrange
    List<int> list;

    List<int>::ConstIterator c_begin = list.begin();
    List<int>::ConstIterator c_end = list.begin();

    // Act, assert
    EXPECT_EQ(list.size(), 0);
    EXPECT_EQ(list.begin(), list.end());
    EXPECT_EQ(list.cbegin(), list.cend());
    EXPECT_EQ(c_begin, c_end);
    EXPECT_TRUE(list.empty());
}

/*
    Добавление элементов в список без аллокатора
*/
TEST_F(TestList, PushBackWithoutAllocator) 
{
    // Arrange
    List<int> list;

    // Act
    list.push_back(4);
    list.push_back(5);
    list.push_back(6);
    
    // Assert
    EXPECT_EQ(list.size(), 0);
    EXPECT_EQ(list.begin(), list.end());
    EXPECT_TRUE(list.empty());
}

/*
    Добавление элементов в конец списка с аллокатором
*/
TEST_F(TestList, PushBack) 
{
    // Arrange
    List<int> list{ &leto_alloc };
    EXPECT_TRUE(allocator.GetUsedBytes() == 0);

    // Act
    list.push_back(4);
    list.push_back(5);
    list.push_back(6);
    
    // Assert
    EXPECT_EQ(list.size(), 3);
    EXPECT_NE(list.begin(), list.end());
    EXPECT_FALSE(list.empty());
    EXPECT_TRUE(allocator.GetUsedBytes() > 0);
    
    int value;
    EXPECT_TRUE(list.get(0, value));
    EXPECT_EQ(value, 4);
    EXPECT_TRUE(list.get(1, value));
    EXPECT_EQ(value, 5);
    EXPECT_TRUE(list.get(2, value));
    EXPECT_EQ(value, 6);
    EXPECT_FALSE(list.get(3, value));

    EXPECT_TRUE(list.get(-1, value));
    EXPECT_EQ(value, 6);
    EXPECT_TRUE(list.get(-2, value));
    EXPECT_EQ(value, 5);
    EXPECT_TRUE(list.get(-3, value));
    EXPECT_EQ(value, 4);
    EXPECT_FALSE(list.get(-4, value));

    int* value_ptr;
    EXPECT_TRUE(list.get(0, value_ptr));
    *value_ptr = 777;
    
    EXPECT_TRUE(list.get(0, value));
    EXPECT_EQ(value, 777);
    EXPECT_TRUE(list.get(1, value));
    EXPECT_EQ(value, 5);
    EXPECT_TRUE(list.get(2, value));
    EXPECT_EQ(value, 6);
    EXPECT_FALSE(list.get(3, value));

    EXPECT_TRUE(list.get(-1, value));
    EXPECT_EQ(value, 6);
    EXPECT_TRUE(list.get(-2, value));
    EXPECT_EQ(value, 5);
    EXPECT_TRUE(list.get(-3, value));
    EXPECT_EQ(value, 777);
    EXPECT_FALSE(list.get(-4, value));
}

/*
    Добавление элементов в начало списка с аллокатором
*/
TEST_F(TestList, PushFront) 
{
    // Arrange
    List<int> list{ &leto_alloc };
    EXPECT_TRUE(allocator.GetUsedBytes() == 0);

    // Act
    list.push_back(4);
    list.push_back(5);
    list.push_back(6);
    list.push_front(111);
    
    // Assert
    EXPECT_EQ(list.size(), 4);
    EXPECT_NE(list.begin(), list.end());
    EXPECT_FALSE(list.empty());
    EXPECT_TRUE(allocator.GetUsedBytes() > 0);
    
    int value;
    EXPECT_TRUE(list.get(0, value));
    EXPECT_EQ(value, 111);
    EXPECT_TRUE(list.get(1, value));
    EXPECT_EQ(value, 4);
    EXPECT_TRUE(list.get(2, value));
    EXPECT_EQ(value, 5);
    EXPECT_TRUE(list.get(3, value));
    EXPECT_EQ(value, 6);
    EXPECT_FALSE(list.get(4, value));

    EXPECT_TRUE(list.get(-1, value));
    EXPECT_EQ(value, 6);
    EXPECT_TRUE(list.get(-2, value));
    EXPECT_EQ(value, 5);
    EXPECT_TRUE(list.get(-3, value));
    EXPECT_EQ(value, 4);
    EXPECT_TRUE(list.get(-4, value));
    EXPECT_EQ(value, 111);
    EXPECT_FALSE(list.get(-5, value));
}

/*
    Удаление элемента из конца списка с аллокатором
*/
TEST_F(TestList, PopBack) 
{
    // Arrange
    List<int> list{ &leto_alloc };
    EXPECT_TRUE(allocator.GetUsedBytes() == 0);

    // Act
    list.push_back(4);
    list.push_back(5);
    list.push_back(6);
    
    // Assert
    EXPECT_EQ(list.size(), 3);
    EXPECT_NE(list.begin(), list.end());
    EXPECT_FALSE(list.empty());
    EXPECT_TRUE(allocator.GetUsedBytes() > 0);

    // Act
    list.pop_back();
    
    // Assert
    EXPECT_EQ(list.size(), 2);

    int value;
    EXPECT_TRUE(list.get(0, value));
    EXPECT_EQ(value, 4);
    EXPECT_TRUE(list.get(1, value));
    EXPECT_EQ(value, 5);
    EXPECT_FALSE(list.get(2, value));
}

/*
    Удаление элемента из начала списка с аллокатором
*/
TEST_F(TestList, PopFront) 
{
    // Arrange
    List<int> list{ &leto_alloc };
    EXPECT_TRUE(allocator.GetUsedBytes() == 0);

    // Act
    list.push_back(4);
    list.push_back(5);
    list.push_back(6);
    
    // Assert
    EXPECT_EQ(list.size(), 3);
    EXPECT_NE(list.begin(), list.end());
    EXPECT_FALSE(list.empty());
    EXPECT_TRUE(allocator.GetUsedBytes() > 0);

    // Act
    list.pop_front();
    
    // Assert
    EXPECT_EQ(list.size(), 2);

    int value;
    EXPECT_TRUE(list.get(0, value));
    EXPECT_EQ(value, 5);
    EXPECT_TRUE(list.get(1, value));
    EXPECT_EQ(value, 6);
    EXPECT_FALSE(list.get(2, value));
}

/*
    Удаление элемента по итератору с аллокатором
*/
TEST_F(TestList, EraseAfter) 
{
    // Arrange
    List<int> list{ &leto_alloc };
    EXPECT_TRUE(allocator.GetUsedBytes() == 0);

    // Act
    list.push_back(4);
    list.push_back(5);
    list.push_back(6);
    
    // Assert
    EXPECT_EQ(list.size(), 3);
    EXPECT_NE(list.begin(), list.end());
    EXPECT_FALSE(list.empty());
    EXPECT_TRUE(allocator.GetUsedBytes() > 0);

    // Act
    list.erase_after(list.begin());
    
    // Assert
    EXPECT_EQ(list.size(), 2);

    int value;
    EXPECT_TRUE(list.get(0, value));
    EXPECT_EQ(value, 4);
    EXPECT_TRUE(list.get(1, value));
    EXPECT_EQ(value, 6);
    EXPECT_FALSE(list.get(2, value));
}

/*
    Удаление элемента по итератору с аллокатором
*/
TEST_F(TestList, Erase) 
{
    // Arrange
    List<int> list{ &leto_alloc };
    EXPECT_TRUE(allocator.GetUsedBytes() == 0);

    // Act
    list.push_back(4);
    list.push_back(5);
    list.push_back(6);
    
    // Assert
    EXPECT_EQ(list.size(), 3);
    EXPECT_NE(list.begin(), list.end());
    EXPECT_FALSE(list.empty());
    EXPECT_TRUE(allocator.GetUsedBytes() > 0);

    // Act
    list.erase(list.begin()+1);
    
    // Assert
    EXPECT_EQ(list.size(), 2);

    int value;
    EXPECT_TRUE(list.get(0, value));
    EXPECT_EQ(value, 4);
    EXPECT_TRUE(list.get(1, value));
    EXPECT_EQ(value, 6);
    EXPECT_FALSE(list.get(2, value));
}

/*
    Вставка элемента после итератора с аллокатором
*/
TEST_F(TestList, InsertAfter) 
{
    // Arrange
    List<int> list{ &leto_alloc };
    EXPECT_TRUE(allocator.GetUsedBytes() == 0);

    // Act
    list.push_back(4);
    list.push_back(5);
    list.push_back(6);
    
    // Assert
    EXPECT_EQ(list.size(), 3);
    EXPECT_NE(list.begin(), list.end());
    EXPECT_FALSE(list.empty());
    EXPECT_TRUE(allocator.GetUsedBytes() > 0);

    // Act
    list.insert_after(list.begin(), 777);
    
    // Assert
    EXPECT_EQ(list.size(), 4);

    int value;
    EXPECT_TRUE(list.get(0, value));
    EXPECT_EQ(value, 4);
    EXPECT_TRUE(list.get(1, value));
    EXPECT_EQ(value, 777);
    EXPECT_TRUE(list.get(2, value));
    EXPECT_EQ(value, 5);
    EXPECT_FALSE(list.get(6, value));
}

/*
    Проверка for-each
*/
TEST_F(TestList, ForeachCheck) 
{
    // Arrange
    List<int> list{ &leto_alloc };

    // Act
    list.push_back(4);
    list.push_back(5);
    list.push_back(6);
    list.insert_after(list.begin()+1, 777);
    list.push_front(3);
    list.push_front(2);
    list.push_front(1);

    // Assert
    int check[] { 1, 2, 3, 4, 5, 777, 6 };

    size_t idx = 0;
    for (int& val : list)
    {
        EXPECT_EQ(val, check[idx++]);
    }
    
    idx = 0;
    for (const int& val : list)
    {
        EXPECT_EQ(val, check[idx++]);
    }

}
