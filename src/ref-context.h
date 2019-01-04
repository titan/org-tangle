#ifndef __REF_CONTEXT_H
#define __REF_CONTEXT_H
#include "reference.h"
struct reference_context {
  struct reference * reference;
  char ch;
  int pos;
};
#endif
