#include <stdio.h>
#include "dlvm_emitter.h"

void dlvm_lang_emit_dlvm(char* path, dlvm_lang_ast_node_t* ast) {
    FILE* file = fopen(path, "w");

    fputs("", file);

    fclose(file);
}