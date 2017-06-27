#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "compiler.h"

#define NEWLINE '\n'

void dlvm_lang_init_position(dlvm_lang_position_t* position, int index, int line, int column) {
    position->index = index;
    position->line = line;
    position->column = column;
}

// Scanner functions
void dlvm_lang_init_scanner(dlvm_lang_scanner_t* scanner, char* text) {
    scanner->text_size = strlen(text);
    scanner->text = text;

    // We always start at line 1, for user readability
    dlvm_lang_init_position(&scanner->position, 0, 1, 0);

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

        scanner->peek.kind = DLVM_LANG_TOKEN_NUMBER;
        scanner->peek.position = position;

        // atoi is smart and just parses numbers until it hits something else
        scanner->peek.ivalue = atoi(&scanner->text[position.index]);

        return eaten;
    }

    switch (current) {
        case '+':
            scanner->peek.kind = DLVM_LANG_TOKEN_PLUS;
            scanner->peek.position = position;
            break;
        default:
            scanner->peek.kind = DLVM_LANG_TOKEN_UNKNOWN;
            scanner->peek.position = position;
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