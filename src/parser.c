#include <string.h>
#include "parser.h"
#include "block-fsm.h"
#include "block-context.h"
#include "block-config-fsm.h"
#include "ref-fsm.h"

uint8_t parser_new(struct parser_context * ctx) {
  ctx->blocks = NULL;
  ctx->fsm = (struct block_state_machine *) malloc(sizeof(struct block_state_machine));
  if (ctx->fsm) {
    ctx->block_context = (struct block_context *) malloc(sizeof(struct block_context));
    if (ctx->block_context) {
      ctx->block_context->blocks = NULL;
      ctx->block_context->ch = 0;
      block_state_machine_init(ctx->fsm, ctx->block_context);
      return 1;
    }
    free(ctx->block_context);
  }
  return 0;
}

void parser_update(struct parser_context * ctx, char * input, size_t len) {
  for (int i = 0; i < len; i ++) {
    ctx->block_context->ch = input[i];
    switch (input[i]) {
    case ' ': block_state_machine_process(ctx->fsm, BLOCK_SPACE_EVENT); break;
    case '\n': block_state_machine_process(ctx->fsm, BLOCK_CR_EVENT); break;
    case ',': block_state_machine_process(ctx->fsm, BLOCK_COMMA_EVENT); break;
    case '*': block_state_machine_process(ctx->fsm, BLOCK_ASTERISK_EVENT); break;
    case '#': block_state_machine_process(ctx->fsm, BLOCK_SHARP_EVENT); break;
    case '+': block_state_machine_process(ctx->fsm, BLOCK_PLUS_EVENT); break;
    case '_': block_state_machine_process(ctx->fsm, BLOCK_UNDERLINE_EVENT); break;
    case 'b': block_state_machine_process(ctx->fsm, BLOCK_B_EVENT); break;
    case 'B': block_state_machine_process(ctx->fsm, BLOCK_B_EVENT); break;
    case 'c': block_state_machine_process(ctx->fsm, BLOCK_C_EVENT); break;
    case 'C': block_state_machine_process(ctx->fsm, BLOCK_C_EVENT); break;
    case 'd': block_state_machine_process(ctx->fsm, BLOCK_D_EVENT); break;
    case 'D': block_state_machine_process(ctx->fsm, BLOCK_D_EVENT); break;
    case 'e': block_state_machine_process(ctx->fsm, BLOCK_E_EVENT); break;
    case 'E': block_state_machine_process(ctx->fsm, BLOCK_E_EVENT); break;
    case 'g': block_state_machine_process(ctx->fsm, BLOCK_G_EVENT); break;
    case 'G': block_state_machine_process(ctx->fsm, BLOCK_G_EVENT); break;
    case 'i': block_state_machine_process(ctx->fsm, BLOCK_I_EVENT); break;
    case 'I': block_state_machine_process(ctx->fsm, BLOCK_I_EVENT); break;
    case 'n': block_state_machine_process(ctx->fsm, BLOCK_N_EVENT); break;
    case 'N': block_state_machine_process(ctx->fsm, BLOCK_N_EVENT); break;
    case 'r': block_state_machine_process(ctx->fsm, BLOCK_R_EVENT); break;
    case 'R': block_state_machine_process(ctx->fsm, BLOCK_R_EVENT); break;
    case 's': block_state_machine_process(ctx->fsm, BLOCK_S_EVENT); break;
    case 'S': block_state_machine_process(ctx->fsm, BLOCK_S_EVENT); break;
    default: block_state_machine_process(ctx->fsm, BLOCK_OTHERS_EVENT); break;
    }
  }
}

void parser_finish(struct parser_context * ctx) {
  ctx->blocks = ctx->block_context->blocks;
}

void parser_free(struct parser_context * ctx) {
  struct block * next = NULL, * tmp = NULL;
  if (ctx->blocks != NULL) {
    next = ctx->blocks->next;
    block_free(ctx->blocks);
    while (next != NULL) {
      tmp = next->next;
      block_free(next);
      next = tmp;
    }
  }
  if (ctx->block_context != NULL) {
    free(ctx->block_context);
  }
  if (ctx->fsm != NULL) {
    free(ctx->fsm);
  }
}
