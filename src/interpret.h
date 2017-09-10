#pragma once
#include <inttypes.h>
#include "ast.h"

typedef struct compiler_s {
    
} compiler_t;

typedef struct interpreter_s {

} interpreter_t;

typedef enum value_kind_e {
    DLVM_LANG_VALUE_INT,
    DLVM_LANG_VALUE_FLOAT,
    DLVM_LANG_VALUE_STRING,
    DLVM_LANG_VALUE_UNDEFINED
} value_kind_t;

typedef struct value_s {
    uint8_t kind;

    union {
        int64_t ivalue;
        double fvalue;
        char* svalue;
    };
} value_t;

// AST functions
void interpret_ast(ast_node_t *ast);
value_t interpret_expression(ast_node_t *expr);
value_t interpret_binary(ast_node_t *add);
value_t interpret_unary(ast_node_t *unary);

// Value functions
value_t make_value_int(int64_t value);
value_t make_value_float(double value);
value_t make_value_string(char *value);
value_t make_value_undefined();

void print_value(value_t value);