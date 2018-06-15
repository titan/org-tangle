#include <string.h>
#include "parser.h"
#include "block-fsm.h"
#include "block-config-fsm.h"
#include "ref-fsm.h"

void parser_new(struct parser_context * ctx) {
  ctx->blocks = NULL;
  ctx->state = BLOCK_OUTSIDE_STATE;
}

void parser_update(struct parser_context * ctx, char * input, size_t len) {
  void* data[2];
  data[0] = ctx;

  for (int i = 0; i < len; i ++) {
    data[1] = &((char *)input)[i];
    switch (input[i]) {
    case ' ': ctx->state = block_transform_state(ctx->state, BLOCK_SPACE_EVENT, data); break;
    case '\n': ctx->state = block_transform_state(ctx->state, BLOCK_CR_EVENT, data); break;
    case '#': ctx->state = block_transform_state(ctx->state, BLOCK_SHARP_EVENT, data); break;
    case '+': ctx->state = block_transform_state(ctx->state, BLOCK__PLUS__EVENT, data); break;
    case '_': ctx->state = block_transform_state(ctx->state, BLOCK___EVENT, data); break;
    case 'b': ctx->state = block_transform_state(ctx->state, BLOCK_B_EVENT, data); break;
    case 'B': ctx->state = block_transform_state(ctx->state, BLOCK_B_EVENT, data); break;
    case 'c': ctx->state = block_transform_state(ctx->state, BLOCK_C_EVENT, data); break;
    case 'C': ctx->state = block_transform_state(ctx->state, BLOCK_C_EVENT, data); break;
    case 'd': ctx->state = block_transform_state(ctx->state, BLOCK_D_EVENT, data); break;
    case 'D': ctx->state = block_transform_state(ctx->state, BLOCK_D_EVENT, data); break;
    case 'e': ctx->state = block_transform_state(ctx->state, BLOCK_E_EVENT, data); break;
    case 'E': ctx->state = block_transform_state(ctx->state, BLOCK_E_EVENT, data); break;
    case 'g': ctx->state = block_transform_state(ctx->state, BLOCK_G_EVENT, data); break;
    case 'G': ctx->state = block_transform_state(ctx->state, BLOCK_G_EVENT, data); break;
    case 'i': ctx->state = block_transform_state(ctx->state, BLOCK_I_EVENT, data); break;
    case 'I': ctx->state = block_transform_state(ctx->state, BLOCK_I_EVENT, data); break;
    case 'n': ctx->state = block_transform_state(ctx->state, BLOCK_N_EVENT, data); break;
    case 'N': ctx->state = block_transform_state(ctx->state, BLOCK_N_EVENT, data); break;
    case 'r': ctx->state = block_transform_state(ctx->state, BLOCK_R_EVENT, data); break;
    case 'R': ctx->state = block_transform_state(ctx->state, BLOCK_R_EVENT, data); break;
    case 's': ctx->state = block_transform_state(ctx->state, BLOCK_S_EVENT, data); break;
    case 'S': ctx->state = block_transform_state(ctx->state, BLOCK_S_EVENT, data); break;
    default: ctx->state = block_transform_state(ctx->state, BLOCK_OTHERS_EVENT, data); break;
    }
  }
}

void parser_finish(struct parser_context * ctx) {
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
}

void block_do_action(enum BLOCK_ACTION action, void * data) {
  struct parser_context * ctx = (struct parser_context *)((void **)data)[0];
  switch (action) {
  case BLOCK_CREATE_ACTION: {
    struct block * tmp = block_new();
    if (tmp != NULL) {
      tmp -> next = ctx->blocks;
      ctx->blocks = tmp;
    }
    break;
  }
  case BLOCK_APPEND_TO_CONTENT_ACTION: {
    char c = *(char *) (((void **)data)[1]);
    block_append_content(ctx->blocks, c);
    break;
  }
  case BLOCK_APPEND_TO_CONFIG_ACTION: {
    char c = *(char *) (((void **)data)[1]);
    block_append_config(ctx->blocks, c);
    break;
  }
  case BLOCK_APPEND_TO_TMP_ACTION: {
    char c = *(char *) (((void **)data)[1]);
    block_append_tmp(ctx->blocks, c);
    break;
  }
  case BLOCK_APPEND_SPACE_AND_INPUT_TO_CONTENT_ACTION: {
    block_append_content(ctx->blocks, ' ');
    char c = *(char *) (((void **)data)[1]);
    block_append_content(ctx->blocks, c);
    break;
  }
  case BLOCK_APPEND_TMP_TO_CONTENT_ACTION: {
    char c = *(char *) (((void **)data)[1]);
    block_append_tmp(ctx->blocks, c);
    for (int i = 0, len = strlen(ctx->blocks->tmp); i < len; i ++) {
      block_append_content(ctx->blocks, ctx->blocks->tmp[i]);
    }
    block_clear_tmp(ctx->blocks);
    break;
  }
  case BLOCK_DONE_ACTION: {
    int state = BLOCK_CONFIG_LANGUAGE_STATE;
    void* d[4];
    d[0] = ctx->blocks;
    for (int i = 0, len = strlen(ctx->blocks->config); i < len; i ++) {
      d[1] = &ctx->blocks->config[i];
      switch (ctx->blocks->config[i]) {
      case ' ': state = block_config_transform_state(state, BLOCK_CONFIG_SPACE_EVENT, d); break;
      case '\n': state = block_config_transform_state(state, BLOCK_CONFIG_CR_EVENT, d); break;
      case ':': state = block_config_transform_state(state, BLOCK_CONFIG__COLON__EVENT, d); break;
      case '-': state = block_config_transform_state(state, BLOCK_CONFIG___EVENT, d); break;
      case 'a': state = block_config_transform_state(state, BLOCK_CONFIG_A_EVENT, d); break;
      case 'b': state = block_config_transform_state(state, BLOCK_CONFIG_B_EVENT, d); break;
      case 'd': state = block_config_transform_state(state, BLOCK_CONFIG_D_EVENT, d); break;
      case 'e': state = block_config_transform_state(state, BLOCK_CONFIG_E_EVENT, d); break;
      case 'f': state = block_config_transform_state(state, BLOCK_CONFIG_F_EVENT, d); break;
      case 'g': state = block_config_transform_state(state, BLOCK_CONFIG_G_EVENT, d); break;
      case 'i': state = block_config_transform_state(state, BLOCK_CONFIG_I_EVENT, d); break;
      case 'k': state = block_config_transform_state(state, BLOCK_CONFIG_K_EVENT, d); break;
      case 'l': state = block_config_transform_state(state, BLOCK_CONFIG_L_EVENT, d); break;
      case 'm': state = block_config_transform_state(state, BLOCK_CONFIG_M_EVENT, d); break;
      case 'n': state = block_config_transform_state(state, BLOCK_CONFIG_N_EVENT, d); break;
      case 'o': state = block_config_transform_state(state, BLOCK_CONFIG_O_EVENT, d); break;
      case 'p': state = block_config_transform_state(state, BLOCK_CONFIG_P_EVENT, d); break;
      case 'r': state = block_config_transform_state(state, BLOCK_CONFIG_R_EVENT, d); break;
      case 's': state = block_config_transform_state(state, BLOCK_CONFIG_S_EVENT, d); break;
      case 't': state = block_config_transform_state(state, BLOCK_CONFIG_T_EVENT, d); break;
      case 'u': state = block_config_transform_state(state, BLOCK_CONFIG_U_EVENT, d); break;
      case 'w': state = block_config_transform_state(state, BLOCK_CONFIG_W_EVENT, d); break;
      case 'x': state = block_config_transform_state(state, BLOCK_CONFIG_X_EVENT, d); break;
      default: state = block_config_transform_state(state, BLOCK_CONFIG_OTHERS_EVENT, d); break;
      }
    }
    state = block_config_transform_state(state, BLOCK_CONFIG_EOF_EVENT, d);

    state = REFERENCE_INITIAL_STATE;
    d[0] = NULL;
    d[2] = 0; // pos
    size_t lines = 1;
    for (int i = 0, len = strlen(ctx->blocks->content); i < len; i ++) {
      d[1] = &ctx->blocks->content[i];
      d[2] = &i;
      switch(ctx->blocks->content[i]) {
      case '<': state = reference_transform_state(state, REFERENCE_LESS_THAN_EVENT, d); break;
      case '>': state = reference_transform_state(state, REFERENCE_GREATER_THAN_EVENT, d); break;
      case '\n': lines ++; state = reference_transform_state(state, REFERENCE_CR_EVENT, d); break;
      default: state = reference_transform_state(state, REFERENCE_OTHERS_EVENT, d); break;
      }
    }
    state = reference_transform_state(state, REFERENCE_EOF_EVENT, d);
    ctx->blocks->references = d[0];
    ctx->blocks->lines = lines;

    break;
  }
  }
}

void block_config_do_action(enum BLOCK_CONFIG_ACTION action, void * data) {
  struct block * block = (struct block *)((void **)data)[0];
  switch (action) {
  case BLOCK_CONFIG_DONE_ACTION: {
    break;
  }
  case BLOCK_CONFIG_APPEND_LANGUAGE_ACTION: {
    char c = *(char *) (((void **)data)[1]);
    block_append_language(block, c);
    break;
  }
  case BLOCK_CONFIG_INVALID_KEY_ACTION: {
    block->error = "Invalid key\n";
    break;
  }
  case BLOCK_CONFIG_NO_VALUE_ERROR_ACTION: {
    block->error = "Key without value\n";
    break;
  }
  case BLOCK_CONFIG_SAVE_VALUE_ACTION: {
    *(block->key) = (char *)malloc(strlen(block->tmp) + 1);
    memset(*(block->key), 0, strlen(block->tmp) + 1);
    strcpy(*(block->key), block->tmp);
    break;
  }
  case BLOCK_CONFIG_SAVE_VALUE_AND_DONE_ACTION: {
    *(block->key) = (char *)malloc(strlen(block->tmp) + 1);
    memset(*(block->key), 0, strlen(block->tmp) + 1);
    strcpy(*(block->key), block->tmp);
    break;
  }
  case BLOCK_CONFIG_APPEND_VALUE_ACTION: {
    char c = *(char *) (((void **)data)[1]);
    block_append_tmp(block, c);
    break;
  }
  case BLOCK_CONFIG__COLON_ERROR_ACTION: {
    block->error = "No key specified\n";
    break;
  }
  case BLOCK_CONFIG__COLON_EXPORTS_ACTION: {
    block->key = &(block->exports);
    block_clear_tmp(block);
    break;
  }
  case BLOCK_CONFIG__COLON_FILE_ACTION: {
    block->key = &(block->file);
    block_clear_tmp(block);
    break;
  }
  case BLOCK_CONFIG__COLON_MKDIRP_ACTION: {
    block->key = &(block->mkdirp);
    block_clear_tmp(block);
    break;
  }
  case BLOCK_CONFIG__COLON_NOWEB_ACTION: {
    block->key = &(block->noweb);
    block_clear_tmp(block);
    break;
  }
  case BLOCK_CONFIG__COLON_NOWEB_REF_ACTION: {
    block->key = &(block->noweb_ref);
    block_clear_tmp(block);
    break;
  }
  case BLOCK_CONFIG__COLON_OUTPUT_DIR_ACTION: {
    block->key = &(block->output_dir);
    block_clear_tmp(block);
    break;
  }
  case BLOCK_CONFIG__COLON_TANGLE_ACTION: {
    block->key = &(block->tangle);
    block_clear_tmp(block);
    break;
  }
  }
}

void reference_do_action(enum REFERENCE_ACTION action, void * data) {
  struct reference * reference = (struct reference *)((void **)data)[0];
  switch (action) {
  case REFERENCE_CREATE_REFERENCE_ACTION: {
    struct reference * ref = reference_new();
    ref->next = reference;
    *((struct reference **)data) = ref;
    break;
  }
  case REFERENCE_CREATE_REFERENCE_NEWLINE_APPEND_TO_LEADING_ACTION: {
    struct reference * ref = reference_new();
    ref->next = reference;
    *((struct reference **)data) = ref;
    char c = *(char *) (((void **)data)[1]);
    reference_append_leading(ref, c);
    break;
  }
  case REFERENCE_APPEND_TO_LEADING_ACTION: {
    char c = *(char *) (((void **)data)[1]);
    reference_append_leading(reference, c);
    break;
  }
  case REFERENCE_DROP_REFERENCE_ACTION: {
    if (reference->keep == 0) {
      *((struct reference **)data) = reference->next;
      reference_free(reference);
    }
    break;
  }
  case REFERENCE_APPEND_TO_NAME_ACTION: {
    char c = *(char *) (((void **)data)[1]);
    reference_append_name(reference, c);
    break;
  }
  case REFERENCE_SET_START_ACTION: {
    int pos = *(int *) (((void **)data)[2]);
    reference->start = pos - 1;
    break;
  }
  case REFERENCE_SET_STOP_ACTION: {
    int pos = *(int *) (((void **)data)[2]);
    reference->stop = pos;
    break;
  }
  case REFERENCE_SAVE_REFERENCE_ACTION: {
    reference->keep = 1;
    break;
  }
  }
}
