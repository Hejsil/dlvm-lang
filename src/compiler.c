#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "compiler.h"

#define NEWLINE '\n'

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

// Scanner functions
void dlvm_lang_init_scanner(dlvm_lang_scanner_t* scanner, char* text) {
    scanner->text_size = strlen(text);
    scanner->text = text;
    // We always start at line 1, for user readability
    scanner->position = dlvm_lang_make_position(0, 1, 0);

    // Eating will scan a new token as the peek, and return the old one.
    // We can therefor call it here, as it will initialize our peek token.
    dlvm_lang_eat_token(scanner);
}

char dlvm_lang_peek_char(dlvm_lang_scanner_t* scanner) {
    assert(scanner->position.index >= 0);

    if (scanner->position.index >= scanner->text_size) {
        return EOF;
    }

    return scanner->text[scanner->position.index];
}

char dlvm_lang_eat_char(dlvm_lang_scanner_t* scanner) {
    char result = dlvm_lang_peek_char(scanner);
    scanner->position.index++;

    if (result == NEWLINE)
    {
        scanner->position.line++;
        scanner->position.column = 0;
        return result;
    }

    scanner->position.column++;
    return result;
}

dlvm_lang_token_t dlvm_lang_eat_token(dlvm_lang_scanner_t* scanner) {
    dlvm_lang_token_t eaten = scanner->peek;

    // Store scanner->position locally, because scanner->position will change when we
    // eat chars
    dlvm_lang_position_t position = scanner->position;
    
    dlvm_lang_skip(scanner);

    char current = dlvm_lang_eat_char(scanner);

    if (isdigit(current)) {
        while (isdigit(dlvm_lang_peek_char(scanner)))
            dlvm_lang_eat_char(scanner);

        scanner->peek = dlvm_lang_make_token(DLVM_LANG_TOKEN_NUMBER, position);

        // atoi is smart and just parses numbers until it hits something else
        scanner->peek.ivalue = atoi(&scanner->text[position.index]);

        return eaten;
    }

    switch (current) {
        case '+':
            scanner->peek = dlvm_lang_make_token(DLVM_LANG_TOKEN_ADD, position);
            break;
        case '-':
            scanner->peek = dlvm_lang_make_token(DLVM_LANG_TOKEN_SUB, position);
            break;
        case '*':
            scanner->peek = dlvm_lang_make_token(DLVM_LANG_TOKEN_MUL, position);
            break;
        case '/':
            scanner->peek = dlvm_lang_make_token(DLVM_LANG_TOKEN_DIV, position);
            break;
        case '(':
            scanner->peek = dlvm_lang_make_token(DLVM_LANG_TOKEN_PAR_LEFT, position);
            break;
        case ')':
            scanner->peek = dlvm_lang_make_token(DLVM_LANG_TOKEN_PAR_RIGHT, position);
            break;
        default:
            scanner->peek = dlvm_lang_make_token(DLVM_LANG_TOKEN_UNKNOWN, position);
            break;
    }

    return eaten;
}

void dlvm_lang_skip(dlvm_lang_scanner_t* scanner) {
    while (isspace(dlvm_lang_peek_char(scanner)))
        dlvm_lang_eat_char(scanner);
}

// Parser Functions
void dlvm_lang_init_parser(dlvm_lang_parser_t* parser, dlvm_lang_scanner_t scanner) {
    parser->scanner = scanner;
}

dlvm_lang_ast_node_t* dlvm_lang_parse_expression(dlvm_lang_parser_t* parser) {
    dlvm_lang_ast_node_t* top = dlvm_lang_parse_term(parser);
    if (top == NULL) 
        return NULL;

    while (parser->scanner.peek.kind & IS_BINARY) {
        dlvm_lang_token_t op_token = dlvm_lang_eat_token(&parser->scanner);
        dlvm_lang_ast_node_t* right = dlvm_lang_parse_expression(parser);
        top = dlvm_lang_alloc_ast_binary(top->position, op_token.kind, top, right);
    }

    return top;
}

dlvm_lang_ast_node_t* dlvm_lang_parse_term(dlvm_lang_parser_t* parser) {
    dlvm_lang_token_t start_token = dlvm_lang_eat_token(&parser->scanner);

    switch (start_token.kind) {
        case DLVM_LANG_TOKEN_NUMBER:
            return dlvm_lang_alloc_ast_int_lit(start_token.position, start_token.ivalue);

        case DLVM_LANG_TOKEN_PAR_LEFT: {
            dlvm_lang_ast_node_t* expr = dlvm_lang_parse_expression(parser);
            if (expr == NULL) return NULL;

            dlvm_lang_token_t right_par = dlvm_lang_eat_token(&parser->scanner);
            if (right_par.kind != DLVM_LANG_TOKEN_PAR_RIGHT) {
                // TODO: error message
                return NULL;
            }

            return dlvm_lang_alloc_ast_unary(start_token.position, DLVM_LANG_AST_UNARY_PAR, expr);
        }
        default:
            // TODO: error message
            return NULL;
    }
}

// AST functions
dlvm_lang_ast_node_t* dlvm_lang_alloc_ast(dlvm_lang_position_t position, int8_t ast_kind) {
    dlvm_lang_ast_node_t* result = malloc(sizeof(dlvm_lang_ast_node_t));
    result->kind = ast_kind;
    result->position = position;

    return result;
}

dlvm_lang_ast_node_t* dlvm_lang_alloc_ast_unary(dlvm_lang_position_t position, int8_t ast_kind, dlvm_lang_ast_node_t* child) {
    // Ensure that we only send unary operator kinds to this function
    assert(ast_kind & IS_UNARY);

    dlvm_lang_ast_node_t* result = dlvm_lang_alloc_ast(position, ast_kind);
    result->unary.child = child;

    return result;
}

dlvm_lang_ast_node_t* dlvm_lang_alloc_ast_binary(dlvm_lang_position_t position, int8_t ast_kind, dlvm_lang_ast_node_t* left, dlvm_lang_ast_node_t* right) {
    // Ensure that we only send binary operator kinds to this function
    assert(ast_kind & IS_BINARY);

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

void dlvm_lang_dealloc_ast(dlvm_lang_ast_node_t* ast) {
    if (ast->kind & IS_BINARY) {
        dlvm_lang_dealloc_ast(ast->binary.left);
        dlvm_lang_dealloc_ast(ast->binary.right);
        free(ast);
    } else if (ast->kind & IS_UNARY) {
        dlvm_lang_dealloc_ast(ast->unary.child);
        free(ast);
    } else {
        free(ast);
    }
}

void dlvm_lang_print_ast(dlvm_lang_ast_node_t* ast, int64_t depth) {
    if (ast->kind & IS_BINARY) {
        dlvm_lang_print_ast_binary(ast, depth);
    } else if (ast->kind & IS_UNARY) {
        dlvm_lang_print_ast_unary(ast, depth);
    } else {
        switch (ast->kind) {
            case DLVM_LANG_AST_LITERAL_INT:
                dlvm_lang_print_ast_int_lit(ast, depth);
                break;
            default:
                dlvm_lang_print_ast_depth(depth);
                printf("<UNKNOWN\\>\n");
                break;
        }
    }
}

void dlvm_lang_print_ast_int_lit(dlvm_lang_ast_node_t* int_lit, int64_t depth) {
    assert(int_lit->kind == DLVM_LANG_AST_LITERAL_INT);
    dlvm_lang_print_ast_depth(depth);
    printf("<INT_LITERAL value=\"%" PRId64 "\"\\>\n", int_lit->int_lit.value);
}

void dlvm_lang_print_ast_binary(dlvm_lang_ast_node_t* binary, int64_t depth) {
    assert(binary->kind & IS_BINARY);

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
    assert(unary->kind & IS_UNARY);

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

dlvm_lang_value_t dlvm_lang_interpret(dlvm_lang_ast_node_t* ast) {
    if (ast->kind & IS_BINARY) {
        return dlvm_lang_interpret_binary(ast);
    } else {
        switch (ast->kind) {
            case DLVM_LANG_AST_LITERAL_INT:
                return dlvm_lang_make_value_int(ast->int_lit.value);        
        }
    }

    return dlvm_lang_make_value_fuck();
}

double to_double(dlvm_lang_value_t value) {
    assert(value.kind == DLVM_LANG_VALUE_FLOAT || value.kind == DLVM_LANG_VALUE_INT);
    return value.kind == DLVM_LANG_VALUE_FLOAT ? value.fvalue : (int64_t)value.ivalue;
}

dlvm_lang_value_t dlvm_lang_interpret_binary(dlvm_lang_ast_node_t* binary) {
    assert(binary->kind & IS_BINARY);
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
            return dlvm_lang_make_value_fuck();
    }

    assert(false);
    return dlvm_lang_make_value_fuck();
}


// Value functions
dlvm_lang_value_t dlvm_lang_make_value_int(int64_t value) {
    dlvm_lang_value_t result = dlvm_lang_make_value(DLVM_LANG_VALUE_INT);
    result.ivalue = value;

    return result;
}

dlvm_lang_value_t dlvm_lang_make_value_float(double value) {
    dlvm_lang_value_t result = dlvm_lang_make_value(DLVM_LANG_VALUE_FLOAT);
    result.fvalue = value;

    return result;
}

dlvm_lang_value_t dlvm_lang_make_value_fuck() {
    return dlvm_lang_make_value(DLVM_LANG_VALUE_FUCK);
}

dlvm_lang_value_t dlvm_lang_make_value(uint8_t kind) {
    dlvm_lang_value_t result;
    result.kind = kind;

    return result;
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