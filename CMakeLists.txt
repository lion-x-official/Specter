cmake_minimum_required(VERSION 3.21)

project(Specter VERSION 1.0 LANGUAGES CXX)

# Устанавливаем стандарт C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Включаем поддержку Edit and Continue в Debug/RelWithDebInfo
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Добавляем include директорию
include_directories(${CMAKE_SOURCE_DIR}/src/headers)

file(GLOB PROJECT_SOURCES
  "src/*.cpp"
  "src/headers/*.h"
)

# Создаём исполняемый файл
add_executable(Specter ${PROJECT_SOURCES})

# Оптимизации и многопоточность для MSVC
if (MSVC)
  target_compile_options(Specter PRIVATE
    $<$<CONFIG:Debug>:/W3 /Zi>
    $<$<CONFIG:Release>:/O2 /Ob2 /DNDEBUG /MP>
    $<$<CONFIG:RelWithDebInfo>:/O2 /Ob1 /Zi /DNDEBUG /MP>
  )
else()
  target_compile_options(Specter PRIVATE
    $<$<CONFIG:Release>:-O3>
    $<$<CONFIG:RelWithDebInfo>:-O2 -g>
  )
endif()

target_compile_definitions(Specter PRIVATE WIN32_LEAN_AND_MEAN)

# Добавляем зависимости
target_link_libraries(Specter kernel32.lib user32.lib)

