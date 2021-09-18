#include "parser.h"
#define TMP_BUFFER_SIZE 32

char* getTime(){
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    return asctime(timeinfo);
}

void double_print(char *s, FILE* log){
    printf("%s\n", s);
    fprintf(log, "%s\n", s);
}

int server_parser(char *buffer, FILE* log, aquarium *aq, int *fish_speed_random){
    if(!strcmp(buffer,"exit\n") || !strcmp(buffer, "q\n")){
        double_print("\t-> Exiting... (This could take a while)", log);
        return 0;
    }
    char *cmd = strtok(buffer," ");
    if(!strcmp(cmd,"load")){
        char path[TMP_BUFFER_SIZE] = "aquariums/";
        cmd = strtok(NULL, "\n");
        strcat(path,cmd);
        if(load_aquarium(aq, path)){
            printf("\t-> Aquarium loaded (%ld display views)\n", list_size(aq->views));
            fprintf(log, "\t-> Aquarium loaded (%ld display views)\n", list_size(aq->views));
        }
        else{
            printf("\t-> Aquarium %s not found !\n", path);
            fprintf(log, "\t-> Aquarium %s not found !\n", path);
        }
    }
    else if(!already_loaded(aq)){
        double_print("\t-> Aquarium not loaded", log);
    }
    else if(!strcmp(cmd,"show")){
        show_aquarium(aq);
        double_print("\t-> Aquarium displayed", log);
    }
    else if(!strcmp(cmd,"add")){
        cmd = strtok(NULL, " ");
        cmd = strtok(NULL, "\n");
        printf("%s\n",cmd);
        if(add_view(aq, cmd)){
            double_print("\t-> View added", log);
        }
    }
    else if(!strcmp(cmd,"del")){
        cmd = strtok(NULL, " ");
        cmd = strtok(NULL, "\n");
        if(del_view(aq, cmd)){
            printf("\t-> View %s deleted\n", cmd);
            fprintf(log, "\t-> View %s deleted\n", cmd);
        }
    }
    else if(!strcmp(cmd,"save")){
        cmd = strtok(NULL, "\n");
        save_aquarium(aq,cmd);
        printf("\t-> Aquarium saved (%ld display views)\n", list_size(aq->views));
        fprintf(log, "\t-> Aquarium saved (%ld display views)\n", list_size(aq->views));
    }
    else if(!strcmp(cmd,"set")){
        cmd = strtok(NULL, "\n");
        int t  = setting(cmd, fish_speed_random);
        if ( t >= 1){
            printf("\t-> OK : Speed randomizer set to %d\n", t);
            fprintf(log, "\t-> OK : Speed randomizer set to %d\n", t);
        }
        else {
            printf("\t-> NOK : Speed randomizer could not be set to %d, it should be >= 1\n", t);
            fprintf(log, "\t-> NOK : Speed randomizer could not be set to %d, it should be >= 1\n", t);
        }
    }
    else{
        double_print("Commande introuvable!\nTo exit controller, press q or write exit", log);
    }
    return 1;
}

int client_parser(char *buffer, aquarium *aq, char *write, client *c, int tick, int fish_speed_random){
    if (already_loaded(aq)){
        if (!strcmp(buffer, "hello")){
            greeting(buffer, c, aq, write);
        }
        else if (!c->connected){
            char *cmd = strtok(buffer," ");
            if(!strcmp(cmd,"hello")){
                cmd = strtok(NULL, "\n");
                greeting(cmd, c, aq, write);
            }
            else{
                strcat(write, "NOK: No view defined\n");
            }
        }
        else{
            if (!strcmp(buffer, "status")){
                status(aq, c, write);
            }
            else if (!strcmp(buffer, "disconnect")){
                disconnect(c, write);
                return 0;
            }
            else if (!strcmp(buffer, "getFishes")){
                get_fishes(aq, c->v, tick, write);
            }
            else if (!strcmp(buffer, "getFishesContinuously")){
                get_fishes_continuously(aq, c->v, tick, write);
            }
            else if (!strcmp(buffer, "startAll")){
                start_all(aq);
                strcat(write,"OK\n");
            }
            else if (!strcmp(buffer, "ls")){
                ls(aq, c->v, tick, write, fish_speed_random);
            }
            else {
                char *cmd = strtok(buffer," ");
                if(!strcmp(cmd,"addFish")){
                    cmd = strtok(NULL, "\n");
                    if (cmd != NULL && add_fish(aq, c->v, cmd)){
                        strcat(write,"OK\n");
                    }
                    else{
                        strcat(write, "NOK: Modèle de mobilité non supporté ou poisson déjà dans l'aquarium\n");
                    }
                }
                else if(!strcmp(cmd,"delFish")){
                    cmd = strtok(NULL, "\n");
                    if(cmd != NULL && del_fish(aq,cmd)){
                        strcat(write,"OK\n");
                    }
                    else{
                        strcat(write, "NOK: Poisson inexistant\n");
                    }
                }
                else if(!strcmp(cmd,"startFish")){
                    cmd = strtok(NULL, "\n");
                    if(cmd != NULL && start_fish(aq, cmd)){
                        strcat(write, "OK\n");
                    }
                    else{
                        strcat(write, "NOK\n");
                    }
                }
                else if(!strcmp(cmd,"log")){
                    cmd = strtok(NULL, "\n");
                    if (cmd!= NULL && !strcmp(cmd, "out")){
                        log_out(c, write);
                    }
                }
                else if(!strcmp(cmd,"ping")){
                    cmd = strtok(NULL, "\n");
                    ping_pong(cmd, write);
                } 
                else{
                    strcat(write, "NOK: Commande Introuvable\n");
                }
            }
        }
    }
    else{
        char *cmd = strtok(buffer," ");
        if(!strcmp(cmd,"ping")){
            cmd = strtok(NULL, "\n");
            ping_pong(cmd, write);
        }
        else{
            strcat(write, "Aquarium not loaded!\n");
        }
    }
    return 1;
}

void greeting(char *view, client* c, aquarium* aq, char * wr){
    if (c->connected != 1){
        if (!strcmp(view, "hello")){
            c->v = search_free_view(aq);
        }
        else{

            int view_id;
            sscanf(view, "in as N%d\n",&view_id);
            c->v = search_aq(aq, view_id);
            if (c->v == NULL){
                c->v = search_free_view(aq);
            }
        }
        if(c->v != NULL){
            c->connected = 1;
            char tmp[TMP_BUFFER_SIZE];
            sprintf(tmp,"greeting N%d\n", get_view_id(c->v));
            strcat(wr, tmp);
        }
        else{
            strcat(wr,"No greeting\n");
        }
    }
    else{
        char tmp[TMP_BUFFER_SIZE];
        sprintf(tmp,"Already greeted N%d\n", get_view_id(c->v));
        strcat(wr, tmp);
    }
}

void status(aquarium* aq, client* c, char* wr){
    if (c->connected){
        strcat(wr, "OK : Connecté au contôleur, ");
        print_fishes_in_view(aq, c->v, wr);
    }
    else{
        strcat(wr,"Non connecté\n");
    }
}

void log_out(client* c, char* wr){
    if (c->v != NULL){
        c->connected = 0;
        set_free_opposite(c->v);
        strcat(wr, "bye\n");
    }
    else{
        strcat(wr, "View undefined! Bye!\n");
    }
}

void disconnect(client* c, char* wr){
    if (c->v != NULL){
        c->connected = 0;
        set_free_opposite(c->v);
    }
    strcat(wr, "Disconnected from server!\n");
}

void ping_pong(char* num, char* wr){
    strcat(wr, "pong ");
    if (num != NULL){
        strcat(wr, num);
    }
    strcat(wr, "\n");
}

int setting(char *buff, int *fish_speed_random){
    int speed = *fish_speed_random;
    if (sscanf(buff, "speed_randomizer %d\n", &speed)){
        if (speed > 0){
            *fish_speed_random = speed;
        }
    }
    return speed;
}