/**
 * @file MinMaxChanger.hpp
 * @date Jul 11, 2025
 * @author Rakhimov T.
 */

#ifndef INC_DATA_MIN_MAX_CHANGER_HPP_
#define INC_DATA_MIN_MAX_CHANGER_HPP_

// Изменение данных в диапазоне
template <typename T>
class MinMaxChanger
{
public:
    MinMaxChanger(T* value, T min, T max, bool unbounded = false) 
        : value{ value }, min{ min }, max { max }, unbounded { unbounded }
    { }

	~MinMaxChanger() = default;

    void Change(T diff, bool plus = false)
    {
        if (plus) 
            Plus(diff);
        else 
            Minus(diff);
    }

    void Minus(T diff)
    {
        if (*value < min + diff)
        {
            if (!unbounded) return;

            diff--;
            *value = max - diff;
            return;
        }

        T new_value = *value - diff;

        if (new_value < min) return;
        *value = new_value;
    }

    void Plus(T diff)
    {
        if (*value + diff > max)
        {
            if (!unbounded) return;

            diff--;
            *value = min + diff;
            return;
        }

        T new_value = *value + diff;

        if (new_value > max) return;
        *value = new_value;
    }

private:
    T* value;
    T min, max;
    bool unbounded;
};

#endif