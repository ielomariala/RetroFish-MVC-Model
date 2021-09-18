#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <sys/time.h>
void error(char *msg)
{    
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char write_buffer[256];
    char read_buffer[256];
    if (argc< 3) {
        fprintf(stderr,"usage %s hostname port\n", argv[0]);exit(0);
    }
    portno= atoi(argv[2]);
    sockfd= socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd< 0)
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) 
    {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family= AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port= htons(portno);
    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);

    if (connect(sockfd,(const struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    gettimeofday(&end_time, NULL);
    bzero(write_buffer,256);
    sleep(2);
    strcat(write_buffer,"hello in as N1");
    gettimeofday(&start_time, NULL);
    n = write(sockfd,write_buffer,strlen(write_buffer));
    if (n < 0) error("ERROR writing to socket");
        n = read(sockfd, read_buffer, 255);    
    if( n<0 )
        error("ERROR reading from server");
    gettimeofday(&end_time, NULL);
    printf("%ld\n",(end_time.tv_sec-start_time.tv_sec)*1000000 + (end_time.tv_usec-start_time.tv_usec));
    bzero(write_buffer,256);
    sleep(2);
    strcat(write_buffer,"disconnect");
    n = write(sockfd,write_buffer,strlen(write_buffer));
    n = read(sockfd, read_buffer, 255);
    return 0;
}