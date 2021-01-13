set(CMAKE_SYSTEM_NAME wasm32-wasi)
set(CMAKE_SYSTEM_PROCESSOR wasm32)

if( DEFINED ENV{WASI_PREFIX} )
    set(tools $ENV{WASI_PREFIX})
else()
    message( SEND_ERROR "Undefined WASI_PREFIX." )
endif()

set(CMAKE_SYSROOT "${tools}/share/wasi-sysroot")
set(CMAKE_EXECUTABLE_SUFFIX_CXX ".wasm")

set(CMAKE_C_COMPILER ${tools}/bin/clang)
set(CMAKE_CXX_COMPILER ${tools}/bin/clang++)
set(CMAKE_CXX_FLAGS "-fno-exceptions -m32 -msimd128")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)