#pragma once
#include <inttypes.h>
#include "ast.h"

typedef struct dlvm_lang_compiler_s {
    
} dlvm_lang_compiler_t;

typedef enum dlvm_lang_value_kind_e {
    DLVM_LANG_VALUE_INT,
    DLVM_LANG_VALUE_FLOAT,
    DLVM_LANG_VALUE_STRING,
    DLVM_LANG_VALUE_UNDEFINED
} dlvm_lang_value_kind_t;

typedef struct dlvm_lang_value_s {
    uint8_t kind;

    union {
        int64_t ivalue;
        double fvalue;
        char* svalue;
    };
} dlvm_lang_value_t;

// AST functions
void dlvm_lang_interpret(dlvm_lang_ast_node_t* ast);
dlvm_lang_value_t dlvm_lang_interpret_expression(dlvm_lang_ast_node_t* expr);
dlvm_lang_value_t dlvm_lang_interpret_binary(dlvm_lang_ast_node_t* add);
dlvm_lang_value_t dlvm_lang_interpret_unary(dlvm_lang_ast_node_t* unary);

// Value functions
dlvm_lang_value_t dlvm_lang_make_value_int(int64_t value);
dlvm_lang_value_t dlvm_lang_make_value_float(double value);
dlvm_lang_value_t dlvm_lang_make_value_string(char* value);
dlvm_lang_value_t dlvm_lang_make_value_undefined();

void dlvm_lang_print_value(dlvm_lang_value_t value);