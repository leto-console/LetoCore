/**
 * @file Range.hpp
 * @date Mar 31, 2026
 * @author Timur
 */

#ifndef INC_DATA_RANGE_HPP_
#define INC_DATA_RANGE_HPP_

/// @brief Класс-обертка для реализации функционала for-each
/// @tparam Iterator Тип возвращаемого объекта
template <typename Iterator>
struct Range {
    Iterator begin_it;
    Iterator end_it;
    Iterator begin() const { return begin_it; }
    Iterator end()   const { return end_it; }
};

#endif
