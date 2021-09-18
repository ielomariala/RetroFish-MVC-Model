#include "view.h"

#include <stdio.h>
#include <stdlib.h>

void view_to_str(view_t *v, char *buff) {
    sprintf(buff, "N%d %dx%d+%d+%d", v->id, v->x, v->y, v->width, v->height);
}

view_t *str_to_view(char *buff) {
    view_t *v = malloc(sizeof(view_t));
    sscanf(buff, "N%d %dx%d+%d+%d\n", &v->id, &v->x, &v->y, &v->width, &v->height);
    v->free = 1;
    return v;
}

void *copy_view(void *v) {
    view_t *_v = (view_t *)v;
    view_t *copy = malloc(sizeof(view_t));
    copy->id = _v->id;
    copy->x = _v->x;
    copy->y = _v->y;
    copy->height = _v->height;
    copy->width = _v->width;
    copy->free = _v->free;
    return copy;
}

int compare_views(void *v1, void *v2) {
    view_t *_v1 = (view_t *)v1;
    view_t *_v2 = (view_t *)v2;
    return _v1->id == _v2->id;
}

void free_view(void *v) {
    view_t *_v = (view_t *)v;
    free(_v);
    v = NULL;
}

int is_free(view_t *v) {
    return v->free == 1;
}

void set_free_opposite(view_t *v) {
    if (v->free == 1)
        v->free = 0;
    else
        v->free = 1;
}
    
int get_view_id(view_t* v){
    return v->id;
}


