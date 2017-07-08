#pragma once
#include "ast.h"

typedef struct dlvm_lang_scanner_s {
    dlvm_lang_position_t position;
    int64_t text_size;
    char* text;

    dlvm_lang_token_t peek;
} dlvm_lang_scanner_t;

dlvm_lang_scanner_t dlvm_lang_make_scanner(char* text);
char dlvm_lang_peek_char(dlvm_lang_scanner_t* scanner);
char dlvm_lang_eat_char(dlvm_lang_scanner_t* scanner);
void dlvm_lang_skip(dlvm_lang_scanner_t* scanner);
dlvm_lang_token_t dlvm_lang_eat_token(dlvm_lang_scanner_t* scanner);

dlvm_lang_ast_node_t* dlvm_lang_parse_statements(dlvm_lang_scanner_t* scanner);
dlvm_lang_ast_node_t* dlvm_lang_parse_expression(dlvm_lang_scanner_t* scanner);
dlvm_lang_ast_node_t* dlvm_lang_parse_term(dlvm_lang_scanner_t* scanner);