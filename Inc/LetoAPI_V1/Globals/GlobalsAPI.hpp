/**
 * @file GlobalsAPI_V1.hpp
 * @date May 23, 2026
 * @author Rakhimov T.
 */

#ifndef INC_LETO_API_V1_GLOBALS_GLOBALS_API_V1_HPP_
#define INC_LETO_API_V1_GLOBALS_GLOBALS_API_V1_HPP_

#include <cstdint>
#include <type_traits>

#include <Data/IAllocator.hpp>

// Выравнивание по границе 4 байт 
#pragma pack(push, 4)

/// Функции для работы с глобальными контекстом
struct GlobalsAPI_V1
{
    // ===================================================
    //           Управление динамической памятью          
    // ===================================================

    /**
     * @brief Выделить память в динамической памяти приложения
     * 
     * @param size Размер выделяемой памяти в байтах
     * @return Указатель на выделенный блок памяти
     */
    void* (*Alloc)(uint32_t size);

    /**
     * @brief Освободить память в динамической памяти приложения
     * 
     * @param ptr Указатель на блок памяти
     */
    void (*Free)(const void* ptr);

    /// TODO: Внимание! Потенциальная точка отказа
    /// аллокатор может убить всё приложение
    /**
     * @brief Получить аллокатор динамической памяти приложения
     */
    IAllocator* (*GetAllocator)();

    // ===================================================
    //                Управление состоянием                     
    // ===================================================

    /**
     * @brief Получить отладочный режим работы системы
     * @return `true` - включен, `false` - выключен
     */
    bool (*GetDebugMode)();

    /**
     * @brief Получить количество миллисекунд, прошедшее с момента запуска МК
     */
    uint32_t (*GetCurrentMs)();
};

#pragma pack(pop)

static_assert(std::is_standard_layout<GlobalsAPI_V1>::value, "GlobalsAPI_V1 должен быть standard_layout");

#endif
