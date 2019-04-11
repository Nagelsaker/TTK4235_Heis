/**
* @file
* @brief Contains functions that control a timer
*/
#ifndef __TIMER_H__
#define __TIMER_H__

#include <time.h>

/**
* @brief Starts the timer
*/
void startTimer();

/**
* @brief Stops the timer
*/
void stopTimer();

/**
* @brief Checks whether the timer is currently running
*
* @return 0 if timer is not running, 1 if it is running
*/
int hasTimerStarted();

/**
* @brief Checks whether the timer has passed @p x seconds
*
* @param[in] int x  Number of seconds
*
* @return 0 if timer has not passed @p x seconds, 1 else
*/
int hasTimerPassedXs(int x);
#endif
