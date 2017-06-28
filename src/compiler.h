#pragma once
#include <inttypes.h>
#include <stdbool.h>

#define IS_BINARY 0x80
#define IS_UNARY 0x40

typedef struct dlvm_lang_position_s {
    int64_t index, line, column;
} dlvm_lang_position_t;

typedef enum dlvm_lang_token_kind_e {
    DLVM_LANG_TOKEN_NUMBER    = 0x00,
    DLVM_LANG_TOKEN_PAR_LEFT  = 0x01,
    DLVM_LANG_TOKEN_PAR_RIGHT = 0x02,
    DLVM_LANG_TOKEN_ADD       = 0x00 | IS_BINARY,
    DLVM_LANG_TOKEN_SUB       = 0x01 | IS_BINARY,
    DLVM_LANG_TOKEN_UNKNOWN   = 0xFF & ~IS_BINARY & ~IS_UNARY,
} dlvm_lang_token_kind_t;

typedef struct dlvm_lang_token_s {
    uint8_t kind;
    dlvm_lang_position_t position;

    union {
        int64_t ivalue;
        double dvalue;
        char* svalue;
    };
} dlvm_lang_token_t;

typedef struct dlvm_lang_scanner_s {
    dlvm_lang_position_t position;
    int64_t text_size;
    char* text;

    dlvm_lang_token_t peek;
} dlvm_lang_scanner_t;

typedef enum dlvm_lang_ast_kind_e {
    DLVM_LANG_AST_LITERAL_INT = 0x00,
    DLVM_LANG_AST_BINARY_ADD  = DLVM_LANG_TOKEN_ADD,
    DLVM_LANG_AST_BINARY_SUB  = DLVM_LANG_TOKEN_SUB,
    DLVM_LANG_AST_UNARY_PAR   = 0x00 | IS_UNARY
} dlvm_lang_ast_kind_t;

typedef struct dlvm_lang_ast_node_s {
    uint8_t kind;
    dlvm_lang_position_t position;

    union {
        struct {
            struct dlvm_lang_ast_node_s* left;
            struct dlvm_lang_ast_node_s* right;
        } binary;

        struct {
            struct dlvm_lang_ast_node_s* child;
        } unary;

        struct {
            int64_t value;
        } int_lit;
    };
} dlvm_lang_ast_node_t;

typedef struct dlvm_lang_parser_s {
    dlvm_lang_scanner_t scanner;
} dlvm_lang_parser_t;

typedef struct dlvm_lang_compiler_s {
    
} dlvm_lang_compiler_t;

typedef enum dlvm_lang_value_kind_e {
    DLVM_LANG_VALUE_INT,
    DLVM_LANG_VALUE_FLOAT,
    DLVM_LANG_VALUE_FUCK
} dlvm_lang_value_kind_t;

typedef struct dlvm_lang_value_s {
    uint8_t kind;

    union {
        int64_t ivalue;
        double fvalue;
    };
} dlvm_lang_value_t;

dlvm_lang_position_t dlvm_lang_make_position(int64_t index, int64_t line, int64_t column);
dlvm_lang_token_t dlvm_lang_make_token(uint8_t kind, dlvm_lang_position_t position);

// Scanner functions
void dlvm_lang_init_scanner(dlvm_lang_scanner_t* scanner, char* text);
char dlvm_lang_peek_char(dlvm_lang_scanner_t* scanner);
char dlvm_lang_eat_char(dlvm_lang_scanner_t* scanner);
void dlvm_lang_skip(dlvm_lang_scanner_t* scanner);
dlvm_lang_token_t dlvm_lang_eat_token(dlvm_lang_scanner_t* scanner);

// Parser functions
void dlvm_lang_init_parser(dlvm_lang_parser_t* parser, dlvm_lang_scanner_t scanner);
dlvm_lang_ast_node_t* dlvm_lang_parse_expression(dlvm_lang_parser_t* parser);
dlvm_lang_ast_node_t* dlvm_lang_parse_term(dlvm_lang_parser_t* parser);

// AST functions
dlvm_lang_ast_node_t* dlvm_lang_alloc_ast(dlvm_lang_position_t position, int8_t ast_kind);
dlvm_lang_ast_node_t* dlvm_lang_alloc_ast_unary(dlvm_lang_position_t position, int8_t ast_kind, dlvm_lang_ast_node_t* child);
dlvm_lang_ast_node_t* dlvm_lang_alloc_ast_binary(dlvm_lang_position_t position, int8_t ast_kind, dlvm_lang_ast_node_t* left, dlvm_lang_ast_node_t* right);
dlvm_lang_ast_node_t* dlvm_lang_alloc_ast_int_lit(dlvm_lang_position_t position, int64_t value);
void dlvm_lang_dealloc_ast(dlvm_lang_ast_node_t* ast);
void dlvm_lang_dealloc_ast_binary(dlvm_lang_ast_node_t* ast);

void dlvm_lang_print_ast(dlvm_lang_ast_node_t* ast, int64_t depth);
void dlvm_lang_print_ast_int_lit(dlvm_lang_ast_node_t* int_lit, int64_t depth);
void dlvm_lang_print_ast_binary(dlvm_lang_ast_node_t* binary, int64_t depth);
void dlvm_lang_print_ast_unary(dlvm_lang_ast_node_t* unary, int64_t depth);
void dlvm_lang_print_ast_depth(int64_t depth);

dlvm_lang_value_t dlvm_lang_interpret(dlvm_lang_ast_node_t* ast);
dlvm_lang_value_t dlvm_lang_interpret_binary(dlvm_lang_ast_node_t* add);

// Value functions
dlvm_lang_value_t dlvm_lang_make_value_int(int64_t value);
dlvm_lang_value_t dlvm_lang_make_value_float(double value);
dlvm_lang_value_t dlvm_lang_make_value_fuck();
dlvm_lang_value_t dlvm_lang_make_value(uint8_t kind);

void dlvm_lang_print_value(dlvm_lang_value_t value);