#ifndef _BLOCK_H
#define _BLOCK_H
#include <string.h>
#include "reference.h"

struct block {
  char * name;
  char * tangle;
  char * exports;
  char * file;
  char * mkdirp;
  char * noweb;
  char * noweb_ref;
  char * output_dir;
  char * error;
  char * content;
  size_t content_capacity;
  char * tmp;
  size_t tmp_capacity;
  char * config;
  size_t config_capacity;
  char * language;
  size_t language_capacity;
  char * key;
  size_t lines;
  int depth;
  struct reference * references;
  struct block * next;
};

struct block * block_new(void);
void block_free(struct block * block);
int block_append_content(struct block * block, char c); // return count of appended char, 0 for error
int block_append_tmp(struct block * block, char c); // return count of appended char, 0 for error
int block_append_config(struct block * block, char c); // return count of appended char, 0 for error
int block_append_language(struct block * block, char c); // return count of appended char, 0 for error

int block_insert_content(struct block * block, int pos, char * content);

size_t block_expand_content(struct block * block); // return expanded capacity
size_t block_truncate_content(struct block * block, size_t size);

inline size_t block_content_capacity(struct block * block) {
  return block->content_capacity;
}
inline size_t block_content_length(struct block * block) {
  return strlen(block->content);
}
inline size_t block_lines(struct block * block) {
  return block->lines;
}

#define block_clear_tmp(block) do { memset((block)->tmp, 0, (block)->tmp_capacity);} while (0)
#endif
