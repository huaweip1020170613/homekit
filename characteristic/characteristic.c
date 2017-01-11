/****************************************************************************
 *
 * MODULE:             Characteristic.c
 *
 * COMPONENT:          homekit interface
 *
 * REVISION:           $Revision:  1.0$
 *
 * DATED:              $Date: 2017-01-11 15:13:17 +0100 (Fri, 12 Dec 2016 $
 *
 * AUTHOR:             PCT
 *
 ****************************************************************************
 *
 * Copyright panchangtao@gmail.com 2017. All rights reserved
 *
 ***************************************************************************/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "characteristic.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
tsCharacteristics asCharacteristics[] = {
    {"public.hap.characteristic.name", E_CHARACTERISTIC_NAME, E_TYPE_STRING, {.psData="NULL"}, E_PERM_PAIRED_READ, NULL, E_UNIT_NULL, {T_FALSE,{0}}, {T_FALSE,{0}}, {T_FALSE,{0}}},
    {"public.hap.characteristic.on", E_CHARACTERISTIC_ON, E_TYPE_BOOL, {.bData=T_FALSE}, E_PERM_READ | E_PERM_WRITE | E_PERM_PAIRED_WRITE | E_PERM_EVENT_NOTIFYCATION, NULL, E_UNIT_NULL, {T_FALSE,{0}}, {T_FALSE,{0}}, {T_FALSE,{0}}},
};
/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/


