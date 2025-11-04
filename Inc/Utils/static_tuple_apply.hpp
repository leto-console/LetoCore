#ifndef INC_UTILS_STATIC_TUPLE_APPLY_HPP
#define INC_UTILS_STATIC_TUPLE_APPLY_HPP

#include <tuple>
#include <stdint.h>
#include <utility>

// 1. Контейнер для последовательности индексов
template<std::size_t... I>
struct index_sequence {};

// 2. Генератор последовательности (рекурсия)
template<std::size_t N, std::size_t... I>
struct make_index_sequence_impl : make_index_sequence_impl<N - 1, N - 1, I...> {};

// 3. Базовый случай остановки рекурсии
template<std::size_t... I>
struct make_index_sequence_impl<0, I...> {
    using type = index_sequence<I...>;
};

// 4. Удобный алиас
template<std::size_t N>
using make_index_sequence = typename make_index_sequence_impl<N>::type;

// 5. Алиас для получения индексов под размер кортежа
template<typename Tuple>
using make_tuple_indices = make_index_sequence<std::tuple_size<Tuple>::value>;

namespace detail {
    template<typename Func, typename Tuple, std::size_t... I>
    auto apply_impl(Func&& f, Tuple&& t, index_sequence<I...>)
        -> decltype(std::forward<Func>(f)(std::get<I>(std::forward<Tuple>(t))...))
    {
        return std::forward<Func>(f)(std::get<I>(std::forward<Tuple>(t))...);
    }
}

template<typename Func, typename Tuple>
auto apply(Func&& f, Tuple&& t)
-> decltype(detail::apply_impl(std::forward<Func>(f), std::forward<Tuple>(t),
    make_tuple_indices<typename std::remove_reference<Tuple>::type>{}))
{
    return detail::apply_impl(
        std::forward<Func>(f),
        std::forward<Tuple>(t),
        make_tuple_indices<typename std::remove_reference<Tuple>::type>{}
    );
}

#endif