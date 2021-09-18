#include "aquarium.h"
#include "mobility.h"
#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 128

aquarium *init_aquarium() {
    aquarium *aq = malloc(sizeof(aquarium));
    aq->views = list_empty(copy_view, compare_views, free_view);
    aq->fishes = list_empty(copy_fish, compare_fishes, free_fish);
    aq->height = 0;
    aq->width = 0;
    return aq;
}

int load_aquarium(aquarium *aq, char *filename) {
    FILE *file = fopen(filename, "r");
    
    if (file == NULL) {
        return 0;
    }
    fscanf(file, "%dx%d\n", &aq->width, &aq->height);
    char *line = NULL;
    size_t len = 0;
    while (getline(&line, &len, file) != -1) {
        view_t *v = str_to_view(line);
        list_add(aq->views, v);
        free_view(v);
    }
    free(line);
    fclose(file);
    return 1;
}

void save_aquarium(aquarium *aq, char *filename) {
    FILE *file = fopen(filename, "w");
    fprintf(file, "%dx%d\n", aq->width, aq->height);

    size_t size = list_size(aq->views);
    size_t i;
    for (i = 0; i < size; i++) {
        char strv[BUFFER_SIZE];
        view_t *v = list_at(aq->views, i);
        view_to_str(v, strv);
        strcat(strv, "\n");
        fputs(strv, file);
    }
    fclose(file);
}

int already_loaded(aquarium *aq){
    if(aq->height == 0 || aq->width == 0){
        return 0;
    }
    return 1;
}

int add_view(aquarium *aq, char *buff) {
    view_t *v = str_to_view(buff);
    int a = list_add(aq->views, v);
    free_view(v);
    return a;
}

int del_view(aquarium *aq, char *buff) {
    view_t *v = str_to_view(buff);
    int a = list_del(aq->views, v);
    free_view(v);
    return a;
}

void show_aquarium(aquarium *aq) {
    printf("%dx%d\n", aq->width, aq->height);
    size_t size = list_size(aq->views);
    size_t i;
    for (i = 0; i < size; i++) {
        char strv[BUFFER_SIZE];
        view_t *v = list_at(aq->views, i);
        view_to_str(v, strv);
        puts(strv);
    }
}


void finalize(aquarium *aq) {
    list_free(aq->views);
    list_free(aq->fishes);
    free(aq);
    aq = NULL;
}

int add_fish(aquarium *aq, view_t *v, char *fish) {
    fish_t *f = str_to_fish(v, fish);

    if (!supported_path(f)) {
        return 0;
    }
    int a = list_add(aq->fishes, f);
    free_fish(f);
    return a;
}

int del_fish(aquarium *aq, char *fish) {
    fish_t *f = str_to_fish(NULL, fish);

    int a = list_del(aq->fishes, f);
    free_fish(f);
    return a;
}

int start_fish(aquarium *aq, char *fish) {
    fish_t *f = str_to_fish(NULL, fish);
    size_t size = list_size(aq->fishes);
    size_t i;
    for (i = 0; i < size; i++) {
        fish_t *tmp = list_at(aq->fishes, i);
        if (compare_fishes(tmp, f)){
            tmp->started = 1;
            return 1;
        }
    }
    free_fish(f);
    return 0;
}

void start_all(aquarium *aq) {
    size_t size = list_size(aq->fishes);
    size_t i;
    for (i = 0; i < size; i++) {
        fish_t *f = list_at(aq->fishes, i);
        f->started = 1;
    }
}

void get_fishes(aquarium *aq, view_t *v, int tick, char *ret) {
    size_t size = list_size(aq->fishes);
    size_t i;
    unsigned long seconds = 0;
    struct timeval tv;
    if (tick > 1)
        gettimeofday(&tv, NULL);
    for (i = 0; i < size; i++) {
        fish_t * f = (fish_t *) list_at(aq->fishes, i);
        
        if (is_fish_in_view(v, f) || fish_just_left_view(v, f) || fish_just_entered_view(v, f)){
            char wr[BUFFER_SIZE];
            /* --- */
            if (f->posX_aq + f->width*v->width/100 > aq->width){
                f->posX_aq = aq->width - f->width*v->width/100;
            }
            if (f->posY_aq + f->height*v->height/100 > aq->height){
                f->posY_aq = aq->height - f->height*v->height/100;
            }
            /* --- */
            if (tick > 1)
                seconds = ((tv.tv_sec - aq->update_tick.tv_sec)*1000000 + (tv.tv_usec - aq->update_tick.tv_usec))/1000000;
            
            if (f->entered == 0){
                if (fish_just_entered_view(v, f)){
                    sprintf(wr, "list [ %s at %dx%d, %dx%d, %d ]\t", f->name, (f->last_posX_aq-v->x)*100/v->width, (f->last_posY_aq-v->y)*100/v->height, f->width, f->height, 0);
                    strcat(ret, wr);
                }
            }
            if (f->entered && is_fish_in_view(v, f) && !fish_just_entered_view(v, f))
                f->entered = 0;
    
            sprintf(wr, "list [ %s at %dx%d, %dx%d, %ld ]\t", f->name, (f->posX_aq-v->x)*100/v->width, (f->posY_aq-v->y)*100/v->height, f->width, f->height, tick-seconds);
            strcat(ret, wr);
        }
    }
    strcat(ret, "\n");
}

void get_fishes_continuously(aquarium *aq, view_t *v, int tick, char* ret) {
    int i;
    for(i = 0; i < 3; i++){
        get_fishes(aq, v, tick, ret);
        /* Send ret to client who should be waiting for 3 successive responses when he sends getFishesContinuously */
        sleep(tick);
    }
}

void ls(aquarium *aq, view_t *v, int tick, char* ret, int speedrandom){
    size_t i;
    size_t n = 3;
    size_t size = list_size(aq->fishes);
    for(i = 0; i < n; i++){   
        update_fishes(aq, tick, 0, speedrandom);
        if (i > 0){
            strcat(ret, "< ");
        }
        get_fishes(aq, v, tick, ret);
    }
    for (i = 0; i < size; i++) {
        fish_t * f = (fish_t *) list_at(aq->fishes, i);
        f->posX_aq = f->realx;
        f->posY_aq = f->realy;
        f->last_posX_aq = f->real_last_x;
        f->last_posY_aq = f->real_last_y;
   } 
}

view_t* search_aq(aquarium *aq, int view_id) {
    size_t size = list_size(aq->views);
    size_t i;
    for (i = 0; i < size; i++) {
        view_t *v;
        v = list_at(aq->views, i);
        if (get_view_id(v) == view_id && is_free(v)){
            set_free_opposite(v);
            return v;
        }
    }
    return NULL;
}

view_t* search_free_view(aquarium *aq) {
    size_t size = list_size(aq->views);
    size_t i;
    for (i = 0; i < size; i++) {
        view_t *v = list_at(aq->views, i);
        if (is_free(v)) {
            set_free_opposite(v);
            return v;
        }
    }
    return NULL;
}

void print_fishes_in_view(aquarium* aq, view_t* v, char* buffer){
    char fish_buff[BUFFER_SIZE];
    size_t size = list_size(aq->fishes);
    int count = 0;
    size_t i;
    for (i = 0; i < size; i++) {
        fish_t *f = list_at(aq->fishes, i);
        if (is_fish_in_view(v, f)){
            count++;
            fish_to_str(f, v, fish_buff);
            strcat(buffer, fish_buff);
        }
    }
    sprintf(fish_buff, "%d poissons trouvés\n", count);
    strcat(buffer, fish_buff);
}

void update_fishes(aquarium* aq, int tick, int real, int speedrandom){
    size_t i;
    if (real && tick > 1)
        gettimeofday(&(aq->update_tick), NULL);
    for(i = 0; i < list_size(aq->fishes); i++){
        fish_t * f = (fish_t *) list_at(aq->fishes, i);
        update_fish(aq, f, tick, real, speedrandom);
    } 
}
