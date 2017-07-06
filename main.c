#include <stdio.h>
#include "vm.h"
#include "hp_inkjet.h"
#include "parsing.h"
#include "interpret.h"

int main() {
    
    char buffer[1000];

    /*
    uint64_t prog[] = {PUSH, INT, 10, PUSH, INT, 2, MUL, PRINT, HALT};

    stack_t *stack = stack_init(100);
    program_t *program = malloc(sizeof(program_t));
    program->program = prog;
    program->program_size = 6;

    dlvm_t *vm = dlvm_init(program, stack);

    dlvm_exec(vm);

    scanf("%s", buffer);
    return 0;
    /**/

    dlvm_lang_scanner_t scanner;

    dlvm_lang_init_scanner(
        &scanner, 
        "5 / 2.0"
    );

    dlvm_lang_ast_node_t* ast = dlvm_lang_parse_expression(&scanner);
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