#include <stdlib.h>
#include <string.h>
#include "block.h"

struct block * block_new(void) {
  struct block * block = (struct block *) malloc(sizeof(struct block));
  memset(block, 0, sizeof(struct block));
  if (block != NULL) {
    block->key = NULL;

    block->content = (char *) malloc(128);
    memset(block->content, 0, 128);
    block->content_capacity = 128;

    block->tmp = (char *) malloc(128);
    memset(block->tmp, 0, 128);
    block->tmp_capacity = 128;

    block->config = (char *) malloc(128);
    memset(block->config, 0, 128);
    block->config_capacity = 128;

    block->language = (char *) malloc(8);
    memset(block->language, 0, 8);
    block->language_capacity = 8;

  }
  return block;
}

void block_free(struct block * block) {
  if (block != NULL) {
    if (block->content != NULL) {
      free(block->content);
    }
    if (block->tmp != NULL) {
      free(block->tmp);
    }
    if (block->config != NULL) {
      free(block->config);
    }
    if (block->language != NULL) {
      free(block->language);
    }
    if (block->name != NULL) {
      free(block->name);
    }
    if (block->tangle != NULL) {
      free(block->tangle);
    }
    if (block->exports != NULL) {
      free(block->exports);
    }
    if (block->file != NULL) {
      free(block->file);
    }
    if (block->mkdirp != NULL) {
      free(block->mkdirp);
    }
    if (block->noweb != NULL) {
      free(block->noweb);
    }
    if (block->noweb_ref != NULL) {
      free(block->noweb_ref);
    }
    if (block->output_dir != NULL) {
      free(block->output_dir);
    }
    if (block->error != NULL) {
      free(block->error);
    }
    struct reference * refer, * tmp;
    refer = block->references;
    while (refer != NULL) {
      tmp = refer->next;
      reference_free(refer);
      refer = tmp;
    }
    free(block);
  }
}

size_t block_truncate_content(struct block * block, size_t size) {
  if (strlen(block->content) > size) {
    memset(block->content + size, 0, strlen(block->content) - size);
  }
  return strlen(block->content);
}

#define defn_block_append(field) int block_append_##field(struct block * block, char c) { \
    if (block->field##_capacity == strlen(block->field) + 1) {          \
      block->field##_capacity = block->field##_capacity << 1;           \
      void * ptr = realloc(block->field, block->field##_capacity);      \
      if (ptr == NULL) {                                                \
        return 0;                                                       \
      }                                                                 \
      block->field = ptr;                                               \
      memset(block->field + (block->field##_capacity >> 1), 0, block->field##_capacity >> 1); \
    }                                                                   \
    block->field[strlen(block->field)] = c;                             \
    return 1;                                                           \
  }

defn_block_append(content)
defn_block_append(tmp)
defn_block_append(config)
defn_block_append(language)


#define defn_block_expand(field) size_t block_expand_##field(struct block * block) { \
    block->field##_capacity = block->field##_capacity << 1;             \
    void * ptr = realloc(block->field, block->field##_capacity);        \
    if (ptr == NULL) {                                                  \
      return block->field##_capacity;                                   \
    }                                                                   \
    block->field = ptr;                                                 \
    memset(block->field + (block->field##_capacity >> 1), 0, block->field##_capacity >> 1); \
    return block->field##_capacity;                                     \
  }

defn_block_expand(content)
