#include <stdio.h>
#include "src/parsing.h"
#include "src/hp_inkjet.h"
#include "src/interpret.h"
#include <inttypes.h>
#include "src/data_structures/array.h"
#include "src/data_structures/basic.h"

int main() {
    int b_data[100] = { 22, 22 };
    array_t(int) b = { .data = b_data, .capacity = sizeof(b_data) / sizeof(int) };

    foreach_ptr(array, item, array_slice_from_length(b, 1, 50)) {
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

    arraylist_verbose_foreach(int, item, item_ptr, it, int_array) {
        printf("array[%d] = %d, %p\n", it, item, item_ptr);
    }

    arraylist_ptr_foreach(int, item_ptr, it, int_array) {
        printf("array[%d] = %p\n", it, item_ptr);
    }

    arraylist_foreach(int, item, it, int_array) {
        printf("array[%d] = %d\n", it, item);
    }

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