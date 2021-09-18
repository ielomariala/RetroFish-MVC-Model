#ifndef VIEW_H
#define VIEW_H
#include "queue.h"

/** @struct view_s
 *  @brief This structure describes a view
 *  @var view_s::id
 *  Member 'id': The ID of the view
 *  @var view_s::x
 *  Member 'x': contains 'x' coordinate of the view
 *  @var view_s::y
 *  Member 'y': contains 'y' coordinate of the view
 *  @var view_s::width
 *  Member 'width': The width of the view
 *  @var view_s::height
 *  Member 'height': The height of the view
 *  @var view_s::free
 *  Member 'free': 1 if view is not occupied by a client, 0 otherwise
 */
typedef struct view_s {
    int id;
    int x;
    int y;
    int width;
    int height;
    int free;
} view_t;

/** @struct client
 *  @brief This structure defines a client
 *  @var client::sockfd
 *  Member 'sockfd': Socket number of client
 *  @var client::v
 *  Member 'v': Client view's
 *  @var client::connected
 *  Member 'connected': 1 if the client is connected, 0 otherwise
 *  @var client::last_use
 *  Member 'last_use': Time of the last client communication with the server
 */
typedef struct client{
    int sockfd;
    view_t* v; 
    int connected;
    int last_use;
    SIMPLEQ_ENTRY(client) clients;
} client;

/*!
 * \fn view_to_str(view_t *v, char *buff)
 * \brief Converts view to string
 * \param v View
 * \param buff Buffer to be filled with informations
 */
void view_to_str(view_t *v, char *buff);

/*!
 * \fn str_to_view(char *buff)
 * \brief Converts string to view
 * \param buff Buffer where informations are written
 */
view_t *str_to_view(char *buff);

/*!
 * \fn copy_view(void *v)
 * \brief Copies view v to a new view which will be returned
 * \param v View to copy
 */
void *copy_view(void *v);

/*!
 * \fn compare_views(void *v1, void *v2)
 * \brief Compares views v1 and v2 (comparaison of identifiers)
 * \param v1 First view
 * \param v2 Second view
 */
int compare_views(void *v1, void *v2);

/*!
 * \fn free_view(void *v)
 * \brief Frees view v
 * \param v View to free
 */
void free_view(void *v);

/*!
 * \fn is_free(void *v)
 * \brief Checks if view v is free (no client occupies it), returns 1 if view v is free, 0 otherwise
 * \param v View to check
 */
int is_free(view_t *v);

/*!
 * \fn set_free_opposite(view_t *v)
 * \brief Sets free (struct attibute representing freedom of view) to opposite (sets it to 1 if it was 0 and vice-versa)
 * \param v View to set
 */
void set_free_opposite(view_t *v);

/*!
 * \fn get_view_id(view_t* v)
 * \brief Returns v's identifier
 * \param v View
 */
int get_view_id(view_t* v);

#endif
