#include "reference.h"
#include "ref-fsm.h"
#include "ref-context.h"

void reference_create_reference(struct reference_context * ctx, enum REFERENCE_STATE state, enum REFERENCE_EVENT event) {
  (void) state;
  (void) event;
  struct reference * ref = reference_new();
  ref->next = ctx->reference;
  ctx->reference = ref;
}

void reference_create_reference_newline_append_to_leading(struct reference_context * ctx, enum REFERENCE_STATE state, enum REFERENCE_EVENT event) {
  (void) state;
  (void) event;
  struct reference * ref = reference_new();
  ref->next = ctx->reference;
  ctx->reference = ref;
  reference_append_leading(ref, ctx->ch);
}

void reference_append_to_leading(struct reference_context * ctx, enum REFERENCE_STATE state, enum REFERENCE_EVENT event) {
  (void) state;
  (void) event;
  reference_append_leading(ctx->reference, ctx->ch);
}

void reference_drop_reference(struct reference_context * ctx, enum REFERENCE_STATE state, enum REFERENCE_EVENT event) {
  (void) state;
  (void) event;
  if (ctx->reference->keep == 0) {
    struct reference * ref = ctx->reference;
    ctx->reference = ref->next;
    reference_free(ref);
  }
}

void reference_set_start(struct reference_context * ctx, enum REFERENCE_STATE state, enum REFERENCE_EVENT event) {
  (void) state;
  (void) event;
  ctx->reference->start = ctx->pos - 1;
}

void reference_append_to_name(struct reference_context * ctx, enum REFERENCE_STATE state, enum REFERENCE_EVENT event) {
  (void) state;
  (void) event;
  reference_append_name(ctx->reference, ctx->ch);
}

void reference_set_stop(struct reference_context * ctx, enum REFERENCE_STATE state, enum REFERENCE_EVENT event) {
  (void) state;
  (void) event;
  ctx->reference->stop = ctx->pos;
}

void reference_save_reference(struct reference_context * ctx, enum REFERENCE_STATE state, enum REFERENCE_EVENT event) {
  (void) state;
  (void) event;
  ctx->reference->keep = 1;
}
