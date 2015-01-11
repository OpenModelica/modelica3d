#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#include "modcount.h"

/**
 * This is both a very simple example of an external Modelica library
 * and a very useful tool. If you ever find yourself in need for a mutable
 * counter in Modelica, use this pattern.
 */

typedef struct _Context {
  int counter;
} Context;

void* modcount_acquire_context() {
  Context* ctxt = (Context*)malloc(sizeof(Context));
  ctxt -> counter = 0;
  return ctxt;
}

int modcount_get(void* vctxt) {
  Context* ctxt = (Context*)vctxt;
  return ctxt -> counter;
}

int modcount_set(void* vctxt, int val) {
  Context* ctxt = (Context*)vctxt;
  ctxt -> counter = val;
  return ctxt -> counter;
}

void modcount_release_context(void* vctxt) {
  free(vctxt);
}

/**
 * Modelica tools will free strings passed to external functions,
 * so we need to hide our string objects on the heap...
 */
void* modcount_acquire_string(const char *content) {
  /* we do not know modelica's string length
     Instead of char*, we could also use C++ std::string here ...
   */
  char **res = (char**) malloc(sizeof(char*));
  const size_t len = strlen(content);
  char* buf = (char*)calloc(len+1, sizeof(char)); // +1 since it is 0-terminated
  strcpy(buf, content);
  *res = buf;
  return (void*) res;
}

void modcount_release_string(void *obj) {
  char **str = (char**) obj;
  free(*str);
  free(str);
}

const char* modcount_get_string(void *str) {
  /* copy, so omc does _not_ free */
  const char** content = (const char**) str;
  const size_t len = strlen(*content);
  char* buf = (char*)calloc(len+1,sizeof(char)); // +1 since it is 0-terminated
  strncpy(buf, *content, len+1);
  return buf;
}

void modcount_set_string(void *obj, const char *content) {
  const size_t len = strlen(content);
  char **res = (char**) obj;
  *res = realloc(*res, sizeof(char)*(len+1));
  strncpy(*res, content, len+1);
}
