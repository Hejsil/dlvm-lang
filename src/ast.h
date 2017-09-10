#pragma once
#include <inttypes.h>
#include "data_structures/arraylist.h"

#define DLVM_LANG_IS_BINARY 0x80
#define DLVM_LANG_IS_UNARY 0x40

typedef struct string_position_s {
    int64_t index, line, column;
} string_position_t;

typedef enum token_kind_e {
    TOKEN_INT           = 0x00,
    TOKEN_FLOAT         = 0x01,
    TOKEN_STRING        = 0x02,
    TOKEN_KEYWORD_VAR   = 0x03,
    TOKEN_IDENTIFIER    = 0x04,
    TOKEN_EQUAL         = 0x05,

    TOKEN_PAR_LEFT      = 0x10,
    TOKEN_PAR_RIGHT     = 0x11,

    TOKEN_KEYWORD_PRINT = 0x00 | DLVM_LANG_IS_UNARY,

    TOKEN_ADD           = 0x00 | DLVM_LANG_IS_BINARY,
    TOKEN_SUB           = 0x01 | DLVM_LANG_IS_BINARY,
    TOKEN_MUL           = 0x02 | DLVM_LANG_IS_BINARY,
    TOKEN_DIV           = 0x03 | DLVM_LANG_IS_BINARY,

    TOKEN_EOF           = 0xFE & ~DLVM_LANG_IS_BINARY & ~DLVM_LANG_IS_UNARY,
    TOKEN_UNKNOWN       = 0xFF & ~DLVM_LANG_IS_BINARY & ~DLVM_LANG_IS_UNARY,
} token_kind_t;

typedef struct token_s {
    uint8_t kind;
    string_position_t position;

    union {
        int64_t ivalue;
        double fvalue;
        char* svalue;
    };
} token_t;

typedef enum ast_kind_e {
    AST_LITERAL_INT    = 0x00,
    AST_LITERAL_FLOAT  = 0x01,
    AST_LITERAL_STRING = 0x02,

    AST_BINARY_ADD     = TOKEN_ADD,
    AST_BINARY_SUB     = TOKEN_SUB,
    AST_BINARY_MUL     = TOKEN_MUL,
    AST_BINARY_DIV     = TOKEN_DIV,

    AST_UNARY_PRINT    = TOKEN_KEYWORD_PRINT,
    AST_UNARY_PAR      = 0x01 | DLVM_LANG_IS_UNARY,

    AST_STATEMENTS     = 0x10,
    AST_DECL           = 0x11,
    AST_SYMBOL         = 0x12
} ast_kind_t;

typedef struct ast_node_s {
    uint8_t kind;
    string_position_t position;

    union {
        struct {
            struct ast_node_s* left;
            struct ast_node_s* right;
        } binary;

        struct {
            struct ast_node_s* child;
        } unary;

        struct {
            int64_t value;
        } int_lit;

        struct {
            double value;
        } float_lit;

        struct {
            char* value;
        } string_lit;

        struct {
            char* name;
        } symbol;

        struct {
            arraylist_t children;
        } statements;

        struct {
            char* name;
            struct ast_node_s* value;
        } decl;
    };
} ast_node_t;

string_position_t make_position(int64_t index, int64_t line, int64_t column);
token_t make_token(uint8_t kind, string_position_t position);

ast_node_t* alloc_ast(string_position_t position, uint8_t ast_kind);
ast_node_t* alloc_ast_unary(string_position_t position, uint8_t ast_kind, ast_node_t *child);
ast_node_t* alloc_ast_binary(string_position_t position, uint8_t ast_kind, ast_node_t *left, ast_node_t *right);
ast_node_t* alloc_ast_int_lit(string_position_t position, int64_t value);
ast_node_t* alloc_ast_float_lit(string_position_t position, double value);
ast_node_t* alloc_ast_string_lit(string_position_t position, char *value);
ast_node_t* alloc_ast_statements(string_position_t position);
ast_node_t* alloc_ast_decl(string_position_t position, char *name, ast_node_t *value);
ast_node_t* alloc_ast_symbol(string_position_t position, char *name);

void init_ast(ast_node_t *ast, string_position_t position, uint8_t ast_kind);
void init_ast_unary(ast_node_t *unary, string_position_t position, uint8_t ast_kind, ast_node_t *child);
void init_ast_binary(ast_node_t *binary, string_position_t position, uint8_t ast_kind, ast_node_t *left, ast_node_t *right);
void init_ast_int_lit(ast_node_t *int_lit, string_position_t position, int64_t value);
void init_ast_float_lit(ast_node_t *float_lit, string_position_t position, double value);
void init_ast_string_lit(ast_node_t *string_lit, string_position_t position, char *value);
void init_ast_statements(ast_node_t *statements, string_position_t position);
void init_ast_decl(ast_node_t *decl, string_position_t position, char *name, ast_node_t *value);
void init_ast_symbol(ast_node_t *decl, string_position_t position, char *name);

void dealloc_ast(ast_node_t *ast);
void deinit_ast(ast_node_t *ast);