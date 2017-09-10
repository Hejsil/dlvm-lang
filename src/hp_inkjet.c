#include <stdio.h>
#include <assert.h>
#include "hp_inkjet.h"
#include "ast.h"

void print_ast(ast_node_t *ast, int64_t depth) {
    if (ast->kind & DLVM_LANG_IS_BINARY) {
        print_ast_binary(ast, depth);
    } else if (ast->kind & DLVM_LANG_IS_UNARY) {
        print_ast_unary(ast, depth);
    } else {
        print_ast_depth(depth);

        switch (ast->kind) {
            case AST_STATEMENTS: {
                printf("<STATEMENTS>\n");
                arraylist_t* list = &ast->statements.children;

                for (int i = 0; i < list->count; i++) {
                    ast_node_t* child = arraylist_get(list, ast_node_t*, i);
                    print_ast(child, depth + 2);
                }

                print_ast_depth(depth);
                printf("<STATEMENTS\\>\n");
                break;
            }

            case AST_DECL:
                printf("<DECL name=\"%s\"\\>\n", ast->string_lit.value);
                print_ast(ast->decl.value, depth + 2);
                print_ast_depth(depth);
                printf("<DECL\\>\n");
                break;

            case AST_LITERAL_STRING:
                printf("<STRING_LITERAL value=\"%s\"\\>\n", ast->string_lit.value);
                break;

            case AST_LITERAL_INT:
                printf("<INT_LITERAL value=\"%" PRId64 "\"\\>\n", ast->int_lit.value);
                break;

            case AST_LITERAL_FLOAT:
                printf("<FLOAT_LITERAL value=\"%f\"\\>\n", ast->float_lit.value);
                break;

            default:
                printf("<UNKNOWN\\>\n");
                break;
        }
    }
}

void print_ast_binary(ast_node_t *binary, int64_t depth) {
    assert(binary->kind & DLVM_LANG_IS_BINARY);

    uint8_t kind = binary->kind;
    char* name = 
        kind == AST_BINARY_ADD ? "ADD" :
        kind == AST_BINARY_SUB ? "SUB" :
        kind == AST_BINARY_MUL ? "MUL" :
        kind == AST_BINARY_DIV ? "DIV" :
        "???";

    print_ast_depth(depth);
    printf("<%s>\n", name);
    depth += 2;

    print_ast(binary->binary.left, depth);
    print_ast(binary->binary.right, depth);

    depth -= 2;
    print_ast_depth(depth);
    printf("<%s\\>\n", name);
}

void print_ast_unary(ast_node_t *unary, int64_t depth) {
    assert(unary->kind & DLVM_LANG_IS_UNARY);

    uint8_t kind = unary->kind;
    char* name = 
        kind == AST_UNARY_PAR ? "PAR" :
        kind == AST_UNARY_PRINT ? "PRINT" :
        "???";

    print_ast_depth(depth);
    printf("<%s>\n", name);
    depth += 2;

    print_ast(unary->unary.child, depth);

    depth -= 2;
    print_ast_depth(depth);
    printf("<%s\\>\n", name);
}

void print_ast_depth(int64_t depth) {
    for (int i = 0; i < depth; i++)
        printf(" ");
}