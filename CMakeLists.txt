cmake_minimum_required(VERSION 3.21)
project(Specter VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Автоматически находит все .cpp/.hpp файлы
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    src/*.cpp
    include/*.hpp
)

add_executable(Specter ${SOURCES})

# Подключаем заголовочные файлы
target_include_directories(Specter PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# Компиляторные флаги
if (MSVC)
    target_compile_options(Specter PRIVATE
        $<$<CONFIG:Debug>:/W3 /Zi>
        $<$<CONFIG:Release>:/O2 /Ob2 /DNDEBUG /MP>
        $<$<CONFIG:RelWithDebInfo>:/O2 /Ob1 /Zi /DNDEBUG /MP>
    )
endif()

# Если нужно отключить линкер-инкрементность и включить LTO:
target_link_options(Specter PRIVATE /INCREMENTAL:NO /LTCG)

target_compile_definitions(Specter PRIVATE WIN32_LEAN_AND_MEAN)
target_link_libraries(Specter PRIVATE kernel32.lib user32.lib)
