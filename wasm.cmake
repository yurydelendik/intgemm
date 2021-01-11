set(CMAKE_SYSTEM_NAME wasm32)
set(CMAKE_SYSTEM_PROCESSOR wasm32)

set(CMAKE_SYSROOT /Users/yury/bin/wasi-sdk/share/wasi-sysroot)
#set(CMAKE_STAGING_PREFIX /Users/yury/stage)

set(tools /Users/yury/bin/wasi-sdk)
set(CMAKE_C_COMPILER ${tools}/bin/clang)
set(CMAKE_CXX_COMPILER ${tools}/bin/clang++)
set(CMAKE_CXX_FLAGS "-fno-exceptions -m32 -msimd128")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)