add_compile_definitions(${PRESET_DEFINE_MACRO})

cmake_minimum_required(VERSION 3.10)
project("STM32F4_Libraries")

set(COMMON_BIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin/${PRESET_NAME})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${COMMON_BIN_DIR})

file(
	GLOB CPP_COURCES 
	"Src/*/*.cpp" "Src/*/*.c"
	"Inc/*/*.cpp" "Inc/*/*.c"
	"Src/*/*/*.cpp" "Src/*/*/*/*.c"
	"Inc/*/*/*.cpp" "Inc/*/*/*.c"
)
include_directories("Inc")

if(MSVC)
    add_compile_options(/wd4251)
endif()

if (${PRESET_DEFINE_MACRO} STREQUAL "USE_HAL_DRIVER")
	project("STM32F4_Libraries" C CXX ASM)

	file(GLOB HW_COURCES
		"Src/*.cpp" "Src/*.c"
	)

	add_library(${PROJECT_NAME} STATIC "${CPP_COURCES}" "${HW_COURCES}")

	target_compile_definitions(${PROJECT_NAME} PRIVATE
		USE_HAL_DRIVER
		STM32F411xE
	)

	# ------------------------------------------------------------------------------
	# Пути к заголовочным файлам (-I)
	# ------------------------------------------------------------------------------
	target_include_directories(${PROJECT_NAME} PUBLIC
		Inc
		Drivers/CMSIS/Device/ST/STM32F4xx/Include
		Drivers/CMSIS/Include
		Drivers/STM32F4xx_HAL_Driver/Inc
		Drivers/STM32F4xx_HAL_Driver/Inc/Legacy
	)

elseif (${PRESET_DEFINE_MACRO} STREQUAL "__WIN__")

	add_library("${PROJECT_NAME}" SHARED "${CPP_COURCES}")

	# Включаем предупреждения только для вашей цели
	target_compile_options("${PROJECT_NAME}" PRIVATE
		$<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall -Wextra -Wunused>
		$<$<CXX_COMPILER_ID:MSVC>:/W4 /wd4251>
	)

	#target_compile_options("${PROJECT_NAME}" PRIVATE
	#	$<$<CXX_COMPILER_ID:MSVC>:/WX>
	#	$<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Werror>
	#)
	## add_executable ("${PROJECT_NAME}" WIN32 "Core/Main/App.cpp" "${CPP_COURCES}")

elseif (${PRESET_DEFINE_MACRO} STREQUAL "__LINUX__")

	find_package(Qt6 COMPONENTS Widgets REQUIRED Core)
	qt_standard_project_setup()
	add_executable ("${PROJECT_NAME}" "Main/App.cpp" "${CPP_COURCES}")
	target_link_libraries("${PROJECT_NAME}" PRIVATE Qt6::Core Qt6::Widgets)

elseif (${PRESET_DEFINE_MACRO} STREQUAL "__TERMUX__")

	include(FetchContent)
	FetchContent_Declare(
		cpp-httplib
		GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
		GIT_TAG        v0.19.0
	)
	FetchContent_MakeAvailable(cpp-httplib)

	add_library("${PROJECT_NAME}" SHARED "${CPP_COURCES}")
	set_target_properties("${PROJECT_NAME}" PROPERTIES POSITION_INDEPENDENT_CODE ON)

	target_link_libraries(${PROJECT_NAME} PRIVATE httplib::httplib)
	target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

endif ()

target_compile_definitions("${PROJECT_NAME}" PRIVATE STM32F4_LIBRARIES_EXPORT)

# === Подключение подсистемы тестов ===
# Тесты собираются только если это не целевая платформа STM32
if (NOT ${PRESET_DEFINE_MACRO} STREQUAL "USE_HAL_DRIVER")
	enable_testing()
    add_subdirectory(Tests)
endif()
