#ifndef LIST_H
#define LIST_H

typedef struct {
  int size, count;
  void **arr;
} List;

List * list_empty();
void list_enlarge(List *list);
int list_push(List *list, void *data);
int list_push_int(List *list, int value);
void * list_get(List *list, int index);
int list_get_int(List *list, int index);
List * list_slice(List *list, int start, int end);
void list_delete(List *list);
void list_scrap(List *list);

#endif