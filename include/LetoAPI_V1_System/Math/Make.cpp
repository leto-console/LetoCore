#include "Make.hpp"

#include <cstdio>
#include <cstdarg>
#include <cmath>

using namespace std;

// ====================================================================================================

#include <Graphics/DefaultFont.hpp>

bool _isnanf_leto(float x) { return isnan(x); }
bool _isinff_leto(float x) { return isinf(x); }

// ====================================================================================================

const MathAPI_V1* Make_MathAPI()
{
    static const MathAPI_V1 api
    {
        &::acosf,
        &::asinf,
        &::atanf,
        &::atan2f,
        &::cosf,
        &::sinf,
        &::tanf,
        &::floorf,
        &::ceilf,
        &::roundf,
        &::sqrtf,
        &::powf,
        &::fabsf,
        &::hypotf,
        &::expf,
        &::logf,
        &::fmodf,
        &::copysignf,
        &_isnanf_leto,
        &_isinff_leto
    };
    
    return &api;
}
