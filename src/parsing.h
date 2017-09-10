#pragma once
#include "ast.h"

typedef struct scanner_s {
    string_position_t position;
    int64_t text_size;
    char* text;

    token_t peek;
} scanner_t;

scanner_t make_scanner(char *text);
char peek_char(scanner_t *scanner);
char eat_char(scanner_t *scanner);
void skip_whitespace(scanner_t *scanner);
token_t eat_token(scanner_t *scanner);

ast_node_t* parse_statements(scanner_t *scanner);
ast_node_t* parse_decl(scanner_t *scanner);
ast_node_t* parse_expression(scanner_t *scanner);
ast_node_t* parse_term(scanner_t *scanner);