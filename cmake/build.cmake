set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/arc)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(FORCE_COLORED_OUTPUT "Force compiler diagnostic colors" OFF)
if(FORCE_COLORED_OUTPUT)
    add_compile_options($<$<CXX_COMPILER_ID:GNU>:-fdiagnostics-color=always>)
    add_compile_options($<$<CXX_COMPILER_ID:Clang>:-fcolor-diagnostics>)
endif()

add_compile_options(
    -Werror
    -Wall
    #-Wextra
    -Wpedantic
    -Wredundant-decls
    -Wnon-virtual-dtor
    -Wnull-dereference
    -Wzero-as-null-pointer-constant

    $<$<CXX_COMPILER_ID:GNU>:-Wsuggest-override>
    $<$<CXX_COMPILER_ID:GNU>:-Wduplicated-branches>
    $<$<CXX_COMPILER_ID:GNU>:-Wlogical-op>

    $<$<CXX_COMPILER_ID:Clang>:-Wno-gnu-zero-variadic-macro-arguments>
)
