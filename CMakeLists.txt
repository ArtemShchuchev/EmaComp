cmake_minimum_required(VERSION 3.2)
project(EmaComp VERSION 1.0 LANGUAGES CXX)

set(OUTPUT_NAME EmaComp)
set(SOURCES main.cpp)

add_executable(${OUTPUT_NAME} ${SOURCES})


#[[ if (Visual Studio)
	настраиваю в качестве запускаемого проекта
	настройка на стандарт с++17 ]]
if(MSVC)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${OUTPUT_NAME})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
else()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
endif()
