/**
* @file
* @brief Contains functions for checking, reseting and updating the elevator order queue.
*/
#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdio.h>
#include <stdlib.h>
#include "elev.h"
#include "controller.h"


/**
* @brief Matrix containing orders.
* 0 is standard value, 1 represent an order.\n
* | 1	| 2	| 3	| 4	|\n
* | x	| x	| x	| x	| UP\n
* | x	| x	| x	| x	| DOWN\n
* | x | x | x | x | ELEV\n
*/
int queue[3][N_FLOORS];

/**
* @brief Iterates through @p queue to check if any orders have been made
*
* @return 0 when queue is empty, 1 otherwise
*/
int checkForOrders();

/**
* @brief Updates the matrix @p queue if an order button has been pressed.
*/
void updateOrderQueue();

/**
* @brief Sets all values in order queue to 0, and turns off all the order
*button lights.
*/
void resetQueueAndLights();

/**
* @brief Clears all orders made from a given @p floor in the order queue.
*
* @param int floor is the floor where all orders are cleared.
*/
void removeFromOrder(int floor);
#endif
