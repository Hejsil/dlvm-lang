cmake_minimum_required(VERSION 3.8)
project(dlvm_lang)

set(CMAKE_CXX_STANDARD 11)

set(
    SOURCE_FILES
    libs/dlvm/src/opcodes.h
    libs/dlvm/src/types.c
    libs/dlvm/src/types.h
    libs/dlvm/src/vm.c
    libs/dlvm/src/vm.h
    src/backends/c_emitter.c
    src/backends/c_emitter.h
    src/backends/dlvm_emitter.c
    src/backends/dlvm_emitter.h
    src/ast.c
    src/ast.h
    src/hp_inkjet.c
    src/hp_inkjet.h
    src/interpret.c
    src/interpret.h
    src/parsing.c
    src/parsing.h
    main.c
)

add_executable(dlvm_lang ${SOURCE_FILES})