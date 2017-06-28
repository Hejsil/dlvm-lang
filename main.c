#include <stdio.h>
#include "vm.h"
#include "compiler.h"

int main() {
    char buffer[1000];

    dlvm_lang_scanner_t scanner;
    dlvm_lang_parser_t parser;

    dlvm_lang_init_scanner(&scanner, "5 / 2");
    dlvm_lang_init_parser(&parser, scanner);

    dlvm_lang_ast_node_t* ast = dlvm_lang_parse_expression(&parser);
    dlvm_lang_print_ast(ast, 0);

    dlvm_lang_value_t value = dlvm_lang_interpret(ast);

    printf("\n");
    dlvm_lang_print_value(value);

    dlvm_lang_dealloc_ast(ast);

    /*
    while (scanner.peek.kind != DLVM_LANG_TOKEN_UNKNOWN) {
        dlvm_lang_token_t token = dlvm_lang_eat_token(&scanner);

        switch (token.kind) {
            case DLVM_LANG_TOKEN_NUMBER:
                printf("{ kind = %s, ivalue = %d }\n", "NUMBER", token.ivalue) ;
                break;
            case DLVM_LANG_TOKEN_ADD:
                printf("{ kind = %s }\n", "PLUS") ;
                break;
            case DLVM_LANG_TOKEN_UNKNOWN:
                printf("{ kind = %s }\n", "UNKNOWN") ;
                break;
        }
    }
    */

    scanf("%s", buffer);

    return 0;
}