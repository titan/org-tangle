#ifndef _PARSER_H
#define _PARSER_H
#include <stdlib.h>
#include <stdint.h>
#include "block.h"
#include "block-fsm.h"
#include "block-context.h"

struct parser_context {
  struct block * blocks;
  struct block_state_machine * fsm;
  struct block_context * block_context;
};

uint8_t parser_new(struct parser_context * ctx);
void parser_update(struct parser_context * ctx, char * input, size_t len);
void parser_finish(struct parser_context * ctx);
void parser_free(struct parser_context * ctx);

#endif
