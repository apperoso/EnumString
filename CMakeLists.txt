cmake_minimum_required(VERSION 3.20.0)

project(EnumString VERSION 0.1.0 LANGUAGES CXX)

# Project wide setup
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

# Compiler-specific options
if (MSVC)
	# Set NOMINMAX to ensure Microsoft's min() & max() macros don't clash
	add_compile_definitions(NOMINMAX)

	# Turn on all warnings, warnings as errors but exclude external code
	# See https://github.com/cpp-best-practices/cppbestpractices/blob/master/02-Use_the_Tools_Available.md#user-content-msvc
	add_compile_options(/W4 /WX /external:anglebrackets /external:W0)

	# enable multi-processor compilation
	add_compile_options(/MP)
else()
	# lots of warnings and all warnings as errors
	add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

add_executable(
	EnumString
	"include/EnumString/enum_string.h"
	"src/EnumString/main.cpp"
)

target_include_directories(EnumString PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

# Set the IDE structure to match the folder structure
get_target_property(sources EnumString SOURCES)

source_group(
	TREE ${CMAKE_CURRENT_SOURCE_DIR}
	FILES ${sources}
	)

# Set the startup project for Visual Studio
if (MSVC)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT EnumString)
endif()

