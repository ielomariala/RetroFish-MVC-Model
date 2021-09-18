#ifndef PARSER_H
#define PARSER_H
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "aquarium.h"

/*!
 * \fn server_parser(char *buffer, FILE* log, aquarium *aq)
 * \brief Parses the buffer (command put in the terminal prompt) and responds accordingly 
 * \param buffer Buffer containing commands in the terminal prompt
 * \param log Log file which will be filled accordingly as well
 * \param aq Aquarium which could be modified if the command states so
 * \param fish_speed_random The adress of the int we set to the number found in a server setting command, only if this number > 0 
 */
int server_parser(char *buffer, FILE* log, aquarium *aq, int *fish_speed_random);

/*!
 * \fn client_parser(char *buffer, aquarium *aq, char *write, client* c, int tick)
 * \brief Parses the buffer (command sent by clients) and responds accordingly 
 * \param buffer Buffer containing command sent by the client
 * \param aq Aquarium which could be modified if the command states so, or from which informations are taken
 * \param write Buffer to be filled with server's response
 * \param tick Usually it is fish-update-interval
 * \param fish_speed_random Usually it is speed_randomizer
 */
int client_parser(char *buffer, aquarium *aq, char *write, client* c, int tick, int fish_speed_random);

/*!
 * \fn greeting(char *view, client* c, aquarium* aq, char* wr)
 * \brief Parses the string view (greeting command sent by clients) and responds accordingly 
 * \param view String containing the rest of the greeting command sent by the client, for example (in as N1)
 * \param c Client which sent the command 
 * \param aq Aquarium which could be modified if the command states so, or from which informations are taken
 * \param wr Buffer to be filled with server's response
 */
void greeting(char *view, client* c, aquarium* aq, char* wr);

/*!
 * \fn status(aquarium *aq, client* c, char* wr)
 * \brief Responds to status command sent by the client c
 * \param aq Aquarium which could be modified if the command states so, or from which informations are taken  
 * \param c Client which sent the command
 * \param wr Buffer to be filled with server's response
 */
void status(aquarium *aq, client* c, char* wr);

/*!
 * \fn log_out(client* c, char* wr)
 * \brief Responds to log out command sent by the client c (logging out for us is logging out from your view, you can connect to another one using the greeting commands)
 * \param c Client which sent the command
 * \param wr Buffer to be filled with server's response
 */
void log_out(client* c, char* wr);

/*!
 * \fn ping_pong(char* num, char* wr)
 * \brief Responds to ping command sent by the client c
 * \param num String containing the number sent by the client, for example : ping 123 
 * \param wr Buffer to be filled with server's response
 */
void ping_pong(char* num, char* wr);

/*!
 * \fn getTime()
 * \brief Getting local machine time
 */
char* getTime();

/*!
 * \fn disconnect(client* c, char* wr)
 * \brief Responds disconnect command sent by the client c (disconnecting for us is disconnecting from the server, you no longer have a view and the server does not reply to your commands anymore)
 * \param c Client which sent the command
 * \param wr Buffer to be filled with server's response
 */
void disconnect(client* c, char* wr);

/*!
 * \fn setting(char *buff, int *fish_speed_random)
 * \brief Responds to setting server command
 * \param buff Rest of the command (Example: for the command: set speed_randomizer 3; the rest is: speed_randomizer 3  )
 * \param fish_speed_random The adress of the int we set to the number found in command, only if this number > 0  
 */
int setting(char *buff, int *fish_speed_random);

#endif