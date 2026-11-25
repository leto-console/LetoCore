#include <gtest/gtest.h>

#include <Data/ArenaAllocator.hpp>

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

static constexpr size_t MAX_ALIGN = alignof(std::max_align_t);

/*
    Запрос заведомо большего размера: nullptr, либо исключение под WIN32
    (поведение сохранено с прежней реализации)
*/
template <typename Arena>
static bool AllocFails(Arena& arena, uint32_t size)
{
#ifdef WIN32
    try
    {
        arena.Alloc(size);
        return false;
    }
    catch (const char*)
    {
        return true;
    }
#else
    return arena.Alloc(size) == nullptr;
#endif
}

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
    Произвольный выровненный указатель внутри живой полезной нагрузки не должен
    ломать арену: перед ним лежат данные объекта, а не заголовок аллокации
*/
TEST(TestArenaAllocator, JunkPointerKeepsArenaConsistent)
{
    // Arrange
    ArenaAllocator<1024> arena;

    uint8_t* first = static_cast<uint8_t*>(arena.Alloc(MAX_ALIGN * 4));
    ASSERT_NE(first, nullptr);
    ASSERT_NE(arena.Alloc(16), nullptr);

    /*
            Заполняем полезную нагрузку значением, которое заведомо больше занятого
            объёма: иначе чужие байты по месту фиктивного заголовка могут оказаться
            нулями и проверка пройдёт случайно
    */
    memset(first, 0xFF, MAX_ALIGN * 4);

    // Act: выровненный, но не начальный адрес внутри первой полезной нагрузки
    arena.Clear(first + MAX_ALIGN);
    arena.Free(first + MAX_ALIGN);

    // Assert: инвариант вместимости не нарушен, арена пригодна к работе
    EXPECT_LE(arena.GetUsedBytes(), arena.GetTotalBytes());
    EXPECT_GE(arena.GetUsedBytes(), 0);

    arena.Clear();
    EXPECT_EQ(arena.GetUsedBytes(), 0);

    void* again = arena.Alloc(32);
    ASSERT_NE(again, nullptr);
    EXPECT_EQ(reinterpret_cast<uintptr_t>(again) % MAX_ALIGN, 0u);
}

/*
    Невыровненный указатель не может быть началом полезной нагрузки, поэтому
    откат по нему не выполняется даже внутри границ арены
*/
TEST(TestArenaAllocator, MisalignedPointerRejected)
{
    // Arrange
    ArenaAllocator<1024> arena;

    uint8_t* first = static_cast<uint8_t*>(arena.Alloc(MAX_ALIGN * 2));
    ASSERT_NE(first, nullptr);
    ASSERT_NE(arena.Alloc(16), nullptr);
    const int used_before = arena.GetUsedBytes();

    // Act, assert: смещение на 1 байт нарушает выравнивание
    arena.Clear(first + 1);
    EXPECT_EQ(arena.GetUsedBytes(), used_before);

    arena.Free(first + 1);
    EXPECT_EQ(arena.GetUsedBytes(), used_before);
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
    Тип с максимальным поддерживаемым выравниванием после аллокаций нечётной
    длины тоже выровнен. Самый строгий тип — std::max_align_t, поэтому
    guarantee проверяется на нём: на MSVC и ARM max_align_t == 8, на x86-64
    glibc — 16
*/
TEST(TestArenaAllocator, MaxAlignmentAfterOddAllocation)
{
    static_assert(alignof(std::max_align_t) == static_cast<size_t>(MAX_ALIGN), "MAX_ALIGN должен совпадать с max_align_t");

    // Arrange
    ArenaAllocator<1024> arena;

    ASSERT_NE(arena.Alloc(1), nullptr);
    ASSERT_NE(arena.Alloc(3), nullptr);
    ASSERT_NE(arena.Alloc(17), nullptr);

    // Act
    std::max_align_t* object = arena.Make<std::max_align_t>();
    ASSERT_NE(object, nullptr);

    // Assert
    EXPECT_EQ(reinterpret_cast<uintptr_t>(object) % MAX_ALIGN, 0u);
    EXPECT_EQ(reinterpret_cast<uintptr_t>(object) % alignof(std::max_align_t), 0u);

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
    struct alignas(std::max_align_t) Slot
    {
        uint32_t value;
        uint8_t padding[sizeof(std::max_align_t) - sizeof(uint32_t)];

        Slot() : value{}, padding{} {}
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
        new (array + i) Slot{};
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

    // Запрет не ломает арену: она по-прежнему отдаёт память
    void* third = arena.Alloc(30);
    ASSERT_NE(third, nullptr);
    EXPECT_GT(arena.GetUsedBytes(), used_before);
}

/*
    Переполнение арены не отдаёт память и не портит её состояние
*/
TEST(TestArenaAllocator, Overflow)
{
    // Arrange
    ArenaAllocator<256> arena;

    ASSERT_NE(arena.Alloc(100), nullptr);
    const int used_before = arena.GetUsedBytes();

    // Act, assert: запрос больше арены целиком
    EXPECT_TRUE(AllocFails(arena, 1000));
    EXPECT_EQ(arena.GetUsedBytes(), used_before);

    // Act, assert: укладывается в заголовок, но не в полезную нагрузку
    EXPECT_TRUE(AllocFails(arena, 250));
    EXPECT_EQ(arena.GetUsedBytes(), used_before);

    // Act: арена по-прежнему работоспособна
    void* ptr = arena.Alloc(100);
    ASSERT_NE(ptr, nullptr);
    EXPECT_EQ(reinterpret_cast<uintptr_t>(ptr) % MAX_ALIGN, 0u);
    EXPECT_GT(arena.GetUsedBytes(), used_before);

    // Act: добиваем арену, пока она отдаёт память
    for (int guard = 0; guard < 4096 && !AllocFails(arena, 1); ++guard)
        ASSERT_LE(arena.GetUsedBytes(), arena.GetTotalBytes());

    // Assert: занятое не превысило ёмкость, и дальше память не выдаётся
    const int used_full = arena.GetUsedBytes();
    EXPECT_LE(used_full, arena.GetTotalBytes());
    EXPECT_TRUE(AllocFails(arena, 1));
    EXPECT_EQ(arena.GetUsedBytes(), used_full);
}
