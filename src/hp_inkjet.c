#include <stdio.h>
#include <assert.h>
#include "hp_inkjet.h"

void dlvm_lang_print_ast(dlvm_lang_ast_node_t* ast, int64_t depth) {
    if (ast->kind & DLVM_LANG_IS_BINARY) {
        dlvm_lang_print_ast_binary(ast, depth);
    } else if (ast->kind & DLVM_LANG_IS_UNARY) {
        dlvm_lang_print_ast_unary(ast, depth);
    } else {
        dlvm_lang_print_ast_depth(depth);

        switch (ast->kind) {
            case DLVM_LANG_AST_LITERAL_INT:
                printf("<INT_LITERAL value=\"%" PRId64 "\"\\>\n", ast->int_lit.value);
                break;
            case DLVM_LANG_AST_LITERAL_FLOAT:
                printf("<FLOAT_LITERAL value=\"%f\"\\>\n", ast->float_lit.value);
                break;
            default:
                printf("<UNKNOWN\\>\n");
                break;
        }
    }
}

void dlvm_lang_print_ast_binary(dlvm_lang_ast_node_t* binary, int64_t depth) {
    assert(binary->kind & DLVM_LANG_IS_BINARY);

    uint8_t kind = binary->kind;
    char* name = 
        kind == DLVM_LANG_AST_BINARY_ADD ? "ADD" :
        kind == DLVM_LANG_AST_BINARY_SUB ? "SUB" : 
        kind == DLVM_LANG_AST_BINARY_MUL ? "MUL" : 
        kind == DLVM_LANG_AST_BINARY_DIV ? "DIV" : 
        "???";

    dlvm_lang_print_ast_depth(depth);
    printf("<%s>\n", name);
    depth += 2;

    dlvm_lang_print_ast(binary->binary.left, depth);
    dlvm_lang_print_ast(binary->binary.right, depth);

    depth -= 2;
    dlvm_lang_print_ast_depth(depth);
    printf("<%s\\>\n", name);
}

void dlvm_lang_print_ast_unary(dlvm_lang_ast_node_t* unary, int64_t depth) {
    assert(unary->kind & DLVM_LANG_IS_UNARY);

    uint8_t kind = unary->kind;
    char* name = 
        kind == DLVM_LANG_AST_UNARY_PAR ? "PAR" :
        "???";

    dlvm_lang_print_ast_depth(depth);
    printf("<%s>\n", name);
    depth += 2;

    dlvm_lang_print_ast(unary->unary.child, depth);

    depth -= 2;
    dlvm_lang_print_ast_depth(depth);
    printf("<%s\\>\n", name);
}

void dlvm_lang_print_ast_depth(int64_t depth) {
    for (int i = 0; i < depth; i++)
        printf(" ");
}