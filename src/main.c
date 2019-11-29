#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include "parser.h"

static struct block * find_block_by_name(struct block * blocks, char * name) {
  struct block * block = blocks;
  while (block != NULL) {
    if ((block->name != NULL && strcmp(block->name, name) == 0) || (block->noweb_ref != NULL && strcmp(block->noweb_ref, name) == 0)) {
      return block;
    }
    block = block->next;
  }
  return block;
}

void load(char * filename, struct parser_context * ctx) {
  int fd = 0; // stdin
  if (!(strlen(filename) == 1 && filename[0] == '-')) {
    fd = open(filename, O_RDONLY);
  }
  if (fd == -1) {
    printf("Cannot open %s\n", filename);
    return;
  }
  char buf[1024];
  int r = 0;
  while ((r = read(fd, buf, 1024)) > 0) {
    parser_update(ctx, buf, r);
  }
  parser_finish(ctx);
}

int check(struct block * blocks) {
  int errnum = 0;
  struct block * block = blocks;
  while (block != NULL) {
    if (block->error != NULL) {
      printf("Error: %s at %s\n", block->error, block->config);
      errnum ++;
    }
    block = block->next;
  }
  return errnum;
}

void _depth(struct block * block, struct block * blocks) {
  if (block->references != NULL) {
    struct reference * reference = block->references;
    int max_depth = 0;
    while (reference != NULL) {
      struct block * subblock = find_block_by_name(blocks, reference->name);
      if (subblock != NULL) {
        if (subblock->depth > max_depth) {
          max_depth = subblock->depth;
        }
      }
      reference = reference->next;
    }
    block->depth = max_depth + 1;
  } else {
    block->depth = 0;
  }
}

size_t depth(struct block * blocks) {
  size_t total = 0, last_total = 0;
  struct block * block = blocks;
  while (block != NULL) {
    _depth(block, blocks);
    total += block->depth;
    block = block->next;
  }
  while (last_total != total) {
    last_total = total;
    total = 0;
    block = blocks;
    while (block != NULL) {
      _depth(block, blocks);
      total += block->depth;
      block = block->next;
    }
  }
  return total;
}

static void _merge(struct block * block, struct block * blocks) {
  if (block != NULL) {
    if (block->depth != 0) {
      struct reference * refer = block->references;
      struct reference * last_refer = refer;
      while (refer != NULL) {
        struct block * found = find_block_by_name(blocks, refer->name);
        if (found != NULL) {
          if (found->depth != 0) {
            _merge(found, blocks); // merge found block first
            _depth(found, blocks); // recalculate depth of found block
          }
          const size_t refer_size = block_content_length(found) + reference_leading_length(refer) * block_lines(found);
          int diff = (int)block_content_capacity(block) - (int)refer_size;
          while (diff <= 0) {
            block_expand_content(block);
            diff = (int)block_content_capacity(block) - (int)refer_size;
          }
          char * tmp = NULL;
          if (strlen(block->content) == refer->stop + 1) {
            // reference is at the last of block content
          } else {
            tmp = (char *)malloc(strlen(block->content) - refer->stop); // to save the rest content
            memset(tmp, 0, strlen(block->content) - refer->stop);
            if (tmp != NULL) {
              memcpy(tmp, block->content + refer->stop + 1, strlen(block->content) - refer->stop - 1);
            }
          }
          block_truncate_content(block, refer->start);

          for (int i = 0, referlen = strlen(found->content); i < referlen; i ++) {
            if (found->content[i] == '\n') {
              block_append_content(block, '\n');
              if (i + 1 < referlen) {
                for (int j = 0, leadlen = strlen(refer->leading); j < leadlen; j ++) {
                  block_append_content(block, refer->leading[j]);
                }
              }
            } else {
              block_append_content(block, found->content[i]);
            }
          }
          if (tmp != NULL) {
            for (int i = 0, len = strlen(tmp); i < len; i ++) {
              block_append_content(block, tmp[i]);
            }
            free(tmp);
          }

          // adjust reference position in merged content
          int offset = refer_size - (refer->stop - refer->start + 1);
          if (offset != 0) {
            struct reference * tmpref = refer->next;
            while (tmpref != NULL) {
              if (tmpref->start > refer->start) {
                tmpref->start += offset;
                tmpref->stop += offset;
              }
              tmpref = tmpref->next;
            }
          }
          if (last_refer == refer) {
            last_refer = refer->next;
            block->references = last_refer;
            reference_free(refer);
            refer = block->references;
          } else {
            last_refer->next = refer->next;
            reference_free(refer);
            refer = last_refer;
          }
          _depth(block, blocks); // recalculate depth of block
        } else {
          last_refer = refer;
          if (refer != NULL) {
            refer = refer->next;
          }
        }
      }
    }
  }
}

void merge(struct block * blocks) {
  size_t last = depth(blocks);
  size_t depths = last;
  struct block * block = blocks;
  while (block != NULL) {
    if (block->tangle == NULL && block->content[strlen(block->content) - 1] == '\n') {
      block->content[strlen(block->content) - 1] = 0;
    }
    block = block->next;
  }
  block = blocks;
  while (block != NULL) {
    _merge(block, blocks);
    block = block->next;
  }
  depths = depth(blocks);
  while (last != depths) {
    block = blocks;
    while (block != NULL) {
      _merge(block, blocks);
      block = block->next;
    }
    last = depths;
    depths = depth(blocks);
  }
}

struct block * filter(struct block ** blocks) {
  struct block * result = NULL;
  struct block * rest = NULL;
  struct block * block = * blocks;
  while (block != NULL) {
    struct block * tmp = block->next;
    if (block->tangle != NULL) {
      block->next = result;
      result = block;
    } else {
      block->next = rest;
      rest = block;
    }
    block = tmp;
  }
  * blocks = rest;
  return result;
}

void mkdirp(char * path) {
  size_t len = strlen("mkdir -p ") + strlen(path);
  char cmd[len + 1];
  memset(cmd, 0, len + 1);
  memcpy(cmd, "mkdir -p ", sizeof("mkdir -p ") - 1);
  memcpy(cmd + sizeof("mkdir -p ") - 1, path, strlen(path));
  system(cmd);
}


int main(int argc, char ** argv) {
  if (argc == 2) {
    struct parser_context ctx;
    memset(&ctx, 0, sizeof(struct parser_context));
    parser_new(&ctx);
    load(argv[1], &ctx);

    if (check(ctx.blocks) == 0) {
      depth(ctx.blocks);
      merge(ctx.blocks);
      struct block * block = filter(&ctx.blocks);
      struct block * used = NULL;
      while (block != NULL) {
        struct block * usediter = used;
        int file_recreated = 0;
        while (usediter != NULL) {
          if (strcmp(usediter->tangle, block->tangle) == 0) {
            file_recreated = 1;
            break;
          }
          usediter = usediter->next;
        }
        if (file_recreated == 0) {
          // delete the old tangled file
          unlink(block->tangle);
        }
        char path[strlen(block->tangle) + 1];
        memset(path, 0, strlen(block->tangle) + 1);
        memcpy(path, block->tangle, strlen(block->tangle));
        char * sep = strrchr(path, '/');
        if (sep != NULL) {
          * sep = 0;
        }
        mkdirp(path);
        FILE * f = fopen(block->tangle, "a");
        if (f) {
          fwrite(block->content, 1, strlen(block->content), f);
          fclose(f);
        }
        struct block * tmp = block->next;
        block->next = used;
        used = block;
        block = tmp;
      }
      block = used;
      while (block != NULL) {
        struct block * tmp = block->next;
        block_free(block);
        block = tmp;
      }
    }

    parser_free(&ctx);
  } else {
    printf("Usage: org-tangle src\r\n");
  }
  return 0;
}
