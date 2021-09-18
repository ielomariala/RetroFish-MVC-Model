#include "list.h"

#include "fish.h"

static const struct element_s end_mark = {NULL, (struct element_s *) &end_mark};

#define END_MARK ((struct element_s *) &end_mark)

list_t *list_empty(void *(*copy)(void *), int (*cmp)(void *, void *), void (*del)(void *)) {
    list_t *list = malloc(sizeof(list_t));
    list->head = END_MARK;
    list->copy = copy;
    list->cmp = cmp;
    list->del = del;
    return list;
}

int list_is_empty(list_t *list) {
    return list->head == END_MARK;
}

int list_is_member(list_t *list, void *v) {
    if (list_is_empty(list)) {
        //printf("Debug list_is_member: list empty!\n");
        return 0;
    }
    struct element_s *e = list->head;
    while (e != END_MARK) {
        if (list->cmp(v, e->v)) {
            return 1;
        }
        e = e->next;
    }
    return 0;
}


int list_add(list_t *list, void *v) {
    struct element_s *e = malloc(sizeof(struct element_s));
    void *copy = list->copy(v);
    e->v = copy;
    e->next = END_MARK;
    if (list_is_empty(list)) {
        //printf("Debug list_add: List is empty!\n");
        list->head = e;
    }
    else {
        struct element_s *tmp = list->head;
        if (list->cmp(tmp->v, v)){
            //printf("Debug list_add: Fish is already in list in head position!\n");
            return 0;
        }
        while (tmp->next != END_MARK) {
            if (list->cmp(tmp->next->v, v)){
                //printf("Debug list_add: Fish is already in list\n");
                return 0;
            }
            tmp = tmp->next;
        }
        tmp->next = e;
    }
    return 1;
}

int list_del(list_t *list, void *v) {
    if (list_is_empty(list)) {
        //printf("Debug list_del: List is empty!\n");
        return 0;
    }
    struct element_s *e = list->head;
    if (list->cmp(e->v, v)) {
        list->head = e->next;
        list->del(e->v);
        free(e);
        return 1;
    }
    else {
        while (e->next != END_MARK) {
            if(list->cmp(e->next->v, v)) {
                struct element_s *to_delete = e->next;
                e->next = e->next->next;
                list->del(to_delete->v);
                free(to_delete);
                return 1;
            }
            e = e->next;
        }
    }
    return 0;
}

void *list_at(list_t *list, int i) {
    if (i >= 0) {
        int counter = 0;
        struct element_s *e = list->head;
        while (e != END_MARK) {
            if (counter == i) {
                return e->v;//list->copy(e->v);
            }
            counter++;
            e = e->next;
        }
    }
    return NULL;
}

size_t list_size(list_t *list) {
    if (list_is_empty(list)) {
        return 0;
    }
    size_t size = 1;
    struct element_s *e = list->head;
    while (e->next != END_MARK) {
        e = e->next;
        size++;
    }
    return size;
}

void list_free(list_t *list) {
    struct element_s *e;
    while (!list_is_empty(list)) {
        e = list->head;
        list->head = list->head->next;
        list->del(e->v);
        free(e);
    }
    free(list);
    list = NULL;
}

void list_debug(list_t *list) {
    printf("Debugging list: size is %zu\n", list_size(list));
    struct element_s *e = list->head;
    int i = 0;
    while (e != END_MARK) {
        if (list->cmp == compare_fishes)
            printf("Fish %d = %s\n", i++, ((fish_t *) (e->v))->name);
        else {
            printf("View %d = N%d\n", i++, ((view_t *) (e->v))->id);
        }
        e = e->next;
    }
}