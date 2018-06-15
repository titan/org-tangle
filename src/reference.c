#include <stdlib.h>
#include <string.h>
#include "reference.h"

struct reference * reference_new(void) {
  struct reference * ref = (struct reference *) malloc(sizeof(struct reference));
  memset(ref, 0, sizeof(struct reference));
  if (ref != NULL) {
    ref->leading = (char *) malloc(128);
    memset(ref->leading, 0, 128);
    ref->leading_capacity = 128;
    ref->name = (char *) malloc(128);
    memset(ref->name, 0, 128);
    ref->name_capacity = 128;
  }
  return ref;
}

void reference_free(struct reference * ref) {
  if (ref != NULL) {
    if (ref->name) {
      free(ref->name);
    }
    if (ref->leading) {
      free(ref->leading);
    }
    free(ref);
  }
}

#define defn_reference_append(field) int reference_append_##field(struct reference * reference, char c) { \
    if (reference->field##_capacity == strlen(reference->field) + 1) {  \
      reference->field##_capacity = reference->field##_capacity << 1;   \
      void * ptr = realloc(reference->field, reference->field##_capacity); \
      if (ptr == NULL) {                                                \
        return 0;                                                       \
      }                                                                 \
      reference->field = ptr;                                           \
      memset(reference->field + (reference->field##_capacity >> 1), 0, reference->field##_capacity >> 1); \
    }                                                                   \
    reference->field[strlen(reference->field)] = c;                     \
    return 1;                                                           \
  }

defn_reference_append(name)
defn_reference_append(leading)
