#include <stdio.h>
#include "c_emitter.h"

void dlvm_lang_compile_c(char* path) {
}

void dlvm_lang_emit_c(char* path, dlvm_lang_ast_node_t* ast) {
    FILE* file = fopen(path, "w");

    fputs("", file);

    fclose(file);
}