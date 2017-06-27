typedef struct dlvm_lang_position_s {
    int index, line, column;
} dlvm_lang_position_t;

typedef enum dlvm_lang_token_kind_e {
    DLVM_LANG_TOKEN_NUMBER,
    DLVM_LANG_TOKEN_PLUS,
    DLVM_LANG_TOKEN_UNKNOWN
} dlvm_lang_token_kind_t;

typedef struct dlvm_lang_token_s {
    dlvm_lang_token_kind_t kind;
    dlvm_lang_position_t position;

    union {
        long ivalue;
        double dvalue;
        char* svalue;
    };
} dlvm_lang_token_t;

typedef struct dlvm_lang_scanner_s {
    dlvm_lang_position_t position;
    int text_size;
    char* text;

    dlvm_lang_token_t peek;
} dlvm_lang_scanner_t;

typedef enum dlvm_lang_ast_kind_e {
    DLVM_LANG_AST_INTEGER_LITERAL,
    DLVM_LANG_AST_BINARY_PLUS
} dlvm_lang_ast_kind_t;

typedef struct dlvm_lang_ast_node_s {
    dlvm_lang_ast_kind_t kind;

    union {
        struct {
            struct dlvm_lang_ast_node_s* left;
            struct dlvm_lang_ast_node_s* right;
        } binary;

        struct {
            long value;
        } int_lit;
    };
} dlvm_lang_ast_node_t;

typedef struct dlvm_lang_parser_s {
    dlvm_lang_scanner_t scanner;
} dlvm_lang_parser_t;

typedef struct dlvm_lang_compiler_s {
    
} dlvm_lang_compiler_t;

void dlvm_lang_init_position(dlvm_lang_position_t* position, int index, int line, int column);

// Scanner functions
void dlvm_lang_init_scanner(dlvm_lang_scanner_t* scanner, char* text);
char dlvm_lang_peek_char(dlvm_lang_scanner_t* scanner);
char dlvm_lang_eat_char(dlvm_lang_scanner_t* scanner);
dlvm_lang_token_t dlvm_lang_eat_token(dlvm_lang_scanner_t* scanner);
void dlvm_lang_skip(dlvm_lang_scanner_t* scanner);

// Parser functions
void dlvm_lang_init_parser(dlvm_lang_parser_t* parser, dlvm_lang_scanner_t scanner);