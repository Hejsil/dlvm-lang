
#include <stdlib.h>
#include <assert.h>
#include "ast.h"
#include "data_structures/arraylist.h"

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

dlvm_lang_ast_node_t* dlvm_lang_alloc_ast(dlvm_lang_position_t position, uint8_t ast_kind) {
    dlvm_lang_ast_node_t* result = malloc(sizeof(dlvm_lang_ast_node_t));
    dlvm_lang_init_ast(result, position, ast_kind);

    return result;
}

dlvm_lang_ast_node_t* dlvm_lang_alloc_ast_unary(dlvm_lang_position_t position, uint8_t ast_kind, dlvm_lang_ast_node_t* child) {
    dlvm_lang_ast_node_t* result = malloc(sizeof(dlvm_lang_ast_node_t));
    dlvm_lang_init_ast_unary(result, position, ast_kind, child);

    return result;
}

dlvm_lang_ast_node_t* dlvm_lang_alloc_ast_binary(dlvm_lang_position_t position, uint8_t ast_kind, dlvm_lang_ast_node_t* left, dlvm_lang_ast_node_t* right) {
    dlvm_lang_ast_node_t* result = malloc(sizeof(dlvm_lang_ast_node_t));
    dlvm_lang_init_ast_binary(result, position, ast_kind, left, right);

    return result;
}

dlvm_lang_ast_node_t* dlvm_lang_alloc_ast_int_lit(dlvm_lang_position_t position, int64_t value) {
    dlvm_lang_ast_node_t* result = malloc(sizeof(dlvm_lang_ast_node_t));
    dlvm_lang_init_ast_int_lit(result, position, value);

    return result;
}

dlvm_lang_ast_node_t* dlvm_lang_alloc_ast_float_lit(dlvm_lang_position_t position, double value) {
    dlvm_lang_ast_node_t* result = malloc(sizeof(dlvm_lang_ast_node_t));
    dlvm_lang_init_ast_float_lit(result, position, value);

    return result;
}

dlvm_lang_ast_node_t* dlvm_lang_alloc_ast_string_lit(dlvm_lang_position_t position, char* value) {
    dlvm_lang_ast_node_t* result = malloc(sizeof(dlvm_lang_ast_node_t));
    dlvm_lang_init_ast_string_lit(result, position, value);

    return result;
}

dlvm_lang_ast_node_t* dlvm_lang_alloc_ast_statements(dlvm_lang_position_t position) {
    dlvm_lang_ast_node_t* result = malloc(sizeof(dlvm_lang_ast_node_t));
    dlvm_lang_init_ast_statements(result, position);

    return result;
}

void dlvm_lang_init_ast(dlvm_lang_ast_node_t *ast, dlvm_lang_position_t position, uint8_t ast_kind) {
    ast->position = position;
    ast->kind = ast_kind;
}

void dlvm_lang_init_ast_unary(dlvm_lang_ast_node_t *unary, dlvm_lang_position_t position, uint8_t ast_kind,
                              dlvm_lang_ast_node_t *child) {
    // Ensure that we only send unary operator kinds to this function
    assert(ast_kind & DLVM_LANG_IS_UNARY);

    dlvm_lang_init_ast(unary, position, ast_kind);
    unary->unary.child = child;
}

void dlvm_lang_init_ast_binary(dlvm_lang_ast_node_t *binary, dlvm_lang_position_t position, uint8_t ast_kind,
                               dlvm_lang_ast_node_t *left, dlvm_lang_ast_node_t *right) {
    // Ensure that we only send binary operator kinds to this function
    assert(ast_kind & DLVM_LANG_IS_BINARY);

    dlvm_lang_init_ast(binary, position, ast_kind);
    binary->binary.left = left;
    binary->binary.right = right;
}

void dlvm_lang_init_ast_int_lit(dlvm_lang_ast_node_t *int_lit, dlvm_lang_position_t position, int64_t value) {
    dlvm_lang_init_ast(int_lit, position, DLVM_LANG_AST_LITERAL_INT);
    int_lit->int_lit.value = value;
}

void dlvm_lang_init_ast_float_lit(dlvm_lang_ast_node_t *float_lit, dlvm_lang_position_t position, double value) {
    dlvm_lang_init_ast(float_lit, position, DLVM_LANG_AST_LITERAL_FLOAT);
    float_lit->float_lit.value = value;
}

void dlvm_lang_init_ast_string_lit(dlvm_lang_ast_node_t *string_lit, dlvm_lang_position_t position, char *value) {
    dlvm_lang_init_ast(string_lit, position, DLVM_LANG_AST_LITERAL_STRING);
    string_lit->string_lit.value = value;
}

void dlvm_lang_init_ast_statements(dlvm_lang_ast_node_t *statements, dlvm_lang_position_t position) {
    dlvm_lang_init_ast(statements, position, DLVM_LANG_AST_STATEMENTS);
    arraylist_init(&statements->statements.children, sizeof(dlvm_lang_ast_node_t*), 2);
}


void dlvm_lang_dealloc_ast(dlvm_lang_ast_node_t* ast) {
    dlvm_lang_deinit_ast(ast);
    free(ast);
}

void dlvm_lang_deinit_ast(dlvm_lang_ast_node_t *ast) {
    if (ast->kind & DLVM_LANG_IS_BINARY) {
        dlvm_lang_dealloc_ast(ast->binary.left);
        dlvm_lang_dealloc_ast(ast->binary.right);
    } else if (ast->kind & DLVM_LANG_IS_UNARY) {
        dlvm_lang_dealloc_ast(ast->unary.child);
    } else {
        switch (ast->kind) {
            case DLVM_LANG_AST_LITERAL_STRING:
                free(ast->string_lit.value);
                break;
            case DLVM_LANG_AST_STATEMENTS: {
                arraylist_t* list = &ast->statements.children;

                for (int i = 0; i < list->count; i++) {
                    dlvm_lang_ast_node_t* child;
                    arraylist_get(list, i, &child);
                    dlvm_lang_dealloc_ast(child);
                }

                arraylist_deinit(list);
                break;
            }

            default:
                break;
        }
    }
}
