#include <stdio.h>
#include "vm.h"

int main() {
    uint64_t prog[] = { 
        PUSH, 10,
        PUSH, 2, 
        MUL, 
        PRINT, 
        HALT
    };

    printf("Hello World!");
    printf("Hello World!");

    stack_t *stack = stack_init(100);
    program_t *program = malloc(sizeof(program_t));
    program->program = prog;
    program->program_size = 6;

    dlvm_t *vm = dlvm_init(program, stack);

    dlvm_exec(vm);

    return 0;
}