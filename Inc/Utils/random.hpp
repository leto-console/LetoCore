#ifndef INC_UTILS_RANDOM_HPP
#define INC_UTILS_RANDOM_HPP

#include "LibrariesExport.h"

#include <cstdlib>

// Generate a random float number [0.0 to 1.0]
extern LETO_CORE_EXPORT float RandomFloat();

// Generate a random number [a to b)
template <typename Type>
Type Random(Type a, Type b)
{
    float r = RandomFloat();
    if (r >= 1.0f)
        return (Type) (b - 1);
    return (Type) (r * (b - a)) + a;
}

#endif 
