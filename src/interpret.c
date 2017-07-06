#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include "interpret.h"


// AST functions
dlvm_lang_value_t dlvm_lang_interpret(dlvm_lang_ast_node_t* ast) {
    dlvm_lang_value_t result;
    if (ast->kind & DLVM_LANG_IS_BINARY) {
        result = dlvm_lang_interpret_binary(ast);
        return result;
    } else {
        switch (ast->kind) {
            case DLVM_LANG_AST_LITERAL_INT:
                return dlvm_lang_make_value_int(ast->int_lit.value);     
            case DLVM_LANG_AST_LITERAL_FLOAT:
                return dlvm_lang_make_value_float(ast->float_lit.value);       
        }
    }

    return dlvm_lang_make_value_fuck();
}

static double to_double(dlvm_lang_value_t value) {
    assert(value.kind == DLVM_LANG_VALUE_FLOAT || value.kind == DLVM_LANG_VALUE_INT);
    return value.kind == DLVM_LANG_VALUE_FLOAT ? value.fvalue : (double)value.ivalue;
}

dlvm_lang_value_t dlvm_lang_interpret_binary(dlvm_lang_ast_node_t* binary) {
    assert(binary->kind & DLVM_LANG_IS_BINARY);
    dlvm_lang_value_t left_value = dlvm_lang_interpret(binary->binary.left);
    dlvm_lang_value_t right_value = dlvm_lang_interpret(binary->binary.right);

    if (left_value.kind == DLVM_LANG_VALUE_FUCK)
        return left_value;
    if (right_value.kind == DLVM_LANG_VALUE_FUCK)
        return right_value;
    if (left_value.kind == DLVM_LANG_VALUE_INT && right_value.kind == DLVM_LANG_VALUE_INT) {
        switch (binary->kind) {
            case DLVM_LANG_AST_BINARY_ADD:
                return dlvm_lang_make_value_int(left_value.ivalue + right_value.ivalue);
            case DLVM_LANG_AST_BINARY_SUB:
                return dlvm_lang_make_value_int(left_value.ivalue - right_value.ivalue);
            case DLVM_LANG_AST_BINARY_MUL:
                return dlvm_lang_make_value_int(left_value.ivalue * right_value.ivalue);
            case DLVM_LANG_AST_BINARY_DIV:
                return dlvm_lang_make_value_int(left_value.ivalue / right_value.ivalue);
            default:
                assert(false);
                return dlvm_lang_make_value_fuck();
        }
    }
    
    double left_double = to_double(left_value);
    double right_double = to_double(right_value);
    dlvm_lang_value_t result;

    switch (binary->kind) {
        case DLVM_LANG_AST_BINARY_ADD:
            result = dlvm_lang_make_value_float(left_double + right_double);
            return result;
        case DLVM_LANG_AST_BINARY_SUB:
            result = dlvm_lang_make_value_float(left_double - right_double);
            return result;
        case DLVM_LANG_AST_BINARY_MUL:
            result = dlvm_lang_make_value_float(left_double * right_double);
            return result;
        case DLVM_LANG_AST_BINARY_DIV:
            result = dlvm_lang_make_value_float(left_double / right_double);
            return result;
        default:
            assert(false);
            return dlvm_lang_make_value_fuck();
    }

    assert(false);
    return dlvm_lang_make_value_fuck();
}


// Value functions
dlvm_lang_value_t dlvm_lang_make_value_int(int64_t value) {
    return (dlvm_lang_value_t){ .kind = DLVM_LANG_VALUE_INT, .ivalue = value };
}

dlvm_lang_value_t dlvm_lang_make_value_float(double value) {
    dlvm_lang_value_t result = (dlvm_lang_value_t){ .kind = DLVM_LANG_VALUE_FLOAT, .fvalue = value };
    return result;
}

dlvm_lang_value_t dlvm_lang_make_value_fuck() {
    return (dlvm_lang_value_t){ .kind = DLVM_LANG_VALUE_FUCK };
}

void dlvm_lang_print_value(dlvm_lang_value_t value) {
    switch (value.kind) {
        case DLVM_LANG_VALUE_FLOAT:
            printf("%f", value.fvalue);
            break;
        case DLVM_LANG_VALUE_INT:
            printf("%" PRId64, value.ivalue);
            break;
        default:
            printf("FUCK");
            break;
    }
}