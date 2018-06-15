#ifndef _REFERENCE_H
#define _REFERENCE_H
#include <string.h>
struct reference {
  int start;
  int stop;
  char * leading;
  size_t leading_capacity;
  char * name;
  size_t name_capacity;
  int keep; // keep the reference
  struct reference * next;
};

struct reference * reference_new(void);
void reference_free(struct reference * reference);
int reference_append_leading(struct reference * reference, char c); // return count of appended char, 0 for error
int reference_append_name(struct reference * reference, char c); // return count of appended char, 0 for error

inline size_t reference_leading_length(struct reference * reference) {
  return strlen(reference->leading);
}


#define reference_clear_leading(ref) do { memset((ref)->leading, 0, (ref)->leading_capacity);} while (0)

#endif
