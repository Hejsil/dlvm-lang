#pragma once
#include <inttypes.h>
#include "ast.h"

void print_ast(ast_node_t *ast, int64_t depth);
void print_ast_binary(ast_node_t *binary, int64_t depth);
void print_ast_unary(ast_node_t *unary, int64_t depth);
void print_ast_depth(int64_t depth);