/**
 * @file CheckAPI.hpp
 * @date Oct 11, 2026
 * @author Rakhimov T.
 */
#pragma once

#define LETO_CHECK_API(api, module, function, default_value) \
do \
{ \
    if (!(api) || !(api->module) || !(api->module->function)) \
        return default_value; \
} \
while (0)
