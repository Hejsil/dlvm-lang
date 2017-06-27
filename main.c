#include <stdio.h>
#include "vm.h"
#include "compiler.h"

int main() {
    dlvm_lang_scanner_t scanner;

    dlvm_lang_init_scanner(&scanner, " 1000 + 100\n");

    while (scanner.peek.kind != DLVM_LANG_TOKEN_UNKNOWN) {
        dlvm_lang_token_t token = dlvm_lang_eat_token(&scanner);

        switch (token.kind) {
            case DLVM_LANG_TOKEN_NUMBER:
                printf("{ kind = %s, ivalue = %d }\n", "NUMBER", token.ivalue) ;
                break;
            case DLVM_LANG_TOKEN_PLUS:
                printf("{ kind = %s }\n", "PLUS") ;
                break;
            case DLVM_LANG_TOKEN_UNKNOWN:
                printf("{ kind = %s }\n", "UNKNOWN") ;
                break;
        }
    }

    return 0;
}