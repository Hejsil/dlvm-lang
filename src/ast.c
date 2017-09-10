
#include <stdlib.h>
#include <assert.h>
#include "ast.h"
#include "data_structures/arraylist.h"

string_position_t make_position(int64_t index, int64_t line, int64_t column) {
    string_position_t result;
    result.index = index;
    result.line = line;
    result.column = column;

    return result;
}

token_t make_token(uint8_t kind, string_position_t position) {
    token_t result;
    result.kind = kind;
    result.position = position;

    return result;
}

ast_node_t* alloc_ast(string_position_t position, uint8_t ast_kind) {
    ast_node_t* result = malloc(sizeof(ast_node_t));
    init_ast(result, position, ast_kind);

    return result;
}

ast_node_t* alloc_ast_unary(string_position_t position, uint8_t ast_kind, ast_node_t *child) {
    ast_node_t* result = malloc(sizeof(ast_node_t));
    init_ast_unary(result, position, ast_kind, child);

    return result;
}

ast_node_t* alloc_ast_binary(string_position_t position, uint8_t ast_kind, ast_node_t *left, ast_node_t *right) {
    ast_node_t* result = malloc(sizeof(ast_node_t));
    init_ast_binary(result, position, ast_kind, left, right);

    return result;
}

ast_node_t* alloc_ast_int_lit(string_position_t position, int64_t value) {
    ast_node_t* result = malloc(sizeof(ast_node_t));
    init_ast_int_lit(result, position, value);

    return result;
}

ast_node_t* alloc_ast_float_lit(string_position_t position, double value) {
    ast_node_t* result = malloc(sizeof(ast_node_t));
    init_ast_float_lit(result, position, value);

    return result;
}

ast_node_t* alloc_ast_string_lit(string_position_t position, char *value) {
    ast_node_t* result = malloc(sizeof(ast_node_t));
    init_ast_string_lit(result, position, value);

    return result;
}

ast_node_t* alloc_ast_statements(string_position_t position) {
    ast_node_t* result = malloc(sizeof(ast_node_t));
    init_ast_statements(result, position);

    return result;
}

ast_node_t* alloc_ast_decl(string_position_t position, char *name, ast_node_t *value) {
    ast_node_t* result = malloc(sizeof(ast_node_t));
    init_ast_decl(result, position, name, value);

    return result;
}

ast_node_t* alloc_ast_symbol(string_position_t position, char *name) {
    ast_node_t* result = malloc(sizeof(ast_node_t));
    init_ast_symbol(result, position, name);

    return result;
}





void init_ast(ast_node_t *ast, string_position_t position, uint8_t ast_kind) {
    ast->position = position;
    ast->kind = ast_kind;
}

void init_ast_unary(ast_node_t *unary, string_position_t position, uint8_t ast_kind,
                    ast_node_t *child) {
    // Ensure that we only send unary operator kinds to this function
    assert(ast_kind & DLVM_LANG_IS_UNARY);

    init_ast(unary, position, ast_kind);
    unary->unary.child = child;
}

void init_ast_binary(ast_node_t *binary, string_position_t position, uint8_t ast_kind, ast_node_t *left,
                     ast_node_t *right) {
    // Ensure that we only send binary operator kinds to this function
    assert(ast_kind & DLVM_LANG_IS_BINARY);

    init_ast(binary, position, ast_kind);
    binary->binary.left = left;
    binary->binary.right = right;
}

void init_ast_int_lit(ast_node_t *int_lit, string_position_t position, int64_t value) {
    init_ast(int_lit, position, AST_LITERAL_INT);
    int_lit->int_lit.value = value;
}

void init_ast_float_lit(ast_node_t *float_lit, string_position_t position, double value) {
    init_ast(float_lit, position, AST_LITERAL_FLOAT);
    float_lit->float_lit.value = value;
}

void init_ast_string_lit(ast_node_t *string_lit, string_position_t position, char *value) {
    init_ast(string_lit, position, AST_LITERAL_STRING);
    string_lit->string_lit.value = value;
}

void init_ast_statements(ast_node_t *statements, string_position_t position) {
    init_ast(statements, position, AST_STATEMENTS);
    arraylist_unsafe_init(&statements->statements.children, sizeof(ast_node_t *), 2);
}

void init_ast_decl(ast_node_t *decl, string_position_t position, char *name, ast_node_t *value) {
    init_ast(decl, position, AST_DECL);
    decl->decl.name = name;
    decl->decl.value = value;
}

void init_ast_symbol(ast_node_t *decl, string_position_t position, char *name) {
    init_ast(decl, position, AST_SYMBOL);
    decl->decl.name = name;
}





void dealloc_ast(ast_node_t *ast) {
    deinit_ast(ast);
    free(ast);
}

void deinit_ast(ast_node_t *ast) {
    if (ast->kind & DLVM_LANG_IS_BINARY) {
        dealloc_ast(ast->binary.left);
        dealloc_ast(ast->binary.right);
    } else if (ast->kind & DLVM_LANG_IS_UNARY) {
        dealloc_ast(ast->unary.child);
    } else {
        switch (ast->kind) {
            case AST_SYMBOL:
                free(ast->symbol.name);
                break;
            case AST_LITERAL_STRING:
                free(ast->string_lit.value);
                break;
            case AST_DECL:
                dealloc_ast(ast->decl.value);
                free(ast->decl.value);
                break;
            case AST_STATEMENTS: {
                arraylist_t* list = &ast->statements.children;

                for (int i = 0; i < list->count; i++) {
                    ast_node_t* child = arraylist_get(list, ast_node_t*, i);
                    dealloc_ast(child);
                }

                arraylist_deinit(list);
                break;
            }

            default:
                break;
        }
    }
}
