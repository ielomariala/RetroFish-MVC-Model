/*!
 * \mainpage Network Project: Fish simulation in an aquarium
 *
  This project consists of simmulating numerous fishes swimming an aquarium. The aquarium is controlled by a server who has a list of views and fishes. Every client can connect to a view, which literally a view or unique perspective of the aquarium. Thus, the server or the controller should deliver correct and fast informations to all clients, in order to have proper and coherant visualisation of the fishes in all the views. In order to achieve good results, a good understanding of TCP communication protocols and sockets is compulsory.
  In brief, the project goal is to create a centralized fish aquarium. A controller controlls everything related to server, client connexions, aquarium and views. Moreover, a model contains informations about fishes, their dimensions, their patterns and mobilities. Furthermore, the client occupies a view and visualizes the correspondant fishes. Thus, as described, an MVC model is used, as the controlls links between the model and view.
 * \image html logo.png
 *
 * \authors BENMENDIL Hamza
 * \authors BOULLIT Mohamed Fayçal
 * \authors ELOMARI ALAOUI Ismail
 * \authors KINI Walid
 * \authors MECHACH Hamza
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <semaphore.h>

#include <arpa/inet.h>
#include <netdb.h>

#include <pthread.h>
#include "queue.h"
#include "parser.h"

#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif

#define WRITE_BUFFER_SIZE 1024
#define READ_BUFFER_SIZE 255
#define MAX_THREADS 5

int sockfd;
fd_set ready_sockets, current_sockets;

pthread_mutex_t mutex_queue,mutex_sockets;
sem_t sem_clients;
int max_sockets = 0;

/** @struct configuration
 *  @brief This structure contains server configuration
 *  @var configuration::portno
 *  Member 'portno': The port number
 *  @var configuration::display_timeout_value
 *  Member 'display_timeout_value': contains the time between 2 requests beyond which a display program is removed from the network
 *  @var configuration::fish_update_interval
 *  Member 'fish_update_interval': contains the time between 2 updates of fishes in the aquarium
 */
typedef struct configuration {
    int portno;
    int display_timeout_value;
    int fish_update_interval;
    int speed_randomizer;
} config;

typedef SIMPLEQ_HEAD(clients, client) queue;

queue head = SIMPLEQ_HEAD_INITIALIZER(head);

config cfg;

aquarium *aq;

int notexited = 1;

void error(char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

/*!
 * \fn configure_server(char* filename, config *cfg)
 * \brief Configure server from controller configuration file
 * \param filename File containing configuration of server
 * \param cfg Config struct to fill it with server configuration
 */
int configure_server(char* filename, config *cfg){
    FILE* server_log = fopen("server.log", "a");
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(server_log, "</ %s Server Event: Configuration file not found -%s-! />\n", getTime(), filename);
        fclose(server_log);
        exit(EXIT_FAILURE);
        return 0;
    }
    char *line = NULL;
    size_t len = 0;
    while (getline(&line, &len, file) != -1) {
        if (line[0] != '#'){
            sscanf(line, "controller-port = %d", &(cfg->portno));
            sscanf(line, "display-timeout-value = %d", &(cfg->display_timeout_value));
            sscanf(line, "fish-update-interval = %d", &(cfg->fish_update_interval));
        }
    }
    fprintf(server_log, "</ %s >>> Configuration <<<\n< controller-port = %d\n< display-timeout-value = %d\n< fish-update-interval = %d \n/>\n",
                             getTime(), cfg->portno, cfg->display_timeout_value, cfg->fish_update_interval);
    free(line);
    fclose(file);
    fclose(server_log);
    return 1;
}

/*!
 * \fn accept_new_connection(int socket_server)
 * \brief accept connection with client
 * \param socket_server socket's number of client
 */
client *accept_new_connection(int socket_server){
    unsigned int clilen;
    struct sockaddr_in cli_addr;
    clilen= sizeof(cli_addr);
    int newclient;
    newclient = accept(socket_server,
                (struct sockaddr*) &cli_addr, &clilen);
    client *cl = malloc(sizeof(client));
    cl->sockfd = newclient;
    cl->v = NULL;
    cl->connected = 0;
    cl->last_use = time(NULL);
    return cl;
}

/*!
 * \fn close_connection(client *cl)
 * \brief closes connection with a client
 * \param cl client structure
 */
void close_connection(client *cl){
    FD_CLR(cl->sockfd,&current_sockets);
    close(cl->sockfd);
    free(cl);
    cl = NULL; 
}

/*!
 * \fn handle_connections()
 * \brief manage connections between clients by allowing them to communicate with server
 */
void *handle_connections(void *arg __attribute__((unused))){
    int parser_res = 1;
    char read_buffer[READ_BUFFER_SIZE];
    char *write_buffer = malloc(sizeof(char)*WRITE_BUFFER_SIZE);
    while(notexited){
        parser_res = 1;
        FILE* server_log = fopen("server.log", "a");
        if(sem_trywait(&sem_clients) != 0){
            pthread_mutex_unlock(&mutex_sockets);
            while(sem_trywait(&sem_clients)!=0 && notexited){}
        }
        pthread_mutex_lock(&mutex_queue);
        client *cl = SIMPLEQ_FIRST(&head);
        if(notexited && cl != NULL)
            SIMPLEQ_REMOVE_HEAD(&head,clients);
        pthread_mutex_unlock(&mutex_queue);
        if(notexited && cl != NULL){
            if( FD_ISSET(cl->sockfd, &ready_sockets)) {
                bzero(read_buffer,READ_BUFFER_SIZE);
                int n = read(cl->sockfd,read_buffer,READ_BUFFER_SIZE);
                if (n < 0){
                    if (cl->v != NULL){
                        fprintf(server_log, "</ %s Server Event: ERROR reading from socket of client N%d />\n",getTime(), get_view_id(cl->v));
                    }
                    else
                        error("ERROR reading from socket");
                }
                bzero(write_buffer,WRITE_BUFFER_SIZE);
                strcat(write_buffer,"{\n");
                if(cl->v != NULL)
                    fprintf(server_log, "</ %s Client N%d: %s\n", getTime(), get_view_id(cl->v), read_buffer);
                else
                    fprintf(server_log, "</ %s Client Unknown: %s\n", getTime(), read_buffer);
                parser_res = client_parser(read_buffer, aq, write_buffer, cl, cfg.fish_update_interval, cfg.speed_randomizer);

                strcat(write_buffer, "}\n");
                fprintf(server_log, "Server: %s/>\n", write_buffer);
                n = write(cl->sockfd,write_buffer, WRITE_BUFFER_SIZE);
                cl->last_use = time(NULL);
                if (n < 0){
                    if (cl->v != NULL){
                        fprintf(server_log, "</%s Server Event: ERROR sending to client N%d />\n", getTime(), get_view_id(cl->v));                    }
                    else
                        error("ERROR sending to client");
                }
                if (parser_res == 0) {
                    close_connection(cl);
                }
            }
            if (parser_res && cl != NULL){
                if( (time(NULL) - cl->last_use) > cfg.display_timeout_value){
                    close_connection(cl);
                }
                else{           
                    pthread_mutex_lock(&mutex_queue);
                    SIMPLEQ_INSERT_TAIL(&head,cl,clients);
                    pthread_mutex_unlock(&mutex_queue);
                }
            }
        }
        fclose(server_log);
    }
    pthread_mutex_lock(&mutex_queue);
    client *n1 = SIMPLEQ_FIRST(&head);
    if(n1 != NULL)
        SIMPLEQ_REMOVE_HEAD(&head,clients);
    pthread_mutex_unlock(&mutex_queue);
    while(n1 != NULL){
        close_connection(n1);
        pthread_mutex_lock(&mutex_queue);
        n1 = SIMPLEQ_FIRST(&head);
        if(n1 != NULL)
            SIMPLEQ_REMOVE_HEAD(&head,clients);
        pthread_mutex_unlock(&mutex_queue);
    }
    free(write_buffer);
    return NULL;
}

/*!
 * \fn communicate()
 * \brief Function that detects new connections or attempts of communication with server
 */
void *communicate(void *arg __attribute__((unused))){
    FD_ZERO (&current_sockets);
    FD_SET (sockfd, &current_sockets);
    struct timeval *timeselect = malloc(sizeof(struct timespec));
    max_sockets = sockfd+1;
    while(notexited){
        FILE* server_log = fopen("server.log", "a");
        pthread_mutex_lock(&mutex_sockets);
        ready_sockets = current_sockets;
        timeselect->tv_sec = 5;
        timeselect->tv_usec = 0;

        if(select(max_sockets, &ready_sockets, NULL, NULL, timeselect) < 0){
            fprintf(server_log, "</ %s Server Event: select error />\n", getTime());
            perror("select error");
            exit(EXIT_FAILURE);
        }

        int cond=0;
        int i;
        for( i=0; i <= max_sockets; i++){
            if(FD_ISSET(i, &ready_sockets) && i!=sockfd){
                cond++;
                sem_post(&sem_clients);
            }
        }

        if(FD_ISSET(sockfd, &ready_sockets)) {
            fprintf(server_log, "<%s> Server Event: New client connected", getTime());
            client *cl = accept_new_connection(sockfd);
            FD_SET(cl->sockfd, &current_sockets);
            pthread_mutex_lock(&mutex_queue);
            SIMPLEQ_INSERT_TAIL(&head,cl,clients);
            pthread_mutex_unlock(&mutex_queue);
            if(cl->sockfd >= max_sockets){
                max_sockets = cl->sockfd+1;
            }
        }
        if(!cond){
            pthread_mutex_unlock(&mutex_sockets);
        }
        fclose(server_log);
    }
    free(timeselect);
    return NULL;
}

/*!
 * \fn communicate_server()
 * \brief Execute server commands
 */
void *communicate_server(void *arg __attribute__((unused))){
    char read_buffer[READ_BUFFER_SIZE];
    while(notexited){
        FILE* server_log = fopen("server.log", "a");
        printf("$ ");
        bzero(read_buffer,READ_BUFFER_SIZE);
        fgets(read_buffer,READ_BUFFER_SIZE,stdin);
        fprintf(server_log, "</ %s Server?: %s", getTime(), read_buffer);
        notexited = server_parser(read_buffer, server_log, aq, &(cfg.speed_randomizer));
        fprintf(server_log, "/>\n");
        fclose(server_log);
    }
    return NULL;
}

/*!
 * \fn update_aquarium()
 * \brief Updates the aquarium in an interval of time
 */
void *update_aquarium(void *arg __attribute__((unused))){
    while(notexited){
        sleep(cfg.fish_update_interval);
        if(already_loaded(aq))
            update_fishes(aq, cfg.fish_update_interval, 1, cfg.speed_randomizer);
    }
    return NULL;
}

int main(int argc, char* argv[])
{   
    srand(time(NULL)); // used for random mobility
    FILE * server_log = fopen("server.log","w");
    cfg.portno = 8081;
    cfg.display_timeout_value = 45;
    cfg.fish_update_interval = 1;
    cfg.speed_randomizer = 1;
    configure_server(argv[1], &cfg);
    aq = init_aquarium();
    int portno;
    pthread_t id[MAX_THREADS+3];
    struct sockaddr_in serv_addr;
    if(argc<2){
        fprintf(stderr,"ERROR, no configuration file provided\n");
        exit(1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM,0);
    if (sockfd <= 0){
        fprintf(server_log, "</ %s Server Event: ERROR opening socket />\n", getTime());
        error("ERROR opening socket");
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = cfg.portno;
    serv_addr.sin_family= AF_INET;
    serv_addr.sin_addr.s_addr= INADDR_ANY;
    serv_addr.sin_port= htons(portno);

    if (bind(sockfd, (struct sockaddr*) &serv_addr,
    sizeof(serv_addr)) < 0){
        fprintf(server_log, "</ %s Server Event: ERROR on binding />\n", getTime());
        error("ERROR on binding");
    }
    fclose(server_log);

    listen(sockfd,5);

    sem_init(&sem_clients,0,max_sockets);

    pthread_mutex_init(&mutex_queue, NULL);
    pthread_mutex_init(&mutex_sockets, NULL);
    // Thread to communicate with server -- Maybe we will just let the principal thread manage this ...
    pthread_create(&id[0],NULL,communicate_server,NULL);

    // Thread to listen with non-blocking I/O
    pthread_create(&id[1],NULL,communicate,NULL);

    // Thread to update fishes in aquarium
    pthread_create(&id[2],NULL,update_aquarium,NULL);

    // Threads to handle different connections
    int i;
    for( i=3; i < MAX_THREADS+3; i++){
        pthread_create(&id[i],NULL,handle_connections,NULL);
    }
    
    for( i=0; i < MAX_THREADS+3; i++){
        void *tmp;
        int rc = pthread_join(id[i],&tmp);
        FILE* server_log = fopen("server.log", "a");
        if(rc !=0 )
            fprintf(server_log, "</ %s Server Event: ERROR pthread_join %d />\n", getTime(), i);
        fprintf(server_log, "</ %s Server Event: Joined Thread %d />\n",getTime(), i);
        fclose(server_log);
    }

    finalize(aq);

    sem_destroy(&sem_clients);

    pthread_mutex_destroy(&mutex_queue);
    pthread_mutex_destroy(&mutex_sockets);

    return EXIT_SUCCESS;
}