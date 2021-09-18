#ifndef AQUARIUM_H
#define AQUARIUM_H

#include "fish.h"
#include "list.h"
#include <sys/time.h>
#include <math.h>

/** @struct aquarium_s
 *  @brief This structure describes an aquarium
 *  @var aquarium_s::width
 *  Member 'width': contains the width of the aquarium
 *  @var aquarium_s::height
 *  Member 'height': contains the height of the aquarium
 *  @var aquarium_s::views
 *  Member 'views': contains the views of the aquarium
 *  @var aquarium_s::fishes
 *  Member 'fishes': contains the fishes of the aquarium
 *  @var aquarium_s::update_tick
 *  Member 'update_tick': contains the time interval of updating aquarium
 */
typedef struct aquarium_s {
    int width;
    int height;
    list_t *views;
    list_t *fishes;
    struct timeval update_tick; // Will be updated using gettimeofday whenever aquarium is updated
} aquarium;

/*!
 * \fn init_aquarium()
 * \brief Initializes aquarium 
 */
aquarium *init_aquarium();

/*!
 * \fn load_aquarium(aquarium *aq, char *filename)
 * \brief Load aquarium written in file filename into aquarium structure aq
 * \param aq aquarium where we load infos
 * \param filename File whre informations are written
 */
int load_aquarium(aquarium *aq, char *filename);

/*!
 * \fn save_aquarium(aquarium *aq, char *filename)
 * \brief Saves aquarium aq in file filename
 * \param aq Aquarium that will be saved 
 * \param filename File where aquarium infos will be saved
 */
void save_aquarium(aquarium *aq, char *filename);

/*!
 * \fn already_loaded(aquarium *aq)
 * \brief Returns 1 if aquarium aq is already loaded
 * \param aq Aquarium
 */
int already_loaded(aquarium *aq);

/*!
 * \fn show_aquarium(aquarium *aq)
 * \brief Prints aquarium representation in standard output
 * \param aq Aquarium
 */
void show_aquarium(aquarium *aq);

/*!
 * \fn add_view(aquarium *aq, char *buff)
 * \brief Adds view (represented by string view) to aquarium aq, returns 1 if view is successfully added, 0 otherwise
 * \param aq Aquarium where we will add the view
 * \param buff View represented by a string 
 */
int add_view(aquarium *aq, char *buff);

/*!
 * \fn del_view(aquarium *aq, char *buff)
 * \brief Deletes view (represented by string view) to aquarium aq, returns 1 if view is successfully added, 0 otherwise
 * \param aq Aquarium where we will delete the view
 * \param buff View represented by a string 
 */
int del_view(aquarium *aq, char *buff);

/*!
 * \fn finalize(aquarium *aq)
 * \brief Finalizes and frees aquarium
 * \param aq Aquarium to be finalized and freed
 */
void finalize(aquarium *aq);

/*!
 * \fn add_fish(aquarium *aq, view_t *v, char *fish)
 * \brief Adds fish (represented by string fish) to aquarium aq, taking into account the view which called the command, returns 1 if fish is successfully added, 0 otherwise
 * \param aq Aquarium where we will add the fish
 * \param v View taken into account when computing position based on pourcentage
 * \param fish Fish represented by a string
 */
int add_fish(aquarium *aq, view_t *v, char *fish);

/*!
 * \fn del_fish(aquarium *aq, char *fish)
 * \brief Deletes fish (represented by string fish) to aquarium aq, returns 1 if fish is successfully deleted, 0 otherwise
 * \param aq Aquarium where we will add the fish
 * \param fish Fish represented by a string 
 */
int del_fish(aquarium *aq, char *fish);

/*!
 * \fn start_fish(aquarium *aq, char *fish)
 * \brief Starts fish (represented by string fish) to aquarium aq, returns 1 if fish is successfully started, 0 otherwise
 * \param aq Aquarium where we will start the fish
 * \param fish Fish represented by a string 
 */
int start_fish(aquarium *aq, char *fish);

/*!
 * \fn start_all(aquarium *aq)
 * \brief Starts all fishes in aquarium
 * \param aq Aquarium where all fishes will be started
 */
void start_all(aquarium *aq);

/*!
 * \fn get_fishes(aquarium *aq, view_t *v, int tick, char *ret)
 * \brief Getting fishes in view v and returning them in ret buffer
 * \param aq Aquarium where we will be getting fishes
 * \param v View where we will be getting fishes
 * \param tick Usually it is equal to fish-update-interval
 * \param ret Return buffer
 */
void get_fishes(aquarium *aq, view_t *v, int tick, char *ret);

/*!
 * \fn get_fishes_continuously(aquarium *aq, view_t *v, int tick, char *ret)
 * \brief Not used pseudo-code: Sending fishes to client continously 3 successive times, period is fish-update-interval
 * \param aq Aquarium where we will be getting fishes
 * \param v View where we will be getting fishes
 * \param tick Usually it is equal to fish-update-interval
 * \param ret Return buffer
 */
void get_fishes_continuously(aquarium *aq,view_t *v, int tick, char *ret);

/*!
 * \fn ls(aquarium *aq, view_t *v, int tick, char* ret)
 * \brief Sending fishes to client continously, 3 successive pulses of informations predicted and returned in ret buffer instantly
 * \param aq Aquarium where we will be getting fishes
 * \param v View where we will be getting fishes
 * \param tick Usually it is equal to fish-update-interval
 * \param ret Return buffer
 * \param speedrandom Number used to set the mobilities' speed randomizer
 */
void ls(aquarium *aq, view_t *v, int tick, char* ret, int speedrandom);

/*!
 * \fn search_aq(aquarium *aq, int view_id)
 * \brief Searching aquarium aq for view identified by view_id, returns the view if found and free, NULL otherwise
 * \param aq Aquarium where we will be searching for view
 * \param view_id Identifier of searched view
 */
view_t* search_aq(aquarium *aq, int view_id);

/*!
 * \fn search_free_view(aquarium *aq)
 * \brief Searching aquarium aq for a free view, returns it if found, NULL otherwise
 * \param aq Aquarium where we will be searching for a free view
 */
view_t* search_free_view(aquarium *aq);

/*!
 * \fn print_fishes_in_view(aquarium* aq, view_t* v, char* buffer)
 * \brief Prints fishes in view in a certain way, used for status command
 * \param aq Aquarium 
 * \param v View 
 * \param buffer Buffer which will be filled with informations
 */
void print_fishes_in_view(aquarium* aq, view_t* v, char* buffer);

/*!
 * \fn update_fishes(aquarium* aq, int tick, int real)
 * \brief Updates fishes in aquarium
 * \param aq Aquarium 
 * \param tick Usually it is equal to fish-update-interval
 * \param real 1 if changes are applied, 0 if it is just a prediction
 * \param speedrandom Number used to set the mobilities' speed randomizer
 */
void update_fishes(aquarium* aq, int tick, int real, int speedrandom);

#endif