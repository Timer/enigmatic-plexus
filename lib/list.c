#include "list.h"
#include <stdlib.h>
#include <assert.h>

List * list_empty() {
  List *list = malloc(sizeof(List));
  list->size = list->count = 0;
  list->arr = NULL;
  return list;
}

void list_enlarge(List *list) {
  if (list->arr) {
    if (list->count == list->size) {
      realloc(list->arr, (list->size *= 2) * sizeof(void *));
    }
  } else {
    list->size = 2;
    list->arr = malloc(sizeof(void *) * list->size);
  }
}

int list_push(List *list, void *data) {
  list_enlarge(list);
  (list->arr)[list->count] = data;
  return list->count++;
}

int list_push_int(List *list, int value) {
  list_enlarge(list);
  int *v = malloc(sizeof(int));
  *v = value;
  (list->arr)[list->count] = v;
  return list->count++;
}

void * list_get(List *list, int index) {
  assert(index < list->count);
  return (list->arr)[index];
}

int list_get_int(List *list, int index) {
  assert(index < list->count);
  return *((int *) ((list->arr)[index]));
}

List * list_slice(List *ol, int start, int end) {
  List *sl = list_empty();
  for (int index = start; index < end; ++index) {
    list_push(sl, (ol->arr)[index]);
  }
  return sl;
}

void list_delete(List *list) {
  for (int i = 0; i < list->count; ++i) {
    free(list->arr[i]);
  }
  list_scrap(list);
}

void list_scrap(List *list) {
  free(list->arr);
  list->arr = NULL;
  free(list);
}