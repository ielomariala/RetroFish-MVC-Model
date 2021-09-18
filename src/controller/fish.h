#ifndef FISH_H
#define FISH_H

#include "view.h"
#include <string.h>

typedef enum moving_style {
    RandomWayPoint,
    HorizentalPathWay,
    VerticalPathWay,
    Timeclock,
    AntiTimeclock,
    Drown,
    Ascend,
    Warcry,
    Backwards,
    diagonal_l_u,
    diagonal_l_d,
    diagonal_r_u,
    diagonal_r_d,
    not_supported = -1,
} ms;

/** @struct fish_s
 *  @brief This structure describes a fish
 *  @var fish_s::name
 *  Member 'name': contains the name of the fish
 *  @var fish_s::width
 *  Member 'width': contains the width of the fish (in percentage with the view, where it is)
 *  @var fish_s::height
 *  Member 'height': contains the height of the fish (in percentage with the view, where it is)
 *  @var fish_s::posX_aq
 *  Member 'posX_aq': contains the 'x' position of fish in the aquarium
 *  @var fish_s::posY_aq
 *  Member 'posY_aq': contains the 'y' position of fish in the aquarium
 *  @var fish_s::entered
 *  Member 'entered': 1 if the fish is enterring in the view
 *  @var fish_s::last_posX_aq
 *  Member 'last_posX_aq': contains the previous 'x' position of fish in the aquarium
 *  @var fish_s::last_posY_aq
 *  Member 'last_posY_aq': contains the previous 'y' position of fish in the aquarium
 *  @var fish_s::started
 *  Member 'started': 1 if the fish is moving
 *  @var fish_s::moving_style
 *  Member 'moving_style': contains the moving_style of the fish
 *  @var fish_s::time_clock
 *  Member 'time_clock': 1 if the moving_style is time clock
 *  @var fish_s::realx
 *  Member 'realx': contains the real 'x' position of fish in the aquarium (used for prediction)
 *  @var fish_s::realy
 *  Member 'realy': contains the real 'y' position of fish in the aquarium (used for prediction)
 *  @var fish_s::real_last_x
 *  Member 'real_last_x': contains the real previous 'x' position of fish in the aquarium (used for prediction)
 *  @var fish_s::real_last_y
 *  Member 'real_last_y': contains the real previous 'y' position of fish in the aquarium (used for prediction)
 */
typedef struct fish_s {
    char name[100];
    int width;
    int height;
    int posX_aq; // Fish x position in the aquarium
    int posY_aq; // Fish y position in the aquarium
    int entered; // 1 if fish if going to enter a new view in the next fish-update-interval, 0 otherwise (used for change of views fluidity)

    int last_posX_aq; // used for change of views fluidity
    int last_posY_aq; // used for change of views fluidity

    int started;
    ms moving_style;

    int time_clock; // used to help diffentiate between timeclock and antitimeclock mobility
    int realx;
    int realy;
    int real_last_x;
    int real_last_y;
} fish_t;

/*!
 * \fn fish_to_str(fish_t *f, char *buff)
 * \brief Converts fish to string (used in 'status' command)
 * \param f Fish
 * \param v View which called the 'status' command
 * \param buff Buffer to be filled with informations
 */
void fish_to_str(fish_t *f, view_t *v, char *buff);

/*!
 * \fn str_to_fish( view_t *v, char *buff)
 * \brief Converts string to fish
 * \param v View which will be used to know where the fish is at in the whole aquarium
 * \param buff Buffer where informations are written
 */
fish_t *str_to_fish(view_t *v, char *buff);

/*!
 * \fn copy_fish(void *f)
 * \brief Copies fish f to a new fish which will be returned
 * \param f fish to copy
 */
void *copy_fish(void *f);

/*!
 * \fn compare_fishes(void *f1, void *f2)
 * \brief Compares fishes f1 and f2 (comparaison of names)
 * \param f1 First fish
 * \param f2 Second fish
 */
int compare_fishes(void *f1, void *f2);

/*!
 * \fn free_fish(void *f)
 * \brief Frees fish f
 * \param f Fish to free
 */
void free_fish(void *f);

/*!
 * \fn supported_path(fish_t *f)
 * \brief Returns 1 if the fish has a supported moving style, 0 otherwise 
 * \param f Fish 
 */
int supported_path(fish_t *f);

/*!
 * \fn is_fish_in_view(view_t *v, fish_t *f)
 * \brief Returns 1 if the fish f is in view v, 0 otherwise 
 * \param v View
 * \param f Fish 
 */
int is_fish_in_view(view_t *v, fish_t *f);

/*!
 * \fn is_fish_new(fish_t *f)
 * \brief Returns 1 if the fish f is new (just added), 0 otherwise 
 * \param f Fish 
 */
int is_fish_new(fish_t *f);

/*!
 * \fn was_fish_in_view(view_t *v, fish_t *f)
 * \brief Returns 1 if the fish f was in view v, 0 otherwise 
 * \param v View
 * \param f Fish 
 */
int was_fish_in_view(view_t *v, fish_t *f);

/*!
 * \fn fish_just_entered_view(view_t *v, fish_t *f)
 * \brief Returns 1 if the fish f just entered in view v (or will enter it in the next fish-update-interval), 0 otherwise 
 * \param v View
 * \param f Fish 
 */
int fish_just_entered_view(view_t *v, fish_t *f);

/*!
 * \fn fish_just_left_view(view_t *v, fish_t *f)
 * \brief Returns 1 if the fish f just left the view v (or will leave it in the next fish-update-interval), 0 otherwise 
 * \param v View
 * \param f Fish 
 */
int fish_just_left_view(view_t *v, fish_t *f);

#endif
