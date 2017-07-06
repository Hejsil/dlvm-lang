#pragma once
#include <inttypes.h>
#include "ast.h"

void dlvm_lang_print_ast(dlvm_lang_ast_node_t* ast, int64_t depth);
void dlvm_lang_print_ast_int_lit(dlvm_lang_ast_node_t* int_lit, int64_t depth);
void dlvm_lang_print_ast_binary(dlvm_lang_ast_node_t* binary, int64_t depth);
void dlvm_lang_print_ast_unary(dlvm_lang_ast_node_t* unary, int64_t depth);
void dlvm_lang_print_ast_depth(int64_t depth);