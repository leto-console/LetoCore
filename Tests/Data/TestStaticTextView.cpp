
#include <gtest/gtest.h>

#include <Data/StaticTextView.hpp>

/*
    Ёмкость вида и ёмкость текста должны совпадать: StaticText<N> хранит N - 1
    символов и нулевой терминал. Раньше вид сообщал N, и чтение по
    view[view.Capacity()] уходило за границу буфера.
*/
TEST(TestStaticTextView, MatchesTextCapacity)
{
    StaticText<260> text;

    const StaticTextView view{ text };

    EXPECT_EQ(view.Capacity(), text.Capacity());
    EXPECT_EQ(view.Capacity(), 259u);
}

TEST(TestStaticTextView, LastValidIndexIsTerminator)
{
    StaticText<8> text{ "1234567" };

    const StaticTextView view{ text };

    // Индекс Capacity() — нулевой терминал, а не восьмой символ
    EXPECT_EQ(view.Capacity(), 7u);
    EXPECT_EQ(view[view.Capacity()], '\0');
    EXPECT_EQ(view.TextLength(), 7u);
}

TEST(TestStaticTextView, HoldsCapacityCharacters)
{
    StaticText<8> text;

    const StaticTextView view{ text };

    EXPECT_EQ(text.Capacity(), view.Capacity());
    EXPECT_GE(sizeof(text), view.Capacity() + 1);
}

/*
    Вид на строку без известной ёмкости: capacity — длина строки, и по индексу
    Capacity() тоже лежит терминал.
*/
TEST(TestStaticTextView, StringViewKeepsTerminatorInRange)
{
    const char* literal = "abc";

    const StaticTextView view{ literal };

    EXPECT_EQ(view.Capacity(), 3u);
    EXPECT_EQ(view.TextLength(), 3u);
    EXPECT_EQ(view[view.Capacity()], '\0');
}
