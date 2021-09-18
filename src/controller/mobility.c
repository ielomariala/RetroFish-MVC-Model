#include "mobility.h"
int RANDOM;

void horizental(aquarium* aq, fish_t* f, int tickSpeed){
    front(aq, f, tickSpeed);
}

void vertical(aquarium* aq, fish_t* f, int tickSpeed){
    ascend(aq, f, tickSpeed);
}

void randomWayPoint(aquarium* aq, fish_t* f, int tickSpeed){
    int r = 1+rand()%RANDOM;
    int rx = r*(rand()%(2*tickSpeed) - tickSpeed);
    int ry = r*(rand()%(2*tickSpeed) - tickSpeed);

    f->posX_aq += rx;
    f->posY_aq += ry;

    if (border(aq, f)){
        if (f->posX_aq > aq->width-f->posX_aq){
            f->posX_aq = aq->width - f->width;
        }
        else {
            f->posX_aq = 0;
        }
        
        if (f->posY_aq < aq->height-f->posY_aq){
            f->posY_aq = 0;
        }
        else {
            f->posY_aq = aq->height-f->height;
        }
    }
}

void diagonal_R_D(aquarium* aq, fish_t* f, int xtickSpeed, int ytickSpeed){
    int rx = 1+rand()%RANDOM;
    int ry = 1+rand()%RANDOM;
    f->posX_aq += (xtickSpeed/2)*rx;
    f->posY_aq += (ytickSpeed/2)*ry;

    if (borderx(aq, f)){
        if (f->posX_aq > aq->width-f->posX_aq){
            f->posX_aq = aq->width - f->width;
            f->moving_style = (f->time_clock ? diagonal_l_d : diagonal_l_u);
        }
        else {
            f->posX_aq = 0;
            f->moving_style = (f->time_clock ? diagonal_r_u : diagonal_r_d);
        }
    }
    if (bordery(aq, f)){
        if (f->posY_aq > aq->height-f->posY_aq){
            f->posY_aq = aq->height-f->height;
            f->moving_style = (f->time_clock ? diagonal_l_u : diagonal_r_u);
        }
        else {
            f->posY_aq = 0;
            f->moving_style = (f->time_clock ? diagonal_r_d : diagonal_l_d);
        }
    }
}

void diagonal_L_U(aquarium* aq, fish_t* f, int xtickSpeed, int ytickSpeed){
    diagonal_R_D(aq, f, -xtickSpeed, -ytickSpeed);
}

void diagonal_L_D(aquarium* aq, fish_t* f, int xtickSpeed, int ytickSpeed){
    diagonal_R_D(aq, f, -xtickSpeed, ytickSpeed);
}
void diagonal_R_U(aquarium* aq, fish_t* f, int xtickSpeed, int ytickSpeed){
    diagonal_R_D(aq, f, xtickSpeed, -ytickSpeed);
}

void timeclock(aquarium* aq, fish_t* f, int xtickSpeed, int ytickSpeed){
    if (f->posX_aq < aq->width/2 && f->posY_aq < aq->height/2){
        if (f->time_clock)
            diagonal_R_U(aq, f, xtickSpeed, ytickSpeed);
        else
            diagonal_L_D(aq, f, xtickSpeed, ytickSpeed);
    }
    else if (f->posX_aq > aq->width/2 && f->posY_aq > aq->height/2){
        if (f->time_clock)
            diagonal_L_D(aq, f, xtickSpeed, ytickSpeed);
        else    
            diagonal_R_U(aq, f, xtickSpeed, ytickSpeed);
    }
    else if (f->posX_aq > aq->width/2 && f->posY_aq < aq->height/2){
        if (f->time_clock)
            diagonal_R_D(aq, f, xtickSpeed, ytickSpeed);
        else    
            diagonal_L_U(aq, f, xtickSpeed, ytickSpeed);
    }
    else if (f->posX_aq < aq->width/2 && f->posY_aq > aq->height/2){
        if (f->time_clock)
            diagonal_L_U(aq, f, xtickSpeed, ytickSpeed);
        else    
            diagonal_R_D(aq, f, xtickSpeed, ytickSpeed);
    }
    else {
        f->moving_style = 9 + rand()%4;
    }
}


void front(aquarium* aq, fish_t* f, int tickSpeed){ 
    int rx = 1+rand()%RANDOM;
    f->posX_aq += tickSpeed*rx;

    if (border(aq, f)){
        if (f->posX_aq > aq->width-f->posX_aq){
            f->posX_aq = aq->width - f->width;
            f->moving_style = Backwards;
        }
        else {
            f->posX_aq = 0;
            f->moving_style = Warcry;
        }
    }
}

void back(aquarium* aq, fish_t* f, int tickSpeed){
    front(aq, f, -tickSpeed);
}

void ascend(aquarium *aq, fish_t *f, int tickSpeed){
    int ry = 1+rand()%RANDOM;
    f->posY_aq += tickSpeed*ry;

    if (border(aq, f)){
        if (f->posY_aq > aq->height-f->posY_aq){
            f->posY_aq = aq->height-f->height;
            f->moving_style = Drown;
        }
        else {
            f->posY_aq = 0;
            f->moving_style = Ascend;
        }
    }
}

void drown(aquarium *aq, fish_t *f, int tickSpeed){
    ascend(aq, f, -tickSpeed);
}



int border(aquarium* aq, fish_t* f){
    return (borderx(aq, f) || bordery(aq, f));
}

int borderx(aquarium* aq, fish_t* f){
    return (f->posX_aq + f->width*aq->width/100 > aq->width || f->posX_aq < 0);
}

int bordery(aquarium* aq, fish_t* f){
    return (f->posY_aq + f->height*aq->height/100 > aq->height || f->posY_aq < 0);
}


void update_fish(aquarium* aq, fish_t* f, int tick,int real, int speedrandom){
    RANDOM = speedrandom;
    int n = 25;
    f->last_posX_aq = f->posX_aq;
    f->last_posY_aq = f->posY_aq;
    if (f->started){
        switch(f->moving_style){
            case HorizentalPathWay:
                horizental(aq, f, tick*n);
                break;
            case VerticalPathWay:
                vertical(aq, f, tick*n);
                break;
            case Timeclock:
                timeclock(aq, f, tick*n, tick*n);
                break;
            case AntiTimeclock:
                timeclock(aq, f, tick*n, tick*n);
                break;
            case RandomWayPoint:
                randomWayPoint(aq, f, tick*n);
                break;
            case Drown:
                drown(aq, f, tick*n);
                break;
            case Ascend:
                ascend(aq, f, tick*n);
                break;
            case Warcry:
                front(aq, f, tick*n);
                break;
            case Backwards:
                back(aq, f, tick*n);
                break;
            case diagonal_l_d:
                diagonal_L_D(aq, f, tick*n, tick*n);
                break;
            case diagonal_l_u:
                diagonal_L_U(aq, f, tick*n, tick*n);
                break;
            case diagonal_r_d:
                diagonal_R_D(aq, f, tick*n, tick*n);
                break;
            case diagonal_r_u:
                diagonal_R_U(aq, f, tick*n, tick*n);
                break;
            case not_supported:
                break;
            default:
                break;
        }
        if (real){
            f->realx = f->posX_aq;
            f->realy = f->posY_aq;
            f->real_last_x = f->last_posX_aq;
            f->real_last_y = f->last_posY_aq;
        }
    }
}
