/**
 * @file LetoNewDelete.hpp
 * @date Jun 24, 2026
 * @author Rakhimov T.
 */

#ifndef INC_LETO_API_V1_APPLICATION_LETO_NEW_DELETE_HPP_
#define INC_LETO_API_V1_APPLICATION_LETO_NEW_DELETE_HPP_

#include <new>

#include <LetoAPI_V1/LetoAPI_V1.hpp>
#include <Data/ArenaAllocator.hpp>

static ArenaAllocator<512> pre_leto_alloc;

// 1. Standard single-object new
void* operator new(std::size_t size) {
    void* ptr{};
    if (leto_api_v1)
        ptr = leto_api_v1->Globals->Alloc(size);
    else
        ptr = pre_leto_alloc.Alloc(size);
    if (!ptr)
    {
#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)
        // For Windows, Linux, and STM32 (if C++ exceptions are enabled)
        throw std::bad_alloc();
#else
        // For bare-metal STM32 with -fno-exceptions.
        // Returning nullptr from a standard new causes Undefined Behavior.
        #if defined(__arm__)
            asm("bkpt 0"); // Hardware breakpoint for the debugger
        #endif
        while(1); // Infinite loop to halt execution
#endif
    }
    return ptr;
}

// 2. Standard array new[]
void* operator new[](std::size_t size) {
    return operator new(size);
}

// 3. No-throw single-object new (nothrow)
void* operator new(std::size_t size, const std::nothrow_t&) noexcept {
    return leto_api_v1->Globals->Alloc(size);
}

// 4. No-throw array new[] (nothrow)
void* operator new[](std::size_t size, const std::nothrow_t&) noexcept {
    return leto_api_v1->Globals->Alloc(size);
}

// 5. Single-object delete
void operator delete(void* ptr) noexcept {
    leto_api_v1->Globals->Free(ptr);
}

// 6. Array delete[]
void operator delete[](void* ptr) noexcept {
    leto_api_v1->Globals->Free(ptr);
}

// 7. Sized single-object delete (C++14)
void operator delete(void* ptr, std::size_t size) noexcept {
    (void)size; // Suppress unused parameter warning
    leto_api_v1->Globals->Free(ptr);
}

// 8. Sized array delete[] (C++14)
void operator delete[](void* ptr, std::size_t size) noexcept {
    (void)size; // Suppress unused parameter warning
    leto_api_v1->Globals->Free(ptr);
}

#else
    #error "LetoNewDelete.hpp is included more than once in the project! This violates ODR."
#endif
