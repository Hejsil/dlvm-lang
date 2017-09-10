#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include "parsing.h"


static const char end_of_string = '\0';
static const char new_line = '\n';

// Scanner functions
scanner_t make_scanner(char *text) {
    scanner_t result = (scanner_t){
        .text_size = strlen(text),
        .text = text,
        .position = make_position(0, 1, 0)
    };

    // Eating will scan a new token as the peek, and return the old one.
    // We can therefor call it here, as it will initialize our peek token.
    eat_token(&result);

    return result;
}

char peek_char(scanner_t *scanner) {
    assert(scanner->position.index >= 0);

    if (scanner->position.index >= scanner->text_size) return end_of_string;
    return scanner->text[scanner->position.index];
}

char eat_char(scanner_t *scanner) {
    char result = peek_char(scanner);
    scanner->position.index++;

    if (result == new_line) {
        scanner->position.line++;
        scanner->position.column = 0;
        return result;
    }

    scanner->position.column++;
    return result;
}

token_t eat_token(scanner_t *scanner) {
    static const char var_keyword[] = "var";
    static const char print_keyword[] = "print";
    static const char* keywords[] = {
        var_keyword,
        print_keyword
    };
    static const token_kind_t keyword_tokens[] = {
        TOKEN_KEYWORD_VAR,
        TOKEN_KEYWORD_PRINT
    };
    static_assert(sizeof(keywords) == sizeof(keyword_tokens), "each keyword, must have a corrisponding token.");

    token_t eaten = scanner->peek;

    skip_whitespace(scanner);

    // Store scanner->position locally, because scanner->position will change when we
    // eat chars
    string_position_t start = scanner->position;
    char current = eat_char(scanner);

    if (isdigit(current)) {
        while (isdigit(peek_char(scanner)))
            eat_char(scanner);

        // If no dot, then we are scanning a normal number
        if (peek_char(scanner) != '.') {
            scanner->peek = make_token(TOKEN_INT, start);
            scanner->peek.ivalue = strtoll(&scanner->text[start.index], NULL, 10);
            return eaten;
        }

        eat_char(scanner);

        // If there is no digit just after the dot, then we don't know which token this is
        if (!isdigit(peek_char(scanner))) {
            scanner->peek = make_token(TOKEN_UNKNOWN, start);
            return eaten;
        }
        
        while (isdigit(peek_char(scanner)))
            eat_char(scanner);
        
        scanner->peek = make_token(TOKEN_FLOAT, start);
        scanner->peek.fvalue = strtod(&scanner->text[start.index], NULL);
        return eaten;
    }

    if (isalpha(current)) {
        while (isalpha(peek_char(scanner)))
            eat_char(scanner);

        for (int i = 0; i < sizeof(keywords) / sizeof(char*); i++) {
            const char* keyword = keywords[i];
            if (strncmp(&scanner->text[start.index], keyword, strlen(keyword)) == 0) {
                scanner->peek = make_token(keyword_tokens[i], start);
                return eaten;
            }
        }

        int64_t length = scanner->position.index - start.index;
        char* ident_value = malloc(length + 1);

        memcpy(ident_value, &scanner->text[start.index], length);
        ident_value[length] = '\0';

        scanner->peek = make_token(TOKEN_IDENTIFIER, start);
        scanner->peek.svalue = ident_value;
        return eaten;
    }

    switch (current) {
        case '=':
            scanner->peek = make_token(TOKEN_EQUAL, start);
            break;

        case '+':
            scanner->peek = make_token(TOKEN_ADD, start);
            break;

        case '-':
            scanner->peek = make_token(TOKEN_SUB, start);
            break;

        case '*':
            scanner->peek = make_token(TOKEN_MUL, start);
            break;

        case '/':
            scanner->peek = make_token(TOKEN_DIV, start);
            break;

        case '(':
            scanner->peek = make_token(TOKEN_PAR_LEFT, start);
            break;

        case ')':
            scanner->peek = make_token(TOKEN_PAR_RIGHT, start);
            break;

        case '"':
            while (peek_char(scanner) != '"')
                eat_char(scanner);

            // We found the last '"'. We need to eat that char to
            eat_char(scanner);

            scanner->peek = make_token(TOKEN_STRING, start);

            // We need to alloc for all chars between the '"', and one extra byte for the '\0'
            size_t size = (size_t) (scanner->position.index - (start.index + 1));
            scanner->peek.svalue = malloc(size);
            strncpy(scanner->peek.svalue, &scanner->text[start.index + 1], size - 1);
            scanner->peek.svalue[size - 1] = '\0';
            break;

        case '\0':
            scanner->peek = make_token(TOKEN_EOF, start);
            break;

        default:
            scanner->peek = make_token(TOKEN_UNKNOWN, start);
            break;
    }

    return eaten;
}

void skip_whitespace(scanner_t *scanner) {
    while (isspace(peek_char(scanner)))
        eat_char(scanner);
}

ast_node_t *parse_statements(scanner_t *scanner) {
    ast_node_t* result = alloc_ast_statements(scanner->peek.position);

    while (scanner->peek.kind != TOKEN_EOF) {
        ast_node_t* child;

        switch (scanner->peek.kind) {
            case TOKEN_KEYWORD_VAR:
                child = parse_decl(scanner);
                break;

            default:
                child = parse_expression(scanner);
                break;
        }

        if (child == NULL) {
            dealloc_ast(result);
            return NULL;
        }

        arraylist_add_last(&result->statements.children, child);
    }

    return result;
}

ast_node_t* parse_decl(scanner_t *scanner) {
    token_t var = eat_token(scanner);
    if (var.kind != TOKEN_KEYWORD_VAR) {
        // TODO: error message
        return NULL;
    }

    token_t name = eat_token(scanner);
    if (name.kind != TOKEN_IDENTIFIER) {
        // TODO: error message
        return NULL;
    }

    token_t equals = eat_token(scanner);
    if (equals.kind != TOKEN_EQUAL) {
        // TODO: error message
        return NULL;
    }

    ast_node_t* value = parse_expression(scanner);
    if (value == NULL) return NULL;

    return alloc_ast_decl(var.position, name.svalue, value);
}

ast_node_t* parse_expression(scanner_t *scanner) {
    ast_node_t* top = parse_term(scanner);
    if (top == NULL) return NULL;

    while (scanner->peek.kind & DLVM_LANG_IS_BINARY) {
        token_t op_token = eat_token(scanner);
        ast_node_t* right = parse_expression(scanner);
        top = alloc_ast_binary(top->position, op_token.kind, top, right);
    }

    return top;
}

ast_node_t* parse_term(scanner_t *scanner) {
    token_t start_token = eat_token(scanner);

    switch (start_token.kind) {
        case TOKEN_INT:
            return alloc_ast_int_lit(start_token.position, start_token.ivalue);

        case TOKEN_FLOAT:
            return alloc_ast_float_lit(start_token.position, start_token.fvalue);

        case TOKEN_STRING:
            return alloc_ast_string_lit(start_token.position, start_token.svalue);

        case TOKEN_IDENTIFIER:
            return alloc_ast_symbol(start_token.position, start_token.svalue);

        case TOKEN_KEYWORD_PRINT: {
            ast_node_t* expr = parse_expression(scanner);
            if (expr == NULL) return NULL;

            return alloc_ast_unary(start_token.position, start_token.kind, expr);
        }

        case TOKEN_PAR_LEFT: {
            ast_node_t* expr = parse_expression(scanner);
            if (expr == NULL) return NULL;

            token_t right_par = eat_token(scanner);
            if (right_par.kind != TOKEN_PAR_RIGHT) {
                // TODO: error message
                return NULL;
            }

            return alloc_ast_unary(start_token.position, AST_UNARY_PAR, expr);
        }
        default:
            // TODO: error message
            return NULL;
    }
}
