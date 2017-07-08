
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include "interpret.h"
#include "ast.h"


// AST functions
void dlvm_lang_interpret(dlvm_lang_ast_node_t* ast) {
    switch (ast->kind) {
        case DLVM_LANG_AST_STATEMENTS: {
            arraylist_t* list = &ast->statements.children;

            for (int i = 0; i < list->count; i++) {
                dlvm_lang_ast_node_t* child;
                arraylist_get(list, i, &child);
                dlvm_lang_interpret_expression(child);
            }

            break;
        }

        default:
            assert(false);
            break;
    }
}

dlvm_lang_value_t dlvm_lang_interpret_expression(dlvm_lang_ast_node_t* expr) {
    if (expr->kind & DLVM_LANG_IS_BINARY) return dlvm_lang_interpret_binary(expr);
    if (expr->kind & DLVM_LANG_IS_UNARY) return dlvm_lang_interpret_unary(expr);

    switch (expr->kind) {
        case DLVM_LANG_AST_LITERAL_INT:
            return dlvm_lang_make_value_int(expr->int_lit.value);
        case DLVM_LANG_AST_LITERAL_FLOAT:
            return dlvm_lang_make_value_float(expr->float_lit.value);
        case DLVM_LANG_AST_LITERAL_STRING:
            return dlvm_lang_make_value_string(expr->string_lit.value);
        default:
            return dlvm_lang_make_value_undefined();
    }
}

dlvm_lang_value_t dlvm_lang_interpret_unary(dlvm_lang_ast_node_t* unary) {
    assert(unary->kind & DLVM_LANG_IS_UNARY);
    dlvm_lang_value_t child_value = dlvm_lang_interpret_expression(unary->unary.child);

    switch (unary->kind) {
        case DLVM_LANG_AST_UNARY_PRINT:
            dlvm_lang_print_value(child_value);
            return child_value;

        case DLVM_LANG_AST_UNARY_PAR:
            return child_value;

        default:
            assert(false);
            return dlvm_lang_make_value_undefined();
    }
}

static double to_double(dlvm_lang_value_t value) {
    assert(value.kind == DLVM_LANG_VALUE_FLOAT || value.kind == DLVM_LANG_VALUE_INT);
    return value.kind == DLVM_LANG_VALUE_FLOAT ? value.fvalue : (double)value.ivalue;
}

dlvm_lang_value_t dlvm_lang_interpret_binary(dlvm_lang_ast_node_t* binary) {
    assert(binary->kind & DLVM_LANG_IS_BINARY);
    dlvm_lang_value_t left_value = dlvm_lang_interpret_expression(binary->binary.left);
    dlvm_lang_value_t right_value = dlvm_lang_interpret_expression(binary->binary.right);

    if (left_value.kind == DLVM_LANG_VALUE_UNDEFINED)
        return left_value;
    if (right_value.kind == DLVM_LANG_VALUE_UNDEFINED)
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
                return dlvm_lang_make_value_undefined();
        }
    }
    
    double left_double = to_double(left_value);
    double right_double = to_double(right_value);

    switch (binary->kind) {
        case DLVM_LANG_AST_BINARY_ADD:
            return dlvm_lang_make_value_float(left_double + right_double);
        case DLVM_LANG_AST_BINARY_SUB:
            return dlvm_lang_make_value_float(left_double - right_double);
        case DLVM_LANG_AST_BINARY_MUL:
            return dlvm_lang_make_value_float(left_double * right_double);
        case DLVM_LANG_AST_BINARY_DIV:
            return dlvm_lang_make_value_float(left_double / right_double);
        default:
            assert(false);
            return dlvm_lang_make_value_undefined();
    }
}


// Value functions
dlvm_lang_value_t dlvm_lang_make_value_int(int64_t value) {
    return (dlvm_lang_value_t){ .kind = DLVM_LANG_VALUE_INT, .ivalue = value };
}

dlvm_lang_value_t dlvm_lang_make_value_float(double value) {
    return (dlvm_lang_value_t){ .kind = DLVM_LANG_VALUE_FLOAT, .fvalue = value };
}

dlvm_lang_value_t dlvm_lang_make_value_string(char* value) {
    return (dlvm_lang_value_t){ .kind = DLVM_LANG_VALUE_STRING, .svalue = value };
}

dlvm_lang_value_t dlvm_lang_make_value_undefined() {
    return (dlvm_lang_value_t){ .kind = DLVM_LANG_VALUE_UNDEFINED };
}

void dlvm_lang_print_value(dlvm_lang_value_t value) {
    switch (value.kind) {
        case DLVM_LANG_VALUE_FLOAT:
            printf("%f", value.fvalue);
            break;
        case DLVM_LANG_VALUE_INT:
            printf("%" PRId64, value.ivalue);
            break;
        case DLVM_LANG_VALUE_STRING:
            printf("%s", value.svalue);
            break;
        default:
            printf("UNDEFINED");
            break;
    }
}