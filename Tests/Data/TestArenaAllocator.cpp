#include <gtest/gtest.h>

#include <Data/ArenaAllocator.hpp>

#include <cstdint>
#include <string>
#include <vector>

static constexpr size_t MAX_ALIGN = alignof(std::max_align_t);

/*
    Полезная нагрузка каждой аллокации выровнена на alignof(std::max_align_t)
*/
TEST(TestArenaAllocator, PayloadAlignment)
{
    // Arrange
    ArenaAllocator<64 * 1024> arena;

    uint32_t seed = 12345;
    for (int round = 0; round < 100; ++round)
    {
        arena.Clear();
        std::vector<void*> pointers;

        // Act
        for (int i = 0; i < 200; ++i)
        {
            seed = seed * 1664525u + 1013904223u;
            const uint32_t size = 1 + seed % 300;

            // с запасом на округление payload до Alignment
            if (arena.GetFreeBytes() < static_cast<int>(size) + 2 * static_cast<int>(MAX_ALIGN))
                break;

            void* ptr = arena.Alloc(size);
            ASSERT_NE(ptr, nullptr);
            ASSERT_EQ(reinterpret_cast<uintptr_t>(ptr) % MAX_ALIGN, 0u)
                << "round " << round << ", size " << size;

            pointers.push_back(ptr);
        }

        // Assert: возврат в LIFO порядке очищает арену полностью
        while (!pointers.empty())
        {
            arena.Free(pointers.back());
            pointers.pop_back();
        }
        EXPECT_EQ(arena.GetUsedBytes(), 0);
    }
}

/*
    Free в порядке LIFO возвращает занятый объём к прежнему значению
*/
TEST(TestArenaAllocator, FreeLifo)
{
    // Arrange
    ArenaAllocator<1024> arena;

    void* first = arena.Alloc(1);
    ASSERT_NE(first, nullptr);
    const int used_after_first = arena.GetUsedBytes();

    void* second = arena.Alloc(3);
    ASSERT_NE(second, nullptr);
    const int used_after_second = arena.GetUsedBytes();
    EXPECT_GT(used_after_second, used_after_first);

    // Act, assert
    arena.Free(second);
    EXPECT_EQ(arena.GetUsedBytes(), used_after_first);

    arena.Free(first);
    EXPECT_EQ(arena.GetUsedBytes(), 0);
}

/*
    Free не-верхней аллокации не меняет занятый объём
*/
TEST(TestArenaAllocator, FreeNotTop)
{
    // Arrange
    ArenaAllocator<1024> arena;

    void* first = arena.Alloc(10);
    void* second = arena.Alloc(20);
    ASSERT_NE(first, nullptr);
    ASSERT_NE(second, nullptr);
    const int used_before = arena.GetUsedBytes();

    // Act: первая аллокация не верхняя
    arena.Free(first);

    // Assert: состояние не изменилось
    EXPECT_EQ(arena.GetUsedBytes(), used_before);

    // Act, assert: верхняя освобождается, но первая остаётся занятой
    arena.Free(second);
    EXPECT_LT(arena.GetUsedBytes(), used_before);
    EXPECT_GT(arena.GetUsedBytes(), 0);

    // Act, assert: теперь первая стала верхней
    arena.Free(first);
    EXPECT_EQ(arena.GetUsedBytes(), 0);
}

/*
    Clear(ptr) откатывает арену к состоянию перед любой аллокацией, а не только
    перед верхней
*/
TEST(TestArenaAllocator, ClearByPointerOfAnyAllocation)
{
    // Arrange
    ArenaAllocator<1024> arena;

    void* first = arena.Alloc(7);
    ASSERT_NE(first, nullptr);
    const int used_after_first = arena.GetUsedBytes();

    void* second = arena.Alloc(13);
    void* third = arena.Alloc(21);
    ASSERT_NE(second, nullptr);
    ASSERT_NE(third, nullptr);

    // Act: вторая аллокация не верхняя
    arena.Clear(second);

    // Assert
    EXPECT_EQ(arena.GetUsedBytes(), used_after_first);

    // Act: арена снова пригодна к использованию с того же места
    void* fourth = arena.Alloc(5);
    ASSERT_NE(fourth, nullptr);
    EXPECT_GT(arena.GetUsedBytes(), used_after_first);
    EXPECT_EQ(reinterpret_cast<uintptr_t>(fourth) % MAX_ALIGN, 0u);

    // Act: откат по самой первой аллокации очищает арену
    arena.Clear(first);
    EXPECT_EQ(arena.GetUsedBytes(), 0);
}

/*
    Clear() очищает арену полностью
*/
TEST(TestArenaAllocator, ClearAll)
{
    // Arrange
    ArenaAllocator<1024> arena;

    for (uint32_t size = 1; size <= 16; ++size)
        ASSERT_NE(arena.Alloc(size), nullptr);
    ASSERT_GT(arena.GetUsedBytes(), 0);

    // Act
    arena.Clear();

    // Assert
    EXPECT_EQ(arena.GetUsedBytes(), 0);
}

/*
    Аллокация объекта с alignof == 16 после аллокаций нечётной длины выровнена
*/
TEST(TestArenaAllocator, AlignOf16AfterOddAllocation)
{
    struct alignas(16) Aligned16
    {
        uint8_t bytes[16];
    };
    static_assert(alignof(Aligned16) == 16, "Тип должен требовать выравнивания 16");

    // Arrange
    ArenaAllocator<1024> arena;

    ASSERT_NE(arena.Alloc(1), nullptr);
    ASSERT_NE(arena.Alloc(3), nullptr);
    ASSERT_NE(arena.Alloc(17), nullptr);

    // Act
    Aligned16* object = arena.Make<Aligned16>();
    ASSERT_NE(object, nullptr);

    // Assert
    EXPECT_EQ(reinterpret_cast<uintptr_t>(object) % alignof(Aligned16), 0u);

    object->~Aligned16();
    arena.Free(object);
}

/*
    Объект с std::string внутри арены не должен попадать в невыровненную память
*/
TEST(TestArenaAllocator, ObjectWithStringInArena)
{
    struct StoredAppInfo
    {
        uint16_t id;
        std::string name;
        std::string path;
        char tail[260];
    };

    // Arrange
    ArenaAllocator<64 * 1024> arena;

    for (int round = 0; round < 5; ++round)
    {
        arena.Clear();
        std::vector<StoredAppInfo*> items;

        // Act
        for (int i = 0; i < 16; ++i)
        {
            // Нечётная длина предыдущей аллокации ломала бы выравнивание без правки
            ASSERT_NE(arena.Alloc(1 + i % 3), nullptr);

            StoredAppInfo* item = arena.Make<StoredAppInfo>();
            ASSERT_NE(item, nullptr);
            ASSERT_EQ(reinterpret_cast<uintptr_t>(item) % alignof(StoredAppInfo), 0u)
                << "round " << round << ", index " << i;

            item->id = static_cast<uint16_t>(i);
            item->name.assign(90, static_cast<char>('a' + i));
            item->path.assign(90, static_cast<char>('A' + i));
            items.push_back(item);
        }

        // Assert
        for (size_t i = 0; i < items.size(); ++i)
        {
            EXPECT_EQ(items[i]->id, i);
            EXPECT_EQ(items[i]->name, std::string(90, static_cast<char>('a' + i)));
            EXPECT_EQ(items[i]->path, std::string(90, static_cast<char>('A' + i)));
        }

        for (auto it = items.rbegin(); it != items.rend(); ++it)
            (*it)->~StoredAppInfo();

        arena.Clear();
        EXPECT_EQ(arena.GetUsedBytes(), 0);
    }
}

/*
    Массив объектов в арене: выровнено не только начало блока, но и каждый элемент
    (путь GameCenter: Alloc(sizeof(AppInfo) * count))
*/
TEST(TestArenaAllocator, ObjectArrayAlignment)
{
    struct alignas(16) Slot
    {
        uint32_t value;
        uint8_t padding[12];
    };
    static_assert(sizeof(Slot) % alignof(Slot) == 0, "Элементы массива должны сохранять выравнивание");

    // Arrange
    ArenaAllocator<64 * 1024> arena;
    constexpr uint32_t count = 16;

    // аллокация нечётной длины перед массивом не должна ломать его выравнивание
    ASSERT_NE(arena.Alloc(1), nullptr);

    // Act
    Slot* array = static_cast<Slot*>(arena.Alloc(sizeof(Slot) * count));
    ASSERT_NE(array, nullptr);

    // Assert
    for (uint32_t i = 0; i < count; ++i)
        EXPECT_EQ(reinterpret_cast<uintptr_t>(&array[i]) % alignof(Slot), 0u) << "index " << i;

    for (uint32_t i = 0; i < count; ++i)
        array[i].value = i;
    for (uint32_t i = 0; i < count; ++i)
        EXPECT_EQ(array[i].value, i);

    arena.Free(array);
}

/*
    При FreeAllowed == false освобождение запрещено
*/
TEST(TestArenaAllocator, FreeNotAllowed)
{
    // Arrange
    ArenaAllocator<1024, false> arena;

    void* first = arena.Alloc(10);
    ASSERT_NE(first, nullptr);
    void* second = arena.Alloc(20);
    ASSERT_NE(second, nullptr);
    const int used_before = arena.GetUsedBytes();
    ASSERT_GT(used_before, 0);

    // Act, assert
    arena.Free(second);
    EXPECT_EQ(arena.GetUsedBytes(), used_before);

    arena.Clear(second);
    EXPECT_EQ(arena.GetUsedBytes(), used_before);

    arena.Clear(first);
    EXPECT_EQ(arena.GetUsedBytes(), used_before);

    arena.Clear();
    EXPECT_EQ(arena.GetUsedBytes(), used_before);
}

/*
    Переполнение арены возвращает nullptr и не портит состояние
*/
TEST(TestArenaAllocator, Overflow)
{
    // Arrange
    ArenaAllocator<256> arena;

    ASSERT_NE(arena.Alloc(100), nullptr);
    const int used_before = arena.GetUsedBytes();

    // Act, assert: больше, чем осталось
    EXPECT_EQ(arena.Alloc(1000), nullptr);
    EXPECT_EQ(arena.GetUsedBytes(), used_before);

    // укладывается в заголовок, но не в полезную нагрузку
    EXPECT_EQ(arena.Alloc(250), nullptr);
    EXPECT_EQ(arena.GetUsedBytes(), used_before);

    // Act: арена по-прежнему работоспособна
    void* ptr = arena.Alloc(100);
    ASSERT_NE(ptr, nullptr);
    EXPECT_EQ(reinterpret_cast<uintptr_t>(ptr) % MAX_ALIGN, 0u);
    EXPECT_GT(arena.GetUsedBytes(), used_before);

    // Assert: ровно оставшегося места хватает, а байтом больше — уже нет
    EXPECT_NE(arena.Alloc(16), nullptr);
    EXPECT_EQ(arena.GetUsedBytes(), static_cast<int>(256));
    EXPECT_EQ(arena.Alloc(1), nullptr);
    EXPECT_EQ(arena.GetUsedBytes(), static_cast<int>(256));
}
