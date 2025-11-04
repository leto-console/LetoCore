/*
 *	CallStruct.hpp
 *
 *	Created on: Mar 21, 2026
 *		Author: Timur
 */

#ifndef INC_ABSTRACT_CALLS_CALL_STRUCT_HPP_
#define INC_ABSTRACT_CALLS_CALL_STRUCT_HPP_

#include <cstdint>

// Абстракная структура вызова, позволяющая расширять поведение класса без изменения базового интерфейса класса
template <typename Impl, uint32_t CallID>
struct CallStruct
{
	using ArgType		= typename Impl::Arg;
	using AnswerType	= typename Impl::Answer;

	// Идентификатор вызова
	const static uint32_t ID = CallID;

	// Структура содержащая информацию об аргументе вызова
	ArgType arg{};

	// Структура содержащая информацию об ответе на вызов
	AnswerType answer{};

    inline static const ArgType* GetCallArg(const void* arg)
    {
        return static_cast<const ArgType*>(arg);
    }

    inline static AnswerType* GetCallAnswer(void* answer)
    {
        return static_cast<AnswerType*>(answer);
    }
};

/*
    Ниже - макросная поддержка обработки системных вызовов
*/

#define STRUCT_BODY(...) struct { __VA_ARGS__; }
#define DECLARE_ARG(...) STRUCT_BODY(__VA_ARGS__)
#define DECLARE_ANSWER(...) STRUCT_BODY(__VA_ARGS__)

/// Объявить структуру вызова
#define DECLARE_CALL(StructName, CallID, ArgStruct, AnswerStruct) \
struct __##StructName##_Data__ { using Arg = ArgStruct; using Answer = AnswerStruct; }; \
struct StructName : public CallStruct<__##StructName##_Data__, CallID> { };

/// Реализовать обработку вызова функции
#define DECLARE_CALL_IMPL(StructName) \
bool Call(const StructName::ArgType& arg, StructName::AnswerType& answer)

/// Проверить и при соответствии ID выполнить вызов функции
#define CHECK_CALL(StructName) \
if (ID == StructName::ID) { return Call(*static_cast<const StructName::ArgType*>(arg_ptr), *static_cast<StructName::AnswerType*>(answer_ptr)); }

/// Центральная функция обработчик вызовов
#define DECLARE_CALL_HANDLER(...)\
bool Call(uint32_t ID, const void* arg_ptr, void* answer_ptr)\
{\
    if (!arg_ptr || !answer_ptr) return false;\
\
    __VA_ARGS__\
\
    return false;\
}

/* Пример реализации


#include "../LibSupport/LibVersion.hpp"
#include <AbstractCalls/CallStruct.hpp>

// Интерфейс взаимодействия с системой
struct SystemAPI
{
	LibVersion version;

	// Системный вызов
	bool (*RawCall) (uint32_t ID, const void* arg, void* answer);
		
	template <typename Impl, uint32_t CallID>
	bool Call(CallStruct<Impl, CallID>& call_struct)
	{
		if (!RawCall) return false;
		return RawCall(CallID, &call_struct.arg, &call_struct.answer);
	}
	
	SystemAPI(uint16_t major, uint16_t minor) 
		: version{ major, minor }, RawCall{ nullptr }
	{ }
};

static_assert(std::is_standard_layout<SystemAPI>::value, "SystemAPI должен быть standard_layout");

*/

#endif
