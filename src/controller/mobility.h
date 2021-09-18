#ifndef MOBILITY_H
#define MOBILITY_H
#include "aquarium.h"
#include <time.h>
#include <stdlib.h>

/*!
 * \fn borderx(aquarium* aq, fish_t* f)
 * \brief Returns 1 if fish f tries to escape aquarium aq horizentally 
 * \param aq Aquarium
 * \param f Fish
 */
int borderx(aquarium* aq, fish_t* f);

/*!
 * \fn bordery(aquarium* aq, fish_t* f)
 * \brief Returns 1 if fish f tries to escape aquarium aq vertically
 * \param aq Aquarium
 * \param f Fish
 */
int bordery(aquarium* aq, fish_t* f);

/*!
 * \fn border(aquarium* aq, fish_t* f)
 * \brief Returns 1 if fish f tries to escape aquarium aq 
 * \param aq Aquarium
 * \param f Fish
 */
int border(aquarium* aq, fish_t* f);

/* --- */
/*!
 * \fn diagonal_L_D(aquarium* aq, fish_t* f, int xtickSpeed, int ytickSpeed)
 * \brief Moves or predicts Fish movement (going left down)  
 * \param aq Aquarium
 * \param f Fish
 * \param xtickSpeed Number to add to position horizentally
 * \param ytickSpeed Number to add to position vertically
 */
void diagonal_L_D(aquarium* aq, fish_t* f, int xtickSpeed, int ytickSpeed);

/*!
 * \fn diagonal_L_U(aquarium* aq, fish_t* f, int xtickSpeed, int ytickSpeed)
 * \brief Moves or predicts Fish movement (going left up)  
 * \param aq Aquarium
 * \param f Fish
 * \param xtickSpeed Number to add to position horizentally
 * \param ytickSpeed Number to add to position vertically 
 */
void diagonal_L_U(aquarium* aq, fish_t* f, int xtickSpeed, int ytickSpeed);

/*!
 * \fn diagonal_R_D(aquarium* aq, fish_t* f, int xtickSpeed, int ytickSpeed)
 * \brief Moves or predicts Fish movement (going right down)  
 * \param aq Aquarium
 * \param f Fish
 * \param xtickSpeed Number to add to position horizentally
 * \param ytickSpeed Number to add to position vertically 
 */
void diagonal_R_D(aquarium* aq, fish_t* f, int xtickSpeed, int ytickSpeed);

/*!
 * \fn diagonal_R_U(aquarium* aq, fish_t* f, int xtickSpeed, int ytickSpeed)
 * \brief Moves or predicts Fish movement (going right up)  
 * \param aq Aquarium
 * \param f Fish
 * \param xtickSpeed Number to add to position horizentally
 * \param ytickSpeed Number to add to position vertically 
 */
void diagonal_R_U(aquarium* aq, fish_t* f, int xtickSpeed, int ytickSpeed);

/*!
 * \fn front(aquarium *aq, fish_t* f, int tickSpeed)
 * \brief Moves or predicts Fish movement (going right)  
 * \param aq Aquarium
 * \param f Fish
 * \param tickSpeed Number to add to position horizentally 
 */
void front(aquarium *aq, fish_t* f, int tickSpeed);

/*!
 * \fn back(aquarium *aq, fish_t* f, int tickSpeed)
 * \brief Moves or predicts Fish movement (going left)  
 * \param aq Aquarium
 * \param f Fish
 * \param tickSpeed Number to add to position horizentally 
 */
void back(aquarium* aq, fish_t* f, int tickSpeed);

/*!
 * \fn ascend(aquarium *aq, fish_t* f, int tickSpeed)
 * \brief Moves or predicts Fish movement (going up)  
 * \param aq Aquarium
 * \param f Fish
 * \param tickSpeed Number to add to position vertically 
 */
void ascend(aquarium *aq, fish_t *f, int tickSpeed);

/*!
 * \fn drown(aquarium *aq, fish_t* f, int tickSpeed)
 * \brief Moves or predicts Fish movement (going down)  
 * \param aq Aquarium
 * \param f Fish
 * \param tickSpeed Number to add to position vertically 
 */
void drown(aquarium *aq, fish_t *f, int tickSpeed);
/* --- */

/*!
 * \fn horizental(aquarium *aq, fish_t* f, int tickSpeed)
 * \brief Moves or predicts Fish movement (going right then left the right ...)  
 * \param aq Aquarium
 * \param f Fish
 * \param tickSpeed Number to add to position horizentally 
 */
void horizental(aquarium* aq, fish_t* f, int tickSpeed);

/*!
 * \fn vertical(aquarium *aq, fish_t* f, int tickSpeed)
 * \brief Moves or predicts Fish movement (going up then down then up ...)  
 * \param aq Aquarium
 * \param f Fish
 * \param tickSpeed Number to add to position vertically 
 */
void vertical(aquarium* aq, fish_t* f, int tickSpeed);

/*!
 * \fn timeclock(aquarium* aq, fish_t* f, int xtickSpeed, int ytickSpeed)
 * \brief Moves or predicts Fish movement (following clock)  
 * \param aq Aquarium
 * \param f Fish
 * \param xtickSpeed Number to add to position horizentally
 * \param ytickSpeed Number to add to position vertically 
 */
void timeclock(aquarium* aq, fish_t* f, int xtickSpeed, int ytickSpeed);

/*!
 * \fn randomWayPoint(aquarium* aq, fish_t* f, int xtickSpeed, int ytickSpeed)
 * \brief Moves or predicts Fish movement (going randomly)  
 * \param aq Aquarium
 * \param f Fish
 * \param tickSpeed Number to limit the random  
 */
void randomWayPoint(aquarium* aq, fish_t* f, int tickSpeed);

/*!
 * \fn update_fish(aquarium *aq, fish_t *f, int tick, int real)
 * \brief updates fish position in aquarium  
 * \param aq Aquarium
 * \param f Fish
 * \param tick tick = fish-update-interval 
 * \param real 1 if you want this move to be trully applied, 0 if it is only a prediction
 * \param speedrandom Number used to set the mobilities' speed randomizer
 */
void update_fish(aquarium *aq, fish_t *f, int tick, int real, int speedrandom);

#endif