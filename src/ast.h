#pragma once
#include <inttypes.h>

#define DLVM_LANG_IS_BINARY 0x80
#define DLVM_LANG_IS_UNARY 0x40

typedef struct dlvm_lang_position_s {
    int64_t index, line, column;
} dlvm_lang_position_t;

typedef enum dlvm_lang_token_kind_e {
    DLVM_LANG_TOKEN_INT       = 0x00,
    DLVM_LANG_TOKEN_FLOAT     = 0x01,

    DLVM_LANG_TOKEN_PAR_LEFT  = 0x10,
    DLVM_LANG_TOKEN_PAR_RIGHT = 0x11,

    DLVM_LANG_TOKEN_ADD       = 0x00 | DLVM_LANG_IS_BINARY,
    DLVM_LANG_TOKEN_SUB       = 0x01 | DLVM_LANG_IS_BINARY,
    DLVM_LANG_TOKEN_MUL       = 0x02 | DLVM_LANG_IS_BINARY,
    DLVM_LANG_TOKEN_DIV       = 0x03 | DLVM_LANG_IS_BINARY,

    DLVM_LANG_TOKEN_UNKNOWN   = 0xFF & ~DLVM_LANG_IS_BINARY & ~DLVM_LANG_IS_UNARY,
} dlvm_lang_token_kind_t;

typedef struct dlvm_lang_token_s {
    uint8_t kind;
    dlvm_lang_position_t position;

    union {
        int64_t ivalue;
        double fvalue;
        char* svalue;
    };
} dlvm_lang_token_t;

typedef enum dlvm_lang_ast_kind_e {
    DLVM_LANG_AST_LITERAL_INT = 0x00,
    DLVM_LANG_AST_LITERAL_FLOAT = 0x01,

    DLVM_LANG_AST_BINARY_ADD  = DLVM_LANG_TOKEN_ADD,
    DLVM_LANG_AST_BINARY_SUB  = DLVM_LANG_TOKEN_SUB,
    DLVM_LANG_AST_BINARY_MUL  = DLVM_LANG_TOKEN_MUL,
    DLVM_LANG_AST_BINARY_DIV  = DLVM_LANG_TOKEN_DIV,
    
    DLVM_LANG_AST_UNARY_PAR   = 0x00 | DLVM_LANG_IS_UNARY
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

        struct {
            double value;
        } float_lit;
    };
} dlvm_lang_ast_node_t;

dlvm_lang_position_t dlvm_lang_make_position(int64_t index, int64_t line, int64_t column);
dlvm_lang_token_t dlvm_lang_make_token(uint8_t kind, dlvm_lang_position_t position);

dlvm_lang_ast_node_t* dlvm_lang_alloc_ast(dlvm_lang_position_t position, int8_t ast_kind);
dlvm_lang_ast_node_t* dlvm_lang_alloc_ast_unary(dlvm_lang_position_t position, int8_t ast_kind, dlvm_lang_ast_node_t* child);
dlvm_lang_ast_node_t* dlvm_lang_alloc_ast_binary(dlvm_lang_position_t position, int8_t ast_kind, dlvm_lang_ast_node_t* left, dlvm_lang_ast_node_t* right);
dlvm_lang_ast_node_t* dlvm_lang_alloc_ast_int_lit(dlvm_lang_position_t position, int64_t value);
dlvm_lang_ast_node_t* dlvm_lang_alloc_ast_float_lit(dlvm_lang_position_t position, double value);
void dlvm_lang_dealloc_ast(dlvm_lang_ast_node_t* ast);