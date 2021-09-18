#include "fish.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *get_moving_style(enum moving_style m) {
    if (m == RandomWayPoint)
        return "RandomWayPoint";
    else if (m == HorizentalPathWay || m == Backwards || m == Warcry)
        return "HorizentalPathWay";
    else if (m == VerticalPathWay || m == Ascend || m == Drown)
        return "VerticalPathWay";
    else if (m == Timeclock)
        return "Timeclock";
    else if (m == AntiTimeclock)
        return "AntiTimeclock";
    return NULL;
}

void fish_to_str(fish_t *f, view_t *v, char *buff) {
    char* start;
    if (f->started){
        start = "started";
    }
    else {
        start = "notStarted";
    }
    sprintf(buff, "%s at %dx%d, %dx%d, %s\n", f->name, (f->posX_aq-v->x)*100/v->width, (f->posY_aq-v->y)*100/v->height, f->width, f->height, start);
}

fish_t *str_to_fish(view_t *v, char *buff) {
    fish_t *f = malloc(sizeof(fish_t));
    f->started = 0;
    f->time_clock = 1;
    f->entered = 0;
    char move[100];
    int percentx;
    int percenty;

    sscanf(buff, "%s at %dx%d, %dx%d, %s\n", f->name, &percentx, &percenty, &f->width, &f->height, move);
        if (v != NULL){
            if (move[0] == 'R'){
                f->moving_style = RandomWayPoint;
            } 
            else if (move[0] == 'V'){
                f->moving_style = VerticalPathWay;
            } 
            else if (move[0] == 'H'){
                f->moving_style = HorizentalPathWay;
            }
            else if (move[0] == 'T'){
                f->moving_style = Timeclock;
            }
            else if (move[0] == 'A'){
                f->moving_style = AntiTimeclock;
                f->time_clock = 0;
            }

            /* --- */
            else if (move[0] == 'D'){
                f->moving_style = Drown;
            }
            else if (move[0] == 'B'){
                f->moving_style = Backwards;
            }
            else if (move[0] == 'W'){
                f->moving_style = Warcry;
            }
            else if (move[0] == 'D' && move[1] == 'R' && move[0] == 'U'){
                f->moving_style = diagonal_r_u;
            }
            else if (move[0] == 'D' && move[1] == 'R' && move[0] == 'D'){
                f->moving_style = diagonal_r_d;
            }
            else if (move[0] == 'D' && move[1] == 'L' && move[0] == 'U'){
                f->moving_style = diagonal_l_u;
            }
            else if (move[0] == 'D' && move[1] == 'L' && move[0] == 'D'){
                f->moving_style = diagonal_l_d;
            }
            /* --- */
            else{
                f->moving_style = not_supported;
            }  

            f->posX_aq = v->x + percentx * (v->width/100);
            f->posY_aq = v->y + percenty * (v->height/100);
            f->last_posX_aq = -1;
            f->last_posY_aq = -1;

            f->realx = f->posX_aq;
            f->realy = f->posY_aq;
            f->real_last_x = f->last_posX_aq;
            f->real_last_y = f->last_posY_aq;
        }
    
    return f;
}

void *copy_fish(void *f) {
    fish_t *_f = (fish_t *) f;
    fish_t *copy = malloc(sizeof(fish_t));
    *copy = *_f;
    return copy;
}

int compare_fishes(void *f1, void *f2) {
    fish_t *_f1 = (fish_t *)f1;
    fish_t *_f2 = (fish_t *)f2;
    return (strcmp(_f2->name ,_f1->name) == 0);
}

void free_fish(void *f) {
    free(f);
}

int supported_path(fish_t *f) {
    if (f->moving_style == not_supported)
        return 0;
    return 1;
}

int is_fish_new(fish_t *f){
    return (f->last_posX_aq == -1 && f->last_posY_aq == -1);
}

int is_fish_in_view(view_t *v, fish_t *f) {
    if (((v->x <= f->posX_aq && f->posX_aq < v->width + v->x)/* || (v->x <= f->posX_aq+f->width && f->posX_aq+f->width <= v->width + v->x)*/) &&
        ((v->y <= f->posY_aq && f->posY_aq < v->height + v->y)/* || (v->y <= f->posY_aq+f->height && f->posY_aq+f->height <= v->height + v->y)*/))
        return 1;
    return 0;
}

int was_fish_in_view(view_t *v, fish_t *f) {
    if (((v->x <= f->last_posX_aq && f->last_posX_aq < v->width + v->x) /*|| (v->x <= f->last_posX_aq+f->width && f->last_posX_aq+f->width <= v->width + v->x)*/) &&
        ((v->y <= f->last_posY_aq && f->last_posY_aq < v->height + v->y) /*|| (v->y <= f->last_posY_aq+f->height && f->last_posY_aq+f->height <= v->height + v->y)*/))
        return 1;
    return 0;
}

int fish_just_entered_view(view_t *v, fish_t *f){
    if (!is_fish_new(f) && !was_fish_in_view(v, f) && is_fish_in_view(v, f)){
        f->entered = 1;
        return 1;
    }
    return 0;
}

int fish_just_left_view(view_t *v, fish_t *f){
    return (was_fish_in_view(v, f) && !is_fish_in_view(v, f));
}
