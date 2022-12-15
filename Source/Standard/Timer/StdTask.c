/**********************************************************************************************************************/
/**
 * @file        timer\StdTask.c
 *
 * @author      Stijn Vermeersch
 * @date        22.12.2010
 *
 * @brief       Module for running repetitive tasks with a fixed period.
 *
 * Here, the definition of a task is like, calling a function on a specified time.\n
 * When task are registered, they are not started at that time.  Start and stop functions are seperately available.
 * The period of a registered task can be altered anytime.  Eventually, one can choose to kill a registered task, and
 * free up the RAM space used by it inside this module.  The amount of tasks which can be registered at the same time
 * is specified in AppConfig.h
 *
 *
 * \n<hr>
 * Copyright (c) 2010, TVH\n
 * All rights reserved.
 * \n<hr>\n
 */
/**********************************************************************************************************************/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; V E R I F Y    C O N F I G U R A T I O N
;---------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief  Defines the max number of tasks one wants to support
 */
//#ifndef TASK_COUNT
//    #error "TASK_COUNT not defined in AppConfig.h"
//#endif
/**********************************************************************************************************************/



/***********************************************************************************************************************
; I N C L U D E S
;---------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include "..\..\System\PIC18F2X\SysLibAll.h"

//DRIVER lib include section
#include "..\..\Driver\PIC18F2X\DrvTimerTick.h"

//STANDARD lib include section
#include "StdTask.h"
/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   D E F I N I T I O N S   A N D   M A C R O S
;---------------------------------------------------------------------------------------------------------------------*/

#define DEFAULT_TASK_PERIOD    10
#define TASK_COUNT             8
/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   T Y P E D E F S
;---------------------------------------------------------------------------------------------------------------------*/
typedef struct
{
    UNSIGNED_16                 priority           : 8;
    UNSIGNED_16                 bitflag_asserted   : 1;
    UNSIGNED_16                 bitflag_sleeping   : 1;
    UNSIGNED_16                 bitflag_dummies    : 6;

    UNSIGNED_32                 us_per_task_period;
    UNSIGNED_32                 us_left;

    TASK_CALLBACK_FUNC          callback_func;
    VPTR                        callback_data_ptr;

    VPTR                        prev_task_ptr;
    VPTR                        next_task_ptr;
}
TASK_OBJECT;
/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   F U N C T I O N   P R O T O T Y P E S
;---------------------------------------------------------------------------------------------------------------------*/
static void StdTaskPlaceTaskIntoSortedChain(TASK_OBJECT* task_ptr);
/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/
static UNSIGNED_16                      stdtask_tick_period;
static UNSIGNED_16                      stdtask_count;
static TASK_OBJECT                      stdtask_array[TASK_COUNT];
static TASK_OBJECT*                     stdtask_first_task_ptr;
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/
/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
static void StdTaskPlaceTaskIntoSortedChain(TASK_OBJECT* task_ptr)
{
    TASK_OBJECT* loper_task_ptr;

    if(stdtask_first_task_ptr == NULL) /*** speciaal geval ***/
    {
        task_ptr->prev_task_ptr = NULL;
        task_ptr->next_task_ptr = NULL;
    }
    else
    {
        loper_task_ptr = stdtask_first_task_ptr;
        while(loper_task_ptr != NULL)
        {
            if(loper_task_ptr->priority > task_ptr->priority)  /*** loper has to become my next_task_ptr ***/
            {
                task_ptr->next_task_ptr = (VPTR)loper_task_ptr;
                task_ptr->prev_task_ptr = loper_task_ptr->prev_task_ptr;
                break;
            }
            else if(loper_task_ptr->next_task_ptr == NULL)  /*** loper has to become my previous_task ***/
            {
                task_ptr->next_task_ptr = NULL;
                task_ptr->prev_task_ptr = (VPTR)loper_task_ptr;
                break;
            }
            loper_task_ptr = (TASK_OBJECT*) loper_task_ptr->next_task_ptr;
        }
    }

    DrvTimerTickDisableInterrupt();
    if(task_ptr->next_task_ptr != NULL)
    {
        ((TASK_OBJECT*)task_ptr->next_task_ptr)->prev_task_ptr = (VPTR)task_ptr;
    }
    if(task_ptr->prev_task_ptr != NULL)
    {
        ((TASK_OBJECT*)task_ptr->prev_task_ptr)->next_task_ptr = (VPTR)task_ptr;
    }
    else
    {
        stdtask_first_task_ptr = task_ptr;
    }
    DrvTimerTickEnableInterrupt();
}
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
void StdTaskInit(UNSIGNED_16 tick_period_in_us)
{
    TASK_OBJECT* task_ptr;

    stdtask_tick_period = tick_period_in_us;
    stdtask_count = 0;
    stdtask_first_task_ptr = NULL;

    task_ptr = &stdtask_array[0];
    while(task_ptr < &stdtask_array[TASK_COUNT])
    {
        task_ptr->us_per_task_period    = DEFAULT_TASK_PERIOD;
        task_ptr->us_left               = DEFAULT_TASK_PERIOD;
        task_ptr->callback_func         = NULL;
        task_ptr->callback_data_ptr     = NULL;
        task_ptr->priority              = 128;
        task_ptr->bitflag_asserted      = 0;
        task_ptr->bitflag_sleeping      = 1;
        task_ptr++;
    }
    
}
/*--------------------------------------------------------------------------------------------------------------------*/
void StdTaskTick(void)
{
    TASK_OBJECT* loper_task_ptr;
    TASK_OBJECT* next_loper_task_ptr; //usefull in case of task killed by its own task_callback_func;

    loper_task_ptr = stdtask_first_task_ptr;
    while(loper_task_ptr != NULL)
    {
        next_loper_task_ptr = (TASK_OBJECT*) loper_task_ptr->next_task_ptr;
        if(loper_task_ptr->bitflag_sleeping == 0) /*** TASK is active ***/
        {
            if(loper_task_ptr->us_left > stdtask_tick_period)
            {
                loper_task_ptr->us_left-= stdtask_tick_period;
            }
            else /*** TASK is ASSERTED ***/
            {
                loper_task_ptr->us_left = loper_task_ptr->us_per_task_period;  //reset ticks to period
                if(loper_task_ptr->priority < 128)  /*** TASK handled on interrupt ***/
                {
                    loper_task_ptr->bitflag_asserted = 1;
					loper_task_ptr->callback_func(loper_task_ptr->callback_data_ptr); //execute callback function
					loper_task_ptr->bitflag_asserted = 0;
                }
                else  /*** TASK handled in background ***/
                {
                    loper_task_ptr->bitflag_asserted = 1; //set flag to notify background
                }
            }
        }
        loper_task_ptr = next_loper_task_ptr;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
void StdTaskHandler(void)
{
    TASK_OBJECT* loper_task_ptr;
    TASK_OBJECT* next_loper_task_ptr; //usefull in case of task killed by its own task_callback_func;

    loper_task_ptr = stdtask_first_task_ptr;
    while(loper_task_ptr != NULL)
    {
        next_loper_task_ptr = (TASK_OBJECT*) loper_task_ptr->next_task_ptr;
        if((loper_task_ptr->bitflag_sleeping == 0) && (loper_task_ptr->bitflag_asserted == 1))
        {
            // TASK is active & asserted 
            loper_task_ptr->bitflag_asserted = 0;
            loper_task_ptr->callback_func(loper_task_ptr->callback_data_ptr); //execute callback function
        }
        loper_task_ptr = next_loper_task_ptr;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
TASK_HNDL StdTaskRegisterTask(UNSIGNED_32            task_period_in_us,
                              TASK_CALLBACK_FUNC     callback,
                              VPTR                   data_ptr,
                              UNSIGNED_8             priority)
{
    UNSIGNED_8 i;
    TASK_OBJECT* loper_task_ptr;

    stdtask_count++;
    loper_task_ptr = &stdtask_array[0];

    for(i = 0 ; i < TASK_COUNT ; i++)
    {
        if(stdtask_array[i].callback_func == NULL)
        {
            break;
        }
    }
    loper_task_ptr = &stdtask_array[i];

    loper_task_ptr->us_per_task_period    = task_period_in_us;
    loper_task_ptr->us_left               = task_period_in_us;
    loper_task_ptr->callback_func         = callback;
    loper_task_ptr->callback_data_ptr     = data_ptr;

    loper_task_ptr->priority              = priority;
    loper_task_ptr->bitflag_asserted      = 0;
    loper_task_ptr->bitflag_sleeping      = 1; /*** user must explicitly START the task ***/

    StdTaskPlaceTaskIntoSortedChain(loper_task_ptr);

    return i;
}
/*--------------------------------------------------------------------------------------------------------------------*/
BOOLEAN StdTaskSetPeriod(TASK_HNDL task, UNSIGNED_32 task_period_in_us)
{
    TASK_OBJECT* task_ptr;

    if(task >= TASK_COUNT)
    {
        return FALSE;
	}

    task_ptr = &stdtask_array[task];
    if(task_ptr->callback_func == NULL)
    {
		return FALSE;
	}

    task_ptr->us_per_task_period = task_period_in_us;
    return TRUE;
}
/*--------------------------------------------------------------------------------------------------------------------*/
BOOLEAN StdTaskStart(TASK_HNDL task)
{
    TASK_OBJECT* task_ptr;

    if(task >= TASK_COUNT)
    {
        return FALSE;
    }

    task_ptr = &stdtask_array[task];
    if(task_ptr->callback_func == NULL)
	{
		return FALSE;
	}
    task_ptr->us_left = task_ptr->us_per_task_period;  //reset ticks to period
    task_ptr->bitflag_asserted = 0;
    task_ptr->bitflag_sleeping = 0;
    return TRUE;
}
/*--------------------------------------------------------------------------------------------------------------------*/
BOOLEAN StdTaskStop(TASK_HNDL task)
{
    TASK_OBJECT* task_ptr;

    if(task >= TASK_COUNT)
    {
        return FALSE;
    }

    task_ptr = &stdtask_array[task];
    if(task_ptr->callback_func == NULL)
	{
		return FALSE;
	}
    task_ptr->bitflag_sleeping = 1;
    return TRUE;
}
/*--------------------------------------------------------------------------------------------------------------------*/
BOOLEAN StdTaskKill(TASK_HNDL task)
{
    TASK_OBJECT* task_ptr;

    if(task >= TASK_COUNT)
    {
        return FALSE;
    }

    task_ptr = &stdtask_array[task];
    if(task_ptr->callback_func == NULL)
	{
		return FALSE;
	}

    DrvTimerTickDisableInterrupt();
    if(task_ptr->prev_task_ptr != NULL)
    {
        ((TASK_OBJECT*)task_ptr->prev_task_ptr)->next_task_ptr = task_ptr->next_task_ptr;
    }
    else
    {
        stdtask_first_task_ptr = (TASK_OBJECT*)task_ptr->next_task_ptr;
    }
    if(task_ptr->next_task_ptr != NULL)
    {
        ((TASK_OBJECT*)task_ptr->next_task_ptr)->prev_task_ptr = task_ptr->prev_task_ptr;
    }
    DrvTimerTickEnableInterrupt();
    task_ptr->us_per_task_period    = DEFAULT_TASK_PERIOD;
    task_ptr->us_left               = DEFAULT_TASK_PERIOD;
    task_ptr->callback_func         = NULL;
    task_ptr->callback_data_ptr     = NULL;
    task_ptr->priority              = 128;
    task_ptr->bitflag_asserted      = 0;
    task_ptr->bitflag_sleeping      = 1;

    stdtask_count--;

    return TRUE;
}
/*--------------------------------------------------------------------------------------------------------------------*/
BOOLEAN StdTaskIsTaskRunning(TASK_HNDL task)
{
    TASK_OBJECT* task_ptr;

    if(task >= TASK_COUNT)
    {
        return FALSE;
    }

    task_ptr = &stdtask_array[task];
    if(task_ptr->callback_func == NULL)
	{
		return FALSE;
	}

    if (task_ptr->bitflag_sleeping == 0)
    {
        return TRUE;
    }

    return FALSE;

}
/*--------------------------------------------------------------------------------------------------------------------*/
BOOLEAN StdTaskIsTaskActive(TASK_HNDL task)
{
	TASK_OBJECT* task_ptr;

    if(task >= TASK_COUNT)
    {
        return FALSE;
    }

    task_ptr = &stdtask_array[task];
	if(task_ptr->callback_func == NULL)
	{
		return FALSE;
	}
	if(task_ptr->bitflag_asserted == 1)
	{
		return TRUE;
	}

	return FALSE;

}
/**********************************************************************************************************************/
