#include "Make.hpp"

#include <cstdio>
#include <cstdarg>
#include <cmath>

using namespace std;

// ====================================================================================================

#include <Graphics/DefaultFont.hpp>

float _acosf(float x) { return acosf(x); }
float _asinf(float x) { return asinf(x); }
float _atanf(float x) { return atanf(x); }
float _atan2f(float y, float x) { return atan2f(y, x); }

float _cosf(float x) { return cosf(x); }
float _sinf(float x) { return sinf(x); }
float _tanf(float x) { return tanf(x); }

float _floorf(float x) { return floorf(x); }
float _ceilf(float x) { return ceilf(x); }
float _roundf(float x) { return roundf(x); }

float _sqrtf(float x) { return sqrtf(x); }
float _powf(float x, float y) { return powf(x, y); }
float _fabs(float x) { return fabs(x); }
float _hypotf(float x, float y) { return hypotf(x, y); }

float _expf(float x) { return expf(x); }
float _logf(float x) { return logf(x); }

float _fmodf(float x, float y) { return fmodf(x, y); }
float _copysignf(float number, float sign) { return copysignf(number, sign); }
bool _isnanf_leto(float x) { return isnan(x); }
bool _isinff_leto(float x) { return isinf(x); }

// ====================================================================================================

/// TODO: попробовать избавиться от функций прослоек

const MathAPI_V1* Make_MathAPI()
{
    static const MathAPI_V1 api
    {
        &_acosf,
        &_asinf,
        &_atanf,
        &_atan2f,
        &_cosf,
        &_sinf,
        &_tanf,
        &_floorf,
        &_ceilf,
        &_roundf,
        &_sqrtf,
        &_powf,
        &_fabs,
        &_hypotf,
        &_expf,
        &_logf,
        &_fmodf,
        &_copysignf,
        &_isnanf_leto,
        &_isinff_leto
    };
    
    return &api;
}
