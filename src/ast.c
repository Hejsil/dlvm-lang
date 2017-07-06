
#include <stdlib.h>
#include <assert.h>
#include "ast.h"

dlvm_lang_position_t dlvm_lang_make_position(int64_t index, int64_t line, int64_t column) {
    dlvm_lang_position_t result;
    result.index = index;
    result.line = line;
    result.column = column;

    return result;
}

dlvm_lang_token_t dlvm_lang_make_token(uint8_t kind, dlvm_lang_position_t position) {
    dlvm_lang_token_t result;
    result.kind = kind;
    result.position = position;

    return result;
}

dlvm_lang_ast_node_t* dlvm_lang_alloc_ast(dlvm_lang_position_t position, int8_t ast_kind) {
    dlvm_lang_ast_node_t* result = malloc(sizeof(dlvm_lang_ast_node_t));
    result->kind = ast_kind;
    result->position = position;

    return result;
}

dlvm_lang_ast_node_t* dlvm_lang_alloc_ast_unary(dlvm_lang_position_t position, int8_t ast_kind, dlvm_lang_ast_node_t* child) {
    // Ensure that we only send unary operator kinds to this function
    assert(ast_kind & DLVM_LANG_IS_UNARY);

    dlvm_lang_ast_node_t* result = dlvm_lang_alloc_ast(position, ast_kind);
    result->unary.child = child;

    return result;
}

dlvm_lang_ast_node_t* dlvm_lang_alloc_ast_binary(dlvm_lang_position_t position, int8_t ast_kind, dlvm_lang_ast_node_t* left, dlvm_lang_ast_node_t* right) {
    // Ensure that we only send binary operator kinds to this function
    assert(ast_kind & DLVM_LANG_IS_BINARY);

    dlvm_lang_ast_node_t* result = dlvm_lang_alloc_ast(position, ast_kind);
    result->binary.left = left;
    result->binary.right = right;

    return result;
}

dlvm_lang_ast_node_t* dlvm_lang_alloc_ast_int_lit(dlvm_lang_position_t position, int64_t value) {
    dlvm_lang_ast_node_t* result = dlvm_lang_alloc_ast(position, DLVM_LANG_AST_LITERAL_INT);
    result->int_lit.value = value;

    return result;
}

dlvm_lang_ast_node_t* dlvm_lang_alloc_ast_float_lit(dlvm_lang_position_t position, double value) {
    dlvm_lang_ast_node_t* result = dlvm_lang_alloc_ast(position, DLVM_LANG_AST_LITERAL_FLOAT);
    result->float_lit.value = value;

    return result;
}

void dlvm_lang_dealloc_ast(dlvm_lang_ast_node_t* ast) {
    if (ast->kind & DLVM_LANG_IS_BINARY) {
        dlvm_lang_dealloc_ast(ast->binary.left);
        dlvm_lang_dealloc_ast(ast->binary.right);
        free(ast);
    } else if (ast->kind & DLVM_LANG_IS_UNARY) {
        dlvm_lang_dealloc_ast(ast->unary.child);
        free(ast);
    } else {
        free(ast);
    }
}