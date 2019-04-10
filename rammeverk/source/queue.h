/**
* @file
* @brief Description of Queue
*/
#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdio.h>
#include <stdlib.h>
#include "elev.h"
#include "elevator.h"


/**
* @brief Matrix containing orders.
* 0 is standard value, 1 represent an order.
*       | 1	| 2	| 3	| 4	|
* 	UP	| x	| x	| x	| x	|
*  Down	| x	| x	| x	| x	|
*  Elev | x | x | x | x |
*/
int queue[3][N_FLOORS];

/**
* @brief Iterates @p queue to check for orders
*
* @return 0 when queue is empty, 1 otherwise
*/
int checkForOrders();

/**
* @brief Updates the matrix @p queue if a button is pressed
*/
void updateOrderQueue();

/**
* @brief Sets all values in @p queue to 0, and turns off all the lights
*/
void resetQueueAndLights();

/**
* @brief Sets all values for a specific floor to 0 in @p queue
*/
void removeFromOrder(int floor);
#endif
