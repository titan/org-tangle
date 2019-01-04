#include <stdlib.h>
#include "block.h"
#include "block-fsm.h"
#include "block-context.h"
#include "block-config-fsm.h"
#include "block-config-context.h"
#include "ref-fsm.h"
#include "ref-context.h"

void block_create(struct block_context * ctx, enum BLOCK_STATE state, enum BLOCK_EVENT event) {
  (void) state;
  (void) event;
  struct block * tmp = block_new();
  if (tmp != NULL) {
    tmp -> next = ctx->blocks;
    ctx->blocks = tmp;
  }
}

void block_append_to_config(struct block_context * ctx, enum BLOCK_STATE state, enum BLOCK_EVENT event) {
  (void) state;
  (void) event;
  block_append_config(ctx->blocks, ctx->ch);
}

void block_append_to_content(struct block_context * ctx, enum BLOCK_STATE state, enum BLOCK_EVENT event) {
  (void) state;
  (void) event;
  block_append_content(ctx->blocks, ctx->ch);
}

void block_append_to_tmp(struct block_context * ctx, enum BLOCK_STATE state, enum BLOCK_EVENT event) {
  (void) state;
  (void) event;
  block_append_tmp(ctx->blocks, ctx->ch);
}

void block_append_space_and_input_to_content(struct block_context * ctx, enum BLOCK_STATE state, enum BLOCK_EVENT event) {
  (void) state;
  (void) event;
  block_append_content(ctx->blocks, ' ');
  block_append_content(ctx->blocks, ctx->ch);
}

void block_append_tmp_to_content(struct block_context * ctx, enum BLOCK_STATE state, enum BLOCK_EVENT event) {
  (void) state;
  (void) event;
  block_append_tmp(ctx->blocks, ctx->ch);
  for (int i = 0, len = strlen(ctx->blocks->tmp); i < len; i ++) {
    block_append_content(ctx->blocks, ctx->blocks->tmp[i]);
  }
  block_clear_tmp(ctx->blocks);
}

void block_clear_tmp_and_append_to_content(struct block_context * ctx, enum BLOCK_STATE state, enum BLOCK_EVENT event) {
  (void) state;
  (void) event;
  block_clear_tmp(ctx->blocks);
  block_append_content(ctx->blocks, ctx->ch);
}

void block_done(struct block_context * ctx, enum BLOCK_STATE state, enum BLOCK_EVENT event) {
  (void) state;
  (void) event;
  block_clear_tmp(ctx->blocks);
  struct block_config_state_machine conf_fsm;
  struct block_config_context conf_ctx;
  conf_ctx.block = ctx->blocks;
  block_config_state_machine_init(&conf_fsm, &conf_ctx);
  for (int i = 0, len = strlen(ctx->blocks->config); i < len; i ++) {
    conf_ctx.ch = ctx->blocks->config[i];
    switch (conf_ctx.ch) {
    case ' ': block_config_state_machine_process(&conf_fsm, BLOCK_CONFIG_SPACE_EVENT); break;
    case '\n': block_config_state_machine_process(&conf_fsm, BLOCK_CONFIG_CR_EVENT); break;
    case ':': block_config_state_machine_process(&conf_fsm, BLOCK_CONFIG_COLON_EVENT); break;
    default: block_config_state_machine_process(&conf_fsm, BLOCK_CONFIG_ALPHA_EVENT); break;
    }
  }
  block_config_state_machine_process(&conf_fsm, BLOCK_CONFIG_EOF_EVENT);
  if (conf_ctx.block->key != NULL) {
    free(conf_ctx.block->key);
    conf_ctx.block->key = NULL;
  }

  struct reference_context ref_ctx;
  struct reference_state_machine ref_fsm;
  ref_ctx.reference = NULL;
  ref_ctx.pos = 0;
  reference_state_machine_init(&ref_fsm, &ref_ctx);

  size_t lines = 1;
  for (int i = 0, len = strlen(ctx->blocks->content); i < len; i ++) {
    ref_ctx.ch = ctx->blocks->content[i];
    ref_ctx.pos = i;
    switch(ref_ctx.ch) {
    case '<': reference_state_machine_process(&ref_fsm, REFERENCE_LESS_THAN_EVENT); break;
    case '>': reference_state_machine_process(&ref_fsm, REFERENCE_GREATER_THAN_EVENT); break;
    case '\n': lines ++; reference_state_machine_process(&ref_fsm, REFERENCE_CR_EVENT); break;
    default: reference_state_machine_process(&ref_fsm, REFERENCE_OTHERS_EVENT); break;
    }
  }
  reference_state_machine_process(&ref_fsm, REFERENCE_EOF_EVENT);
  ctx->blocks->references = ref_ctx.reference;
  ctx->blocks->lines = lines;
}
