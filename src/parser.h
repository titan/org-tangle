#ifndef _PARSER_H
#define _PARSER_H
#include <stdlib.h>
#include <stdint.h>
#include "block.h"

struct parser_context {
  uint32_t state;
  struct block * blocks;
};

void parser_new(struct parser_context * ctx);
void parser_update(struct parser_context * ctx, char * input, size_t len);
void parser_finish(struct parser_context * ctx);
void parser_free(struct parser_context * ctx);

#endif
