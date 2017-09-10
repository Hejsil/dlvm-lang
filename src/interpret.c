
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include "interpret.h"
#include "ast.h"


// AST functions
void interpret_ast(ast_node_t *ast) {
    switch (ast->kind) {
        case AST_STATEMENTS: {
            arraylist_t* list = &ast->statements.children;

            for (int i = 0; i < list->count; i++) {
                ast_node_t* child = arraylist_get(list, ast_node_t*, i);
                interpret_expression(child);
            }

            break;
        }

        default:
            assert(false);
            break;
    }
}

value_t interpret_expression(ast_node_t *expr) {
    if (expr->kind & DLVM_LANG_IS_BINARY) return interpret_binary(expr);
    if (expr->kind & DLVM_LANG_IS_UNARY) return interpret_unary(expr);

    switch (expr->kind) {
        case AST_LITERAL_INT:
            return make_value_int(expr->int_lit.value);
        case AST_LITERAL_FLOAT:
            return make_value_float(expr->float_lit.value);
        case AST_LITERAL_STRING:
            return make_value_string(expr->string_lit.value);
        default:
            return make_value_undefined();
    }
}

value_t interpret_unary(ast_node_t *unary) {
    assert(unary->kind & DLVM_LANG_IS_UNARY);
    value_t child_value = interpret_expression(unary->unary.child);

    switch (unary->kind) {
        case AST_UNARY_PRINT:
            print_value(child_value);
            return child_value;

        case AST_UNARY_PAR:
            return child_value;

        default:
            assert(false);
            return make_value_undefined();
    }
}

static double to_double(value_t value) {
    assert(value.kind == DLVM_LANG_VALUE_FLOAT || value.kind == DLVM_LANG_VALUE_INT);
    return value.kind == DLVM_LANG_VALUE_FLOAT ? value.fvalue : (double)value.ivalue;
}

value_t interpret_binary(ast_node_t *binary) {
    assert(binary->kind & DLVM_LANG_IS_BINARY);
    value_t left_value = interpret_expression(binary->binary.left);
    value_t right_value = interpret_expression(binary->binary.right);

    if (left_value.kind == DLVM_LANG_VALUE_UNDEFINED)
        return left_value;
    if (right_value.kind == DLVM_LANG_VALUE_UNDEFINED)
        return right_value;
    if (left_value.kind == DLVM_LANG_VALUE_INT && right_value.kind == DLVM_LANG_VALUE_INT) {
        switch (binary->kind) {
            case AST_BINARY_ADD:
                return make_value_int(left_value.ivalue + right_value.ivalue);
            case AST_BINARY_SUB:
                return make_value_int(left_value.ivalue - right_value.ivalue);
            case AST_BINARY_MUL:
                return make_value_int(left_value.ivalue * right_value.ivalue);
            case AST_BINARY_DIV:
                return make_value_int(left_value.ivalue / right_value.ivalue);
            default:
                assert(false);
                return make_value_undefined();
        }
    }
    
    double left_double = to_double(left_value);
    double right_double = to_double(right_value);

    switch (binary->kind) {
        case AST_BINARY_ADD:
            return make_value_float(left_double + right_double);
        case AST_BINARY_SUB:
            return make_value_float(left_double - right_double);
        case AST_BINARY_MUL:
            return make_value_float(left_double * right_double);
        case AST_BINARY_DIV:
            return make_value_float(left_double / right_double);
        default:
            assert(false);
            return make_value_undefined();
    }
}


// Value functions
value_t make_value_int(int64_t value) {
    return (value_t){ .kind = DLVM_LANG_VALUE_INT, .ivalue = value };
}

value_t make_value_float(double value) {
    return (value_t){ .kind = DLVM_LANG_VALUE_FLOAT, .fvalue = value };
}

value_t make_value_string(char *value) {
    return (value_t){ .kind = DLVM_LANG_VALUE_STRING, .svalue = value };
}

value_t make_value_undefined() {
    return (value_t){ .kind = DLVM_LANG_VALUE_UNDEFINED };
}

void print_value(value_t value) {
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