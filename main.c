#include <stdio.h>
#include "src/parsing.h"
#include "src/hp_inkjet.h"
#include "src/interpret.h"
#include <inttypes.h>
#include "src/data_structures/array.h"
#include "src/data_structures/basic.h"

int main() {
    int b_data[10] = { 2, 4, 6, 8, 10, 12, 14, 16, 18, 20 };
    array_t(int) b = { .data = b_data, .capacity = sizeof(b_data) / sizeof(int) };
    array_t(int) c = array_slice_from_to(b, 5, 10);

    foreach_ptr(rev_array, item, c) {
        printf("[%d] = %d\n", (int)it.index, *item);
    }

    return 0;
    
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

    arraylist_t int_array = arraylist_make(int32_t);

    arraylist_add_last(&int_array, 32);
    arraylist_add_last(&int_array, 2);
    arraylist_add_last(&int_array, 5);
    arraylist_set(&int_array, int, 0, 20);

    scanner_t scanner = make_scanner(
            "var i = 2" // TODO, parsing many statements
            "print \"Hello World\n\""
            "print 5 + 5.0"
            "print \"\n\""
            "print 2"
            "print \"\n\""
            "print 100"
            "print \"\n\""
    );
    ast_node_t* ast = parse_statements(&scanner);
    print_ast(ast, 0);

    //interpret_ast(ast);

    dealloc_ast(ast);

    return 0;
}