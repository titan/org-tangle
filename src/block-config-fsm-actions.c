#include <stdlib.h>
#include "block.h"
#include "block-config-context.h"
#include "block-config-fsm.h"

void block_config_done(struct block_config_context * ctx, enum BLOCK_CONFIG_STATE state, enum BLOCK_CONFIG_EVENT event) {
  (void) state;
  (void) event;
}

void block_config_invalid_key(struct block_config_context * ctx, enum BLOCK_CONFIG_STATE state, enum BLOCK_CONFIG_EVENT event) {
  (void) state;
  (void) event;
  ctx->block->error = "Invalid key\n";
}

void block_config_append_key(struct block_config_context * ctx, enum BLOCK_CONFIG_STATE state, enum BLOCK_CONFIG_EVENT event) {
  (void) state;
  (void) event;
  block_append_tmp(ctx->block, ctx->ch);
}

void block_config_save_key(struct block_config_context * ctx, enum BLOCK_CONFIG_STATE state, enum BLOCK_CONFIG_EVENT event) {
  (void) state;
  (void) event;
  if (ctx->block->key != NULL) {
    free(ctx->block->key);
    ctx->block->key = NULL;
  }
  ctx->block->key = (char *) malloc(strlen(ctx->block->tmp) + 1);
  if (ctx->block->key) {
    memset(ctx->block->key, 0, strlen(ctx->block->tmp) + 1);
    strcpy(ctx->block->key, ctx->block->tmp);
  }
  block_clear_tmp(ctx->block);
}

void block_config_no_value_error(struct block_config_context * ctx, enum BLOCK_CONFIG_STATE state, enum BLOCK_CONFIG_EVENT event) {
  (void) state;
  (void) event;
  ctx->block->error = "Key without value\n";
}

void block_config_save_value(struct block_config_context * ctx, enum BLOCK_CONFIG_STATE state, enum BLOCK_CONFIG_EVENT event) {
  (void) state;
  (void) event;
  if (ctx->block->key) {
    if (strcmp("tangle", ctx->block->key) == 0) {
      char * tmp = (char *) malloc(strlen(ctx->block->tmp) + 1);
      if (tmp) {
        memset(tmp, 0, strlen(ctx->block->tmp) + 1);
        strcpy(tmp, ctx->block->tmp);
        if (ctx->block->tangle) {
          free(ctx->block->tangle);
          ctx->block->tangle = NULL;
        }
        ctx->block->tangle = tmp;
      }
    } else if (strcmp("exports", ctx->block->key) == 0) {
      char * tmp = (char *) malloc(strlen(ctx->block->tmp) + 1);
      if (tmp) {
        memset(tmp, 0, strlen(ctx->block->tmp) + 1);
        strcpy(tmp, ctx->block->tmp);
        if (ctx->block->exports) {
          free(ctx->block->exports);
          ctx->block->exports = NULL;
        }
        ctx->block->exports = tmp;
      }
    } else if (strcmp("file", ctx->block->key) == 0) {
      char * tmp = (char *) malloc(strlen(ctx->block->tmp) + 1);
      if (tmp) {
        memset(tmp, 0, strlen(ctx->block->tmp) + 1);
        strcpy(tmp, ctx->block->tmp);
        if (ctx->block->file) {
          free(ctx->block->file);
          ctx->block->file = NULL;
        }
        ctx->block->file = tmp;
      }
    } else if (strcmp("mkdirp", ctx->block->key) == 0) {
      char * tmp = (char *) malloc(strlen(ctx->block->tmp) + 1);
      if (tmp) {
        memset(tmp, 0, strlen(ctx->block->tmp) + 1);
        strcpy(tmp, ctx->block->tmp);
        if (ctx->block->mkdirp) {
          free(ctx->block->mkdirp);
          ctx->block->mkdirp = NULL;
        }
        ctx->block->mkdirp = tmp;
      }
    } else if (strcmp("noweb", ctx->block->key) == 0) {
      char * tmp = (char *) malloc(strlen(ctx->block->tmp) + 1);
      if (tmp) {
        memset(tmp, 0, strlen(ctx->block->tmp) + 1);
        strcpy(tmp, ctx->block->tmp);
        if (ctx->block->noweb) {
          free(ctx->block->noweb);
          ctx->block->noweb = NULL;
        }
        ctx->block->noweb = tmp;
      }
    } else if (strcmp("noweb-ref", ctx->block->key) == 0) {
      char * tmp = (char *) malloc(strlen(ctx->block->tmp) + 1);
      if (tmp) {
        memset(tmp, 0, strlen(ctx->block->tmp) + 1);
        strcpy(tmp, ctx->block->tmp);
        if (ctx->block->noweb_ref) {
          free(ctx->block->noweb_ref);
          ctx->block->noweb_ref = NULL;
        }
        ctx->block->noweb_ref = tmp;
      }
    } else if (strcmp("output-dir", ctx->block->key) == 0) {
      char * tmp = (char *) malloc(strlen(ctx->block->tmp) + 1);
      if (tmp) {
        memset(tmp, 0, strlen(ctx->block->tmp) + 1);
        strcpy(tmp, ctx->block->tmp);
        if (ctx->block->output_dir) {
          free(ctx->block->output_dir);
          ctx->block->output_dir = NULL;
        }
        ctx->block->output_dir = tmp;
      }
    }
  }
  block_clear_tmp(ctx->block);
}

void block_config_append_value(struct block_config_context * ctx, enum BLOCK_CONFIG_STATE state, enum BLOCK_CONFIG_EVENT event) {
  (void) state;
  (void) event;
  block_append_tmp(ctx->block, ctx->ch);
}
