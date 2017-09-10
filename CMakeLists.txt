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
    src/ast.c
    src/ast.h
    src/hp_inkjet.c
    src/hp_inkjet.h
    src/interpret.c
    src/interpret.h
    src/parsing.c
    src/parsing.h
    main.c
    src/data_structures/arraylist.c
    src/data_structures/arraylist.h
    src/data_structures/map.c
    src/data_structures/map.h src/data_structures/array.c src/data_structures/array.h src/data_structures/basic.c src/data_structures/basic.h)

add_executable(dlvm_lang ${SOURCE_FILES})