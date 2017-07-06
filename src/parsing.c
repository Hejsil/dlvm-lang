#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include "parsing.h"

#define EOT '\0'
#define NEWLINE '\n'

// Scanner functions
dlvm_lang_scanner_t dlvm_lang_make_scanner(char* text) {
    dlvm_lang_scanner_t result = (dlvm_lang_scanner_t){
        .text_size = strlen(text),
        .text = text,
        .position = dlvm_lang_make_position(0, 1, 0)
    };

    // Eating will scan a new token as the peek, and return the old one.
    // We can therefor call it here, as it will initialize our peek token.
    dlvm_lang_eat_token(&result);

    return result;
}

char dlvm_lang_peek_char(dlvm_lang_scanner_t* scanner) {
    assert(scanner->position.index >= 0);

    if (scanner->position.index >= scanner->text_size) {
        return EOT;
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

        // If no dot, then we are scanning a normal number
        if (dlvm_lang_peek_char(scanner) != '.') {
            scanner->peek = dlvm_lang_make_token(DLVM_LANG_TOKEN_INT, position);
            scanner->peek.ivalue = atoi(&scanner->text[position.index]);
            return eaten;
        }

        dlvm_lang_eat_char(scanner);

        // If there is no digit just after the dot, then we don't know which token this is
        if (!isdigit(dlvm_lang_peek_char(scanner))) {
            scanner->peek = dlvm_lang_make_token(DLVM_LANG_TOKEN_UNKNOWN, position);
            return eaten;
        }
        
        while (isdigit(dlvm_lang_peek_char(scanner)))
            dlvm_lang_eat_char(scanner);
        
        scanner->peek = dlvm_lang_make_token(DLVM_LANG_TOKEN_FLOAT, position);
        scanner->peek.fvalue = atof(&scanner->text[position.index]);
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

// scanner Functions
dlvm_lang_ast_node_t* dlvm_lang_parse_expression(dlvm_lang_scanner_t* scanner) {
    dlvm_lang_ast_node_t* top = dlvm_lang_parse_term(scanner);
    if (top == NULL) 
        return NULL;

    while (scanner->peek.kind & DLVM_LANG_IS_BINARY) {
        dlvm_lang_token_t op_token = dlvm_lang_eat_token(scanner);
        dlvm_lang_ast_node_t* right = dlvm_lang_parse_expression(scanner);
        top = dlvm_lang_alloc_ast_binary(top->position, op_token.kind, top, right);
    }

    return top;
}

dlvm_lang_ast_node_t* dlvm_lang_parse_term(dlvm_lang_scanner_t* scanner) {
    dlvm_lang_token_t start_token = dlvm_lang_eat_token(scanner);

    switch (start_token.kind) {
        case DLVM_LANG_TOKEN_INT:
            return dlvm_lang_alloc_ast_int_lit(start_token.position, start_token.ivalue);
        case DLVM_LANG_TOKEN_FLOAT:
            return dlvm_lang_alloc_ast_float_lit(start_token.position, start_token.fvalue);

        case DLVM_LANG_TOKEN_PAR_LEFT: {
            dlvm_lang_ast_node_t* expr = dlvm_lang_parse_expression(scanner);
            if (expr == NULL) return NULL;

            dlvm_lang_token_t right_par = dlvm_lang_eat_token(scanner);
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